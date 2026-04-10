use std::{
    collections::{HashMap, HashSet},
    env,
    fs::{self, read_to_string, File},
    io::{BufRead, BufReader, BufWriter, Write},
    path::Path,
    process,
};

#[derive(Debug, Clone)]
enum AST {
    Alt(Box<AST>, Box<AST>),
    Seq(Box<AST>, Box<AST>),
    Star(Box<AST>),
    Plus(Box<AST>),
    Range(char, char),
    Char(char),
    Lambda,
    Dot,
}

#[derive(Debug)]
struct NFA {
    pub states: usize,
    pub transitions: Vec<(usize, usize, char)>,
    pub lambda_transitions: Vec<(usize, usize)>,
    pub start: usize,
    pub accept_states: Vec<(usize, String)>,
}

// ALPHABETENCODING
fn alphabet_decode(s: &str) -> String {
    let mut decoded = String::new();
    let chars: Vec<char> = s.chars().collect();
    let mut i = 0;
    while i < chars.len() {
        let c = chars[i];
        if c == ' ' {
            i += 1;
            continue;
        }
        if c == 'x' && i + 2 < chars.len() {
            let hex = format!("{}{}", chars[i + 1], chars[i + 2]);
            if let Ok(digit) = u32::from_str_radix(&hex, 16) {
                if let Some(ch) = char::from_u32(digit) {
                    decoded.push(ch);
                    i += 3;
                    continue;
                }
            }
            decoded.push(c);
            i += 1;
        } else {
            decoded.push(c);
            i += 1;
        }
    }
    decoded
}

fn alphabet_encode_char(c: char) -> String {
    if !c.is_ascii_graphic() || c.is_ascii_whitespace() {
        format!("x{:02x}", c as u32)
    } else {
        match c {
            ':' => "x3a".to_string(),
            '\\' => "x5c".to_string(),
            'x' => "x78".to_string(),
            _ => c.to_string(),
        }
    }
}

// ---------- REC MODE ---------- //
pub fn run_rec_mode(scan_lut_path: &str, scan_u_path: &str) {
    let content = std::fs::read_to_string(scan_lut_path).unwrap_or_else(|_| {
        eprintln!("Failed to read {}", scan_lut_path);
        process::exit(1);
    });

    let mut lines = content.lines();
    let alphabet_line = lines.next().unwrap();
    let decoded_alphabet = alphabet_decode(alphabet_line);
    let alphabet_set: HashSet<char> = decoded_alphabet.chars().collect();

    // Find a lambda char not in alphabet
    let lambda = (1..=255)
        .filter_map(|n| std::char::from_u32(n))
        .find(|c| !alphabet_set.contains(c))
        .unwrap_or_else(|| {
            eprintln!("Failed to find lambda character not in alphabet");
            process::exit(1);
        });

    let mut writer = BufWriter::new(File::create(scan_u_path).unwrap_or_else(|_| {
        eprintln!("Failed to create {}", scan_u_path);
        process::exit(1);
    }));

    let alphabet_encoded: Vec<String> =
        decoded_alphabet.chars().map(alphabet_encode_char).collect();
    writeln!(writer, "{}", alphabet_encoded.join(" ")).unwrap();

    let mut token_index = 1;

    for line in lines {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() < 2 {
            continue;
        }

        let regex = parts[0];
        let token_name = parts[1];

        // Parse regex into AST
        let ast = match parse_regex_full(regex, &alphabet_set) {
            Ok(a) => a,
            Err(code) => process::exit(code.into()),
        };

        // Convert AST to NFA
        let nfa = ast_to_nfa(&ast, lambda, &alphabet_set, &token_index.to_string());

        // Write NFA file
        let nfa_filename = format!("{}.nfa", token_name);
        write_nfa_file(&nfa_filename, &nfa, lambda, &alphabet_set);

        // Write to scan.u
        write!(writer, "{}.tt {}", token_name, token_name).unwrap();
        for extra in &parts[2..] {
            write!(writer, " {}", extra).unwrap();
        }
        writeln!(writer).unwrap();

        token_index += 1;
    }
}
// ----------------- PARSER ----------------- //
fn parse_regex_full(regex: &str, alphabet_set: &HashSet<char>) -> Result<AST, u8> {
    let chars: Vec<char> = regex.chars().collect();
    let mut pos = 0;

    let ast = parse_alt(&chars, &mut pos, alphabet_set)?;

    if pos != chars.len() {
        return Err(2); // syntax error
    }

    Ok(ast)
}

fn parse_alt(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> Result<AST, u8> {
    let mut node = parse_seq(chars, pos, alphabet_set)?;
    while *pos < chars.len() && chars[*pos] == '|' {
        *pos += 1;
        let right = parse_seq(chars, pos, alphabet_set)?;
        node = AST::Alt(Box::new(node), Box::new(right));
    }
    Ok(node)
}

fn parse_seq(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> Result<AST, u8> {
    let mut nodes = Vec::new();
    while *pos < chars.len() && chars[*pos] != '|' && chars[*pos] != ')' {
        nodes.push(parse_factor(chars, pos, alphabet_set)?);
    }
    let ast = nodes
        .into_iter()
        .reduce(|a, b| AST::Seq(Box::new(a), Box::new(b)))
        .unwrap_or(AST::Lambda);
    Ok(ast)
}

fn parse_factor(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> Result<AST, u8> {
    let mut node = parse_base(chars, pos, alphabet_set)?;
    while *pos < chars.len() && (chars[*pos] == '*' || chars[*pos] == '+') {
        node = match chars[*pos] {
            '*' => AST::Star(Box::new(node)),
            '+' => AST::Plus(Box::new(node)),
            _ => unreachable!(),
        };
        *pos += 1;
    }
    Ok(node)
}

fn parse_base(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> Result<AST, u8> {
    if *pos >= chars.len() {
        return Ok(AST::Lambda);
    }

    let c = chars[*pos];

    if c == '(' {
        *pos += 1;
        let node = parse_alt(chars, pos, alphabet_set)?;
        if *pos >= chars.len() || chars[*pos] != ')' {
            return Err(2); //syntax error
        }
        *pos += 1;
        Ok(node)
    } else if c == '\\' {
        *pos += 1;
        if *pos >= chars.len() {
            return Err(2); //syntax error
        }

        let escaped = chars[*pos];

        let actual = match escaped {
            'n' => '\n',
            't' => '\t',
            's' => ' ',
            'r' => '\r',
            _ => escaped,
        };
        *pos += 1;

        if !alphabet_set.contains(&actual) {
            return Err(4);
        }

        // Check for range: escaped_char '-' next_char
        if *pos + 1 < chars.len() && chars[*pos] == '-' {
            let after_dash = chars[*pos + 1];
            // Next thing must be an escaped char or alphabet char (not a metachar)
            if after_dash == '\\' {
                // Range end is an escaped character
                *pos += 1; // skip '-'
                *pos += 1; // skip '\'
                if *pos >= chars.len() {
                    return Err(2);
                }
                let end_escaped = chars[*pos];
                let end_actual = match end_escaped {
                    'n' => '\n',
                    't' => '\t',
                    's' => ' ',
                    'r' => '\r',
                    _ => end_escaped,
                };
                *pos += 1;
                if !alphabet_set.contains(&end_actual) {
                    return Err(4);
                }
                if actual > end_actual {
                    return Err(3); // semantic error: inverted range
                }
                return Ok(expand_range_filtered(actual, end_actual, &alphabet_set));
            } else if alphabet_set.contains(&after_dash) && !"|*+()".contains(after_dash) {
                // Range end is a plain alphabet char
                let end_actual = after_dash;
                *pos += 1; // skip '-'
                *pos += 1; // skip end char
                if actual > end_actual {
                    return Err(3);
                }
                return Ok(expand_range_filtered(actual, end_actual, &alphabet_set));
            }
        }

        Ok(AST::Char(actual))
    } else if c == '.' {
        *pos += 1;
        Ok(AST::Dot)
    } else if c == '-' {
        if *pos + 2 < chars.len() && chars[*pos + 1] == '-' {
            *pos += 1;
            return Err(2); //syntax error
        } else if *pos >= 1 && *pos + 1 < chars.len() && chars[*pos + 1] == '-' {
            let start = chars[*pos - 1];
            let end = chars[*pos + 1];
            if alphabet_set.contains(&start) && alphabet_set.contains(&end) && start <= end {
                *pos += 2;
                return Ok(expand_range(start, end));
            } else {
                return Err(2); //syntax error
            }
        } else {
            *pos += 1;
            if alphabet_set.contains(&'-') || (*pos < chars.len()) {
                Ok(AST::Char('-'))
            } else {
                Err(2) //syntax error
            }
        }
    } else if alphabet_set.contains(&c) {
        // if current character in alphabet, filter range
        if *pos + 2 < chars.len() && chars[*pos + 1] == '-' {
            let start = c;
            let end = chars[*pos + 2];
            if alphabet_set.contains(&end) {
                if start > end {
                    return Err(3);
                }
                *pos += 3;
                return Ok(expand_range_filtered(start, end, &alphabet_set));
            }
        }

        *pos += 1;
        Ok(AST::Char(c))
    } else if c == ')' {
        Err(2)
    } else if c == '*' || c == '+' || c == '|' {
        Err(2)
    } else {
        Err(4)
    }
}

fn expand_range(a: char, b: char) -> AST {
    let mut nodes = Vec::new();
    for c in a..=b {
        nodes.push(AST::Char(c));
    }
    nodes
        .into_iter()
        .reduce(|x, y| AST::Alt(Box::new(x), Box::new(y)))
        .unwrap()
}

// filter out stuff not in alphabet
fn expand_range_filtered(a: char, b: char, alphabet_set: &HashSet<char>) -> AST {
    let mut nodes = Vec::new();
    for c in a..=b {
        if alphabet_set.contains(&c) {
            nodes.push(AST::Char(c));
        }
    }
    nodes
        .into_iter()
        .reduce(|x, y| AST::Alt(Box::new(x), Box::new(y)))
        .unwrap_or(AST::Lambda)
}

// ----------------- NFA Part ----------------- //
fn ast_to_nfa(ast: &AST, lambda: char, alphabet_set: &HashSet<char>, token_id: &str) -> NFA {
    let mut state_counter = 0;
    let mut transitions = Vec::new();
    let mut lambda_transitions = Vec::new();

    let (start, accept) = build(
        ast,
        lambda,
        alphabet_set,
        &mut state_counter,
        &mut transitions,
        &mut lambda_transitions,
    );

    if start != 0 {
        let swap = |s: usize| -> usize {
            if s == start {
                0
            } else if s == 0 {
                start
            } else {
                s
            }
        };

        for t in transitions.iter_mut() {
            t.0 = swap(t.0);
            t.1 = swap(t.1);
        }
        for t in lambda_transitions.iter_mut() {
            t.0 = swap(t.0);
            t.1 = swap(t.1);
        }

        let new_accept = swap(accept);

        NFA {
            states: state_counter,
            transitions,
            lambda_transitions,
            start: 0,
            accept_states: vec![(new_accept, token_id.to_string())],
        }
    } else {
        NFA {
            states: state_counter,
            transitions,
            lambda_transitions,
            start: 0,
            accept_states: vec![(accept, token_id.to_string())],
        }
    }
}

fn build(
    ast: &AST,
    lambda: char,
    alphabet_set: &HashSet<char>,
    state_counter: &mut usize,
    transitions: &mut Vec<(usize, usize, char)>,
    lambda_transitions: &mut Vec<(usize, usize)>,
) -> (usize, usize) {
    match ast {
        AST::Char(c) => {
            let s = *state_counter;
            *state_counter += 1;
            let e = *state_counter;
            *state_counter += 1;
            transitions.push((s, e, *c));
            (s, e)
        }
        AST::Dot => {
            let s = *state_counter;
            *state_counter += 1;
            let e = *state_counter;
            *state_counter += 1;
            for &c in alphabet_set {
                if c != lambda {
                    transitions.push((s, e, c));
                }
            }
            (s, e)
        }
        AST::Lambda => {
            let s = *state_counter;
            *state_counter += 1;
            (s, s)
        }
        AST::Alt(l, r) => {
            let (ls, le) = build(
                l,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );
            let (rs, re) = build(
                r,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );

            if ls == rs && le == re {
                (ls, le)
            } else {
                let start = *state_counter;
                *state_counter += 1;
                let end = *state_counter;
                *state_counter += 1;
                lambda_transitions.push((start, ls));
                lambda_transitions.push((start, rs));
                lambda_transitions.push((le, end));
                lambda_transitions.push((re, end));
                (start, end)
            }
        }
        AST::Seq(l, r) => {
            let (ls, le) = build(
                l,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );
            let (rs, re) = build(
                r,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );
            lambda_transitions.push((le, rs));
            (ls, re)
        }
        AST::Star(x) => {
            let (xs, xe) = build(
                x,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );
            let start = *state_counter;
            *state_counter += 1;
            let end = *state_counter;
            *state_counter += 1;
            lambda_transitions.push((start, xs));
            lambda_transitions.push((start, end));
            lambda_transitions.push((xe, xs));
            lambda_transitions.push((xe, end));
            (start, end)
        }
        AST::Plus(x) => {
            let (xs, xe) = build(
                x,
                lambda,
                alphabet_set,
                state_counter,
                transitions,
                lambda_transitions,
            );
            let start = *state_counter;
            *state_counter += 1;
            let end = *state_counter;
            *state_counter += 1;
            lambda_transitions.push((start, xs));
            lambda_transitions.push((xe, xs));
            lambda_transitions.push((xe, end));
            (start, end)
        }
        AST::Range(a, b) => {
            let s = *state_counter;
            *state_counter += 1;
            let e = *state_counter;
            *state_counter += 1;
            for c in (*a..=*b) {
                transitions.push((s, e, c));
            }
            (s, e)
        }
    }
}

fn write_nfa_file(filename: &str, nfa: &NFA, lambda: char, alphabet_set: &HashSet<char>) {
    use std::collections::BTreeMap;

    let file = File::create(Path::new(filename)).unwrap_or_else(|_| {
        eprintln!("Failed to create {}", filename);
        process::exit(1);
    });
    let mut writer = BufWriter::new(file);

    // Write header
    write!(writer, "{} {}", nfa.states, alphabet_encode_char(lambda)).unwrap();

    // Write alphabet
    let mut alphabet: Vec<char> = alphabet_set
        .iter()
        .filter(|&&c| c != lambda)
        .cloned()
        .collect();
    alphabet.sort();

    for c in &alphabet {
        write!(writer, " {}", alphabet_encode_char(*c)).unwrap();
    }
    writeln!(writer).unwrap();

    // Group transitions
    let mut transition_map: BTreeMap<(usize, usize), Vec<char>> = BTreeMap::new();

    // Add lambda transitions
    for (from, to) in &nfa.lambda_transitions {
        transition_map
            .entry((*from, *to))
            .or_insert_with(Vec::new)
            .push(lambda);
    }

    // Add character transitions
    for (from, to, symbol) in &nfa.transitions {
        transition_map
            .entry((*from, *to))
            .or_insert_with(Vec::new)
            .push(*symbol);
    }

    // Write transitions
    for ((from, to), chars) in transition_map.iter() {
        write!(writer, "- {} {}", from, to).unwrap();
        let mut sorted_chars = chars.clone();
        sorted_chars.sort();
        for c in sorted_chars {
            write!(writer, " {}", alphabet_encode_char(c)).unwrap();
        }
        writeln!(writer).unwrap();
    }

    // Write accept states
    for (state, token_id) in &nfa.accept_states {
        writeln!(writer, "+ {} {}", state, token_id).unwrap();
    }
}

// ***** CHECK MODE (Wally only) ***** //
#[derive(Clone, Debug)]
struct CFG {
    start: String,
    nonterms: HashMap<String, bool>, // value = directly has lambda production?
    terms: HashSet<String>,
    prods: HashMap<String, HashSet<Vec<String>>>,
}

// FROM LGA
fn read_cfg(filename: &Path) -> Result<CFG, String> {
    let file = File::open(filename)
        .map_err(|e| format!("Failed to open {}: {}", filename.display(), e))?;
    let reader = BufReader::new(file);

    let mut terms: HashSet<String> = HashSet::new();
    let mut nonterms: HashMap<String, bool> = HashMap::new();
    let mut cfg: HashMap<String, HashSet<Vec<String>>> = HashMap::new();
    let mut start = String::new();
    let mut last_nonterm = String::new();

    for line in reader.lines() {
        let line = match line {
            Ok(l) => l,
            Err(e) => return Err(format!("Read error: {}", e)),
        };

        // Split by ->
        let v: Vec<&str> = line.split("->").collect();
        if v.len() > 1 {
            // Found LHS
            last_nonterm = v[0].trim().to_string();
            terms.remove(&last_nonterm);
            nonterms.insert(last_nonterm.clone(), false);

            if !cfg.contains_key(&last_nonterm) {
                cfg.insert(last_nonterm.clone(), HashSet::new());
            }
        }

        if last_nonterm.is_empty() {
            continue;
        }

        // Process production rules from the RHS (or continuation lines)
        for r in v[v.len() - 1].split('|') {
            let rule = r.trim().to_string();
            if rule.is_empty() {
                continue;
            }

            if rule == "lambda" {
                nonterms
                    .entry(last_nonterm.clone())
                    .and_modify(|has_lambda| *has_lambda = true);

                let prod_v: Vec<String> = vec!["".to_string()];
                match cfg.get_mut(&last_nonterm) {
                    Some(x) => {
                        x.insert(prod_v);
                    }
                    None => return Err("Nonexistent nonterminal.".to_string()),
                }
            } else {
                let prod_v: Vec<String> = rule.split_whitespace().map(|s| s.to_string()).collect();

                // Detect start/goal symbol: production ending with $
                if prod_v.last().map(|s| s.as_str()) == Some("$") {
                    start = last_nonterm.clone();
                }

                match cfg.get_mut(&last_nonterm) {
                    Some(x) => {
                        x.insert(prod_v.clone());
                    }
                    None => return Err("Nonexistent nonterminal.".to_string()),
                }

                for symbol in prod_v.into_iter() {
                    if !nonterms.contains_key(&symbol) {
                        terms.insert(symbol);
                    }
                }
            }
        }
    }

    Ok(CFG {
        nonterms,
        terms,
        prods: cfg,
        start,
    })
}

// FROM LGA
fn derives_to_lambda(l: &str, cfg: &CFG, stack: &mut Vec<Vec<String>>) -> bool {
    if let Some(prods) = cfg.prods.get(l) {
        for p in prods {
            if stack.contains(p) {
                continue;
            }

            // Direct lambda production
            if cfg.nonterms.get(l).copied().unwrap_or(false) && is_lambda_prod(p) {
                return true;
            }

            // If RHS contains a terminal, can't derive lambda
            if p.iter().any(|sym| cfg.terms.contains(sym)) {
                continue;
            }

            let mut all_derive_lambda = true;

            for xi in p {
                if xi.is_empty() {
                    continue;
                }
                stack.push(p.clone());
                all_derive_lambda = derives_to_lambda(xi, cfg, stack);
                stack.pop();

                if !all_derive_lambda {
                    break;
                }
            }

            if all_derive_lambda {
                return true;
            }
        }
    }
    false
}

// FROM LGA
fn is_lambda_prod(prod: &Vec<String>) -> bool {
    prod.len() == 1 && prod[0].is_empty()
}

// FROM LGA
fn first_set(sequence: &[String], cfg: &CFG, t: &mut HashSet<String>) -> HashSet<String> {
    if sequence.is_empty() {
        return HashSet::new();
    }

    let x = &sequence[0];
    let beta = &sequence[1..];

    // If X is a terminal or $, return {X}
    if cfg.terms.contains(x) || x == "$" {
        return HashSet::from([x.clone()]);
    }

    // If X is the empty-string marker (lambda prod), skip it
    if x.is_empty() {
        return HashSet::new();
    }

    let mut f: HashSet<String> = HashSet::new();

    // X is a nonterminal
    if !t.contains(x) {
        t.insert(x.clone());

        if let Some(prods) = cfg.prods.get(x) {
            for rhs in prods {
                let g = first_set(rhs, cfg, t);
                f.extend(g);
            }
        }
    }

    // If X derives to lambda and there's more in the sequence
    if !beta.is_empty() {
        let mut stack: Vec<Vec<String>> = Vec::new();
        if derives_to_lambda(x, cfg, &mut stack) {
            let g = first_set(beta, cfg, t);
            f.extend(g);
        }
    }

    f
}
// FROM LGA
fn follow_set(a: &str, cfg: &CFG, t: &mut HashSet<String>) -> HashSet<String> {
    let mut set: HashSet<String> = HashSet::new();

    if a == cfg.start {
        set.insert("$".to_string());
    }

    if t.contains(a) {
        return set;
    }
    t.insert(a.to_string());

    for (b, prods) in &cfg.prods {
        for rhs in prods {
            for i in 0..rhs.len() {
                if rhs[i] != a {
                    continue;
                }

                let beta = &rhs[i + 1..];

                // Add first(β) to follow(A)
                let mut first_t: HashSet<String> = HashSet::new();
                let first_beta = first_set(beta, cfg, &mut first_t);
                set.extend(first_beta);

                // If β derives to lambda, add follow(B) to follow(A)
                let beta_nullable = beta.is_empty()
                    || beta.iter().all(|sym| {
                        if sym.is_empty() {
                            return true;
                        }
                        let mut stack: Vec<Vec<String>> = Vec::new();
                        derives_to_lambda(sym, cfg, &mut stack)
                    });

                if beta_nullable {
                    let follow_b = follow_set(b, cfg, t);
                    set.extend(follow_b);
                }
            }
        }
    }

    set
}

// FROM LGA
fn predict_set(a: &str, rhs: &Vec<String>, cfg: &CFG) -> HashSet<String> {
    let mut first_t = HashSet::new();
    let mut set = first_set(rhs, cfg, &mut first_t);

    let rhs_derives_lambda = is_lambda_prod(rhs)
        || rhs.iter().all(|sym| {
            if sym.is_empty() {
                return true;
            }
            let mut stack: Vec<Vec<String>> = Vec::new();
            derives_to_lambda(sym, cfg, &mut stack)
        });

    if rhs_derives_lambda {
        let mut follow_t = HashSet::new();
        let follow = follow_set(a, cfg, &mut follow_t);
        set.extend(follow);
    }

    set
}

// FROM LGA
fn make_llt(cfg: &CFG) -> Result<HashMap<(String, String), Vec<String>>, ()> {
    let mut llt: HashMap<(String, String), Vec<String>> = HashMap::new();

    for (nonterm, prod_set) in &cfg.prods {
        for prod in prod_set {
            let ps = predict_set(nonterm, prod, cfg);

            for symbol in ps {
                let key = (nonterm.clone(), symbol);
                if llt.contains_key(&key) {
                    return Err(()); // not pairwise disjoint
                }
                llt.insert(key, prod.clone());
            }
        }
    }

    Ok(llt)
}

#[derive(Debug, Clone)]
struct ParseTreeNode {
    symbol: String,
    children: Vec<ParseTreeNode>,
}

fn ll1_parse_recursive(
    cfg: &CFG,
    llt: &HashMap<(String, String), Vec<String>>,
    input: &[String],
    pos: &mut usize,
    symbol: &str,
) -> Result<ParseTreeNode, ()> {
    // Terminal (but not $)
    if cfg.terms.contains(symbol) && symbol != "$" {
        let current = if *pos < input.len() {
            &input[*pos]
        } else {
            return Err(());
        };
        if current == symbol {
            *pos += 1;
            return Ok(ParseTreeNode {
                symbol: symbol.to_string(),
                children: Vec::new(),
            });
        } else {
            return Err(());
        }
    }

    // $ end-of-input marker
    if symbol == "$" {
        if *pos >= input.len() {
            return Ok(ParseTreeNode {
                symbol: "$".to_string(),
                children: Vec::new(),
            });
        } else {
            return Err(());
        }
    }

    // Nonterminal: look up in LLT
    let lookahead = if *pos < input.len() {
        input[*pos].clone()
    } else {
        "$".to_string()
    };

    let prod = match llt.get(&(symbol.to_string(), lookahead)) {
        Some(p) => p.clone(),
        None => return Err(()),
    };

    let mut node = ParseTreeNode {
        symbol: symbol.to_string(),
        children: Vec::new(),
    };

    // Lambda production (stored as vec![""])
    if is_lambda_prod(&prod) {
        node.children.push(ParseTreeNode {
            symbol: "lambda".to_string(),
            children: Vec::new(),
        });
        return Ok(node);
    }

    // Recurse on each RHS symbol
    for rhs_sym in &prod {
        let child = ll1_parse_recursive(cfg, llt, input, pos, rhs_sym)?;
        node.children.push(child);
    }

    Ok(node)
}

fn collect_at_depth(
    node: &ParseTreeNode,
    current_depth: usize,
    target_depth: usize,
    result: &mut Vec<usize>,
) {
    if current_depth == target_depth {
        result.push(node.children.len());
        return;
    }
    for child in &node.children {
        collect_at_depth(child, current_depth + 1, target_depth, result);
    }
}

fn run_check_mode(cfg_path: &str, input_path: &str, count_depth: usize) {
    let cfg = match read_cfg(Path::new(cfg_path)) {
        Ok(c) => c,
        Err(_) => process::exit(1),
    };

    if cfg.start.is_empty() {
        process::exit(1);
    }

    let llt = match make_llt(&cfg) {
        Ok(table) => table,
        Err(_) => process::exit(5),
    };

    let input_content = match read_to_string(input_path) {
        Ok(s) => s,
        Err(_) => process::exit(1),
    };

    let input_tokens: Vec<String> = input_content
        .split_whitespace()
        .map(|s| s.to_string())
        .collect();

    // Parse using LL(1) table and build concrete syntax tree
    let mut pos: usize = 0;
    let tree = match ll1_parse_recursive(&cfg, &llt, &input_tokens, &mut pos, &cfg.start) {
        Ok(t) => t,
        Err(_) => process::exit(2), // syntax error
    };

    // All input must be consumed
    if pos != input_tokens.len() {
        process::exit(2);
    }

    // COUNTDEPTH: report number of children for each node at the target depth
    let mut result = Vec::new();
    collect_at_depth(&tree, 0, count_depth, &mut result);

    if result.is_empty() {
        println!("OUTPUT :NONE:");
    } else {
        let parts: Vec<String> = result.iter().map(|c| c.to_string()).collect();
        println!("OUTPUT {}", parts.join(" "));
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        process::exit(1);
    }

    let mode = args[1].trim();

    match mode {
        "REC" => {
            if args.len() < 5 {
                process::exit(1);
            }
            let _cfg_file = &args[2]; // llre.cfg
            let scan_lut_path = &args[3];
            let scan_u_path = &args[4];

            match read_to_string(scan_lut_path) {
                Ok(s) => {
                    if s.is_empty() {
                        process::exit(1);
                    }
                }
                Err(_) => process::exit(1),
            }
            if Path::new(scan_u_path).exists() {
                fs::remove_file(scan_u_path).unwrap();
            }

            run_rec_mode(scan_lut_path, scan_u_path);
        }

        "CHECK" => {
            if args.len() < 5 {
                process::exit(1);
            }
            let cfg_path = &args[2];
            let input_path = &args[3];
            let count_depth: usize = match args[4].trim().parse() {
                Ok(n) => n,
                Err(_) => process::exit(1),
            };

            match read_to_string(cfg_path) {
                Ok(s) => {
                    if s.is_empty() {
                        process::exit(1);
                    }
                }
                Err(_) => process::exit(1),
            }

            match read_to_string(input_path) {
                Ok(_) => {}
                Err(_) => process::exit(1),
            }

            run_check_mode(cfg_path, input_path, count_depth);
        }

        _ => {
            process::exit(1);
        }
    }
}
