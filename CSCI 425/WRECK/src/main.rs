use std::{
    collections::{HashMap, HashSet},
    env,
    fs::{self, read_to_string, File},
    io::{BufReader, BufWriter, Write, BufRead},
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
    states: usize,
    transitions: Vec<(usize, usize, char)>,
    lambda_transitions: Vec<(usize, usize)>,
    start: usize,
    accept: usize,
}

// ---------- ALPHABET ENCODING ---------- //
fn alphabet_decode(s: &str) -> String {
    let mut decoded = String::new();
    let chars: Vec<char> = s.chars().collect();
    let mut i = 0;
    while i < chars.len() {
        if chars[i] == ' ' { i += 1; continue; }
        if chars[i] == 'x' && i + 2 < chars.len() {
            if let Ok(n) = u32::from_str_radix(&format!("{}{}", chars[i+1], chars[i+2]), 16) {
                if let Some(ch) = char::from_u32(n) {
                    decoded.push(ch);
                    i += 3;
                    continue;
                }
            }
        }
        decoded.push(chars[i]);
        i += 1;
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

fn alphabet_encode(s: &str) -> String {
    s.chars().map(alphabet_encode_char).collect::<Vec<_>>().join("")
}

// ---------- REC MODE ---------- //
pub fn run_rec_mode(scan_lut_path: &str, scan_u_path: &str) {
    let content = read_to_string(scan_lut_path).unwrap_or_else(|_| {
        eprintln!("Failed to read {}", scan_lut_path);
        process::exit(1);
    });

    let mut lines = content.lines();
    let alphabet_line = lines.next().unwrap();
    let decoded_alphabet = alphabet_decode(alphabet_line);
    let alphabet_set: HashSet<char> = decoded_alphabet.chars().collect();
    
    // Choose lambda character not in alphabet
    let lambda = (1..=255)
        .map(|n| char::from_u32(n).unwrap())
        .find(|c| !alphabet_set.contains(c))
        .unwrap_or_else(|| {
            eprintln!("Failed to find lambda character not in alphabet");
            process::exit(1);
        });

    let mut writer = BufWriter::new(File::create(scan_u_path).unwrap_or_else(|_| {
        eprintln!("Failed to create {}", scan_u_path);
        process::exit(1);
    }));
    
    // Write alphabet line with spaces between characters
    let alphabet_encoded: Vec<String> = decoded_alphabet.chars()
        .map(alphabet_encode_char)
        .collect();
    writeln!(writer, "{}", alphabet_encoded.join(" ")).unwrap();

    for line in lines {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() < 2 { 
            continue;
        }
        
        let regex = parts[0];
        let tokenid = parts[1];

        // Parse regex into AST
        let ast = parse_regex_full(regex, &alphabet_set);
        
        // Convert AST to NFA
        let nfa = ast_to_nfa(&ast, lambda, &alphabet_set);

        // Write NFA file
        let nfa_filename = format!("{}.nfa", tokenid);
        write_nfa_file(&nfa_filename, &nfa, lambda, &alphabet_set);

        // Write to scan.u
        write!(writer, "{}.tt {}", tokenid, tokenid).unwrap();
        for extra in &parts[2..] {
            write!(writer, " {}", extra).unwrap();
        }
        writeln!(writer).unwrap();
    }
}

fn parse_regex_full(regex: &str, alphabet_set: &HashSet<char>) -> AST {
    let chars: Vec<char> = regex.chars().collect();
    let mut pos = 0;
    parse_expr(&chars, &mut pos, alphabet_set)
}

fn parse_expr(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> AST {
    let mut node = parse_concat(chars, pos, alphabet_set);
    while *pos < chars.len() && chars[*pos] == '|' {
        *pos += 1;
        let right = parse_concat(chars, pos, alphabet_set);
        node = AST::Alt(Box::new(node), Box::new(right));
    }
    node
}

fn parse_concat(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> AST {
    let mut nodes = vec![];
    while *pos < chars.len() && chars[*pos] != '|' && chars[*pos] != ')' {
        nodes.push(parse_factor(chars, pos, alphabet_set));
    }
    nodes.into_iter()
        .reduce(|a,b| AST::Seq(Box::new(a), Box::new(b)))
        .unwrap_or(AST::Lambda)
}

fn parse_factor(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> AST {
    let mut node = parse_base(chars, pos, alphabet_set);
    while *pos < chars.len() && (chars[*pos] == '*' || chars[*pos] == '+') {
        node = match chars[*pos] {
            '*' => AST::Star(Box::new(node)),
            '+' => AST::Plus(Box::new(node)),
            _ => node,
        };
        *pos += 1;
    }
    node
}

fn parse_base(chars: &[char], pos: &mut usize, alphabet_set: &HashSet<char>) -> AST {
    if *pos >= chars.len() { 
        return AST::Lambda; 
    }

    let c = chars[*pos];

    if c == '(' {
        *pos += 1;
        let node = parse_expr(chars, pos, alphabet_set);
        if *pos >= chars.len() || chars[*pos] != ')' { 
            process::exit(2); 
        }
        *pos += 1;
        node
    } else if *pos + 2 < chars.len() && chars[*pos + 1] == '-' {
        let start = c;
        let end = chars[*pos + 2];
        
        // Check lexical error: endpoints must be in alphabet
        if !alphabet_set.contains(&start) || !alphabet_set.contains(&end) {
            process::exit(4);
        }
        
        *pos += 3;
        
        // Check semantic error: inverted range
        if start > end {
            process::exit(3);
        }
        
        expand_range(start, end)
    } else if c == '.' {
        *pos += 1;
        AST::Dot
    } else {
        // Check lexical error: character must be in alphabet
        if !alphabet_set.contains(&c) && !"|*+().".contains(c) {
            process::exit(4);
        }
        *pos += 1;
        AST::Char(c)
    }
}

fn check_ast_semantics(ast: &AST) -> Result<(), ()> {
    match ast {
        AST::Range(a,b) => { if a > b { return Err(()); } }
        AST::Seq(l,r) | AST::Alt(l,r) => { check_ast_semantics(l)?; check_ast_semantics(r)?; }
        AST::Star(x) | AST::Plus(x) => check_ast_semantics(x)?,
        _ => {}
    }
    Ok(())
}

fn expand_range(a: char, b: char) -> AST {
    let mut nodes = Vec::new();
    for c in a..=b { 
        nodes.push(AST::Char(c)); 
    }
    nodes.into_iter()
        .reduce(|x, y| AST::Alt(Box::new(x), Box::new(y)))
        .unwrap()
}

fn ast_to_nfa(ast: &AST, lambda: char, alphabet_set: &HashSet<char>) -> NFA {
    match ast {
        AST::Char(c) => {
            // Simple transition from state 0 to 1 on character c
            NFA {
                states: 2,
                transitions: vec![(0, 1, *c)],
                lambda_transitions: vec![],
                start: 0,
                accept: 1,
            }
        }

        AST::Dot => {
            // Transition from state 0 to 1 on ANY character in alphabet
            let mut transitions = Vec::new();
            for c in alphabet_set {
                transitions.push((0, 1, *c));
            }
            NFA {
                states: 2,
                transitions,
                lambda_transitions: vec![],
                start: 0,
                accept: 1,
            }
        }

        AST::Lambda => {
            // Single state that is both start and accept
            NFA {
                states: 1,
                transitions: vec![],
                lambda_transitions: vec![],
                start: 0,
                accept: 0,
            }
        }

        AST::Alt(l, r) => {
            let ln = ast_to_nfa(l, lambda, alphabet_set);
            let rn = ast_to_nfa(r, lambda, alphabet_set);
            
            let new_start = 0;
            let new_accept = ln.states + rn.states + 1;
            
            // Copy left NFA states (offset by 1)
            let mut transitions = Vec::new();
            for (f, t, c) in &ln.transitions {
                transitions.push((f + 1, t + 1, *c));
            }
            
            // Copy right NFA states (offset by ln.states + 1)
            let right_offset = ln.states + 1;
            for (f, t, c) in &rn.transitions {
                transitions.push((f + right_offset, t + right_offset, *c));
            }
            
            // Copy lambda transitions
            let mut lambda_transitions = Vec::new();
            for (f, t) in &ln.lambda_transitions {
                lambda_transitions.push((f + 1, t + 1));
            }
            for (f, t) in &rn.lambda_transitions {
                lambda_transitions.push((f + right_offset, t + right_offset));
            }
            
            // Add new lambda transitions from new start
            lambda_transitions.push((new_start, ln.start + 1));
            lambda_transitions.push((new_start, rn.start + right_offset));
            
            // Add lambda transitions to new accept
            lambda_transitions.push((ln.accept + 1, new_accept));
            lambda_transitions.push((rn.accept + right_offset, new_accept));
            
            NFA {
                states: new_accept + 1,
                transitions,
                lambda_transitions,
                start: new_start,
                accept: new_accept,
            }
        }

        AST::Seq(l, r) => {
            let ln = ast_to_nfa(l, lambda, alphabet_set);
            let rn = ast_to_nfa(r, lambda, alphabet_set);
            
            // Copy left NFA
            let mut transitions = ln.transitions.clone();
            let mut lambda_transitions = ln.lambda_transitions.clone();
            
            // Copy right NFA (offset by ln.states)
            for (f, t, c) in &rn.transitions {
                transitions.push((f + ln.states, t + ln.states, *c));
            }
            for (f, t) in &rn.lambda_transitions {
                lambda_transitions.push((f + ln.states, t + ln.states));
            }
            
            // Add lambda from left accept to right start
            lambda_transitions.push((ln.accept, rn.start + ln.states));
            
            NFA {
                states: ln.states + rn.states,
                transitions,
                lambda_transitions,
                start: ln.start,
                accept: rn.accept + ln.states,
            }
        }

        AST::Star(x) => {
            let inner = ast_to_nfa(x, lambda, alphabet_set);
            
            let new_start = 0;
            let new_accept = inner.states + 1;
            
            // Copy inner NFA (offset by 1)
            let mut transitions = Vec::new();
            for (f, t, c) in &inner.transitions {
                transitions.push((f + 1, t + 1, *c));
            }
            
            let mut lambda_transitions = Vec::new();
            for (f, t) in &inner.lambda_transitions {
                lambda_transitions.push((f + 1, t + 1));
            }
            
            // Add lambda transitions for Kleene star
            lambda_transitions.push((new_start, inner.start + 1));  // Start to inner start
            lambda_transitions.push((new_start, new_accept));       // Start to accept (zero repetitions)
            lambda_transitions.push((inner.accept + 1, inner.start + 1)); // Loop back
            lambda_transitions.push((inner.accept + 1, new_accept));     // Exit to accept
            
            NFA {
                states: new_accept + 1,
                transitions,
                lambda_transitions,
                start: new_start,
                accept: new_accept,
            }
        }

        AST::Plus(x) => {
            // Plus = x followed by Star(x)
            let x_nfa = ast_to_nfa(x, lambda, alphabet_set);
            let star_nfa = ast_to_nfa(&AST::Star(Box::new((**x).clone())), lambda, alphabet_set);
            
            // Sequence of x then star(x)
            let mut transitions = x_nfa.transitions.clone();
            let mut lambda_transitions = x_nfa.lambda_transitions.clone();
            
            // Copy star (offset by x_nfa.states)
            for (f, t, c) in &star_nfa.transitions {
                transitions.push((f + x_nfa.states, t + x_nfa.states, *c));
            }
            for (f, t) in &star_nfa.lambda_transitions {
                lambda_transitions.push((f + x_nfa.states, t + x_nfa.states));
            }
            
            // Lambda from x's accept to star's start
            lambda_transitions.push((x_nfa.accept, star_nfa.start + x_nfa.states));
            
            NFA {
                states: x_nfa.states + star_nfa.states,
                transitions,
                lambda_transitions,
                start: x_nfa.start,
                accept: star_nfa.accept + x_nfa.states,
            }
        }

        AST::Range(a, b) => {
            // Range should already be expanded by the parser
            // But handle it just in case
            if a > b {
                process::exit(3);
            }
            let mut nodes = Vec::new();
            for c in *a..=*b {
                nodes.push(AST::Char(c));
            }
            let ast = nodes.into_iter()
                .reduce(|x, y| AST::Alt(Box::new(x), Box::new(y)))
                .unwrap();
            ast_to_nfa(&ast, lambda, alphabet_set)
        }
    }
}

fn write_nfa_file(filename: &str, nfa: &NFA, lambda: char, alphabet_set: &HashSet<char>) {
    let file = File::create(Path::new(filename)).unwrap_or_else(|_| {
        eprintln!("Failed to create NFA file: {}", filename);
        process::exit(1);
    });
    let mut writer = BufWriter::new(file);

    // First line: number of states and lambda character
    write!(writer, "{} {}", nfa.states, alphabet_encode_char(lambda)).unwrap();
    
    // Add all alphabet characters
    let mut alphabet: Vec<char> = alphabet_set.iter()
        .filter(|&&c| c != lambda)
        .cloned()
        .collect();
    alphabet.sort();
    
    for c in &alphabet {
        write!(writer, " {}", alphabet_encode_char(*c)).unwrap();
    }
    writeln!(writer).unwrap();

    // Write all transitions (lambda transitions use the lambda character)
    for (f, t, s) in &nfa.transitions {
        writeln!(writer, "- {} {} {}", f, t, alphabet_encode_char(*s)).unwrap();
    }
    
    for (f, t) in &nfa.lambda_transitions {
        writeln!(writer, "- {} {} {}", f, t, alphabet_encode_char(lambda)).unwrap();
    }
    
    // Write accept state - note: NFAMATCH expects multiple accept states?
    // The spec shows "+ 1 1" format for multiple accepts
    writeln!(writer, "+ {}", nfa.accept).unwrap();
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

fn run_check_mode(cfg_path: &str, input_path: &str, count_depth: usize) {
    // TODO
    let cfg = match read_cfg(Path::new(cfg_path)) {
        Ok(c) => c,
        Err(_) => process::exit(1),
    };

    if cfg.start.is_empty() {
        process::exit(1);
    }

    let llt = match make_llt(&cfg) {
        Ok(table) => table,
        Err(_) => process::exit(1),
    };

    let input_content = match read_to_string(input_path) {
        Ok(s) => s,
        Err(_) => process::exit(1),
    };

    let input_tokens: Vec<String> = input_content
        .split_whitespace()
        .map(|s| s.to_string())
        .collect();

    //TODO: Parse LL(1) table and create tree

    //TODO: Collect leaf nodes at specified depth (count_depth)
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
