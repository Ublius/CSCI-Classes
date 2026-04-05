use std::{
    collections::{HashMap, HashSet},
    env,
    fs::{self, read_to_string, File},
    io::{self, BufRead, BufWriter, Write},
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

fn alphabet_encode(s: &str) -> String {
    let mut encoded = String::new();
    for c in s.chars() {
        if !c.is_ascii_graphic() || c.is_ascii_whitespace() {
            encoded += &format!("x{:02x}", c as u32);
        } else {
            match c {
                ':' => encoded += "x3a",
                '\\' => encoded += "x5c",
                'x' => encoded += "x78",
                _ => encoded.push(c),
            }
        }
    }
    encoded
}

fn alphabet_encode_char(c: char) -> String {
    alphabet_encode(&c.to_string())
}

// ***** REC MODE (Tristen and Wally) ***** //
fn run_rec_mode(scan_lut_path: &str, scan_u_path: &str) {
    let content = read_to_string(scan_lut_path).unwrap();
    let mut lines = content.lines();

    let alphabet_line = lines.next().unwrap();
    let decoded_alphabet = alphabet_decode(alphabet_line);
    let alphabet_set: HashSet<char> = decoded_alphabet.chars().collect();

    let lambda = (1..=255).map(|n| char::from_u32(n).unwrap()).find(|c| !alphabet_set.contains(c)).unwrap();

    let mut writer = BufWriter::new(File::create(scan_u_path).unwrap());
    writeln!(writer, "{}", alphabet_encode(&decoded_alphabet)).unwrap();

    for line in lines {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() < 2 {
            process::exit(1);
        }

        let regex = parts[0];
        let tokenid = parts[1];

        for c in regex.chars() {
            if !alphabet_set.contains(&c) && !"|*+().-".contains(c) {
                process::exit(4);
            }
        }

        let ast = match parse_regex_ll1(regex, &alphabet_set) {
            Ok(a) => a,
            Err(_) => process::exit(2),
        };

        if let Err(_) = check_ast_semantics(&ast) {
            process::exit(3);
        }

        let nfa = ast_to_nfa(&ast, lambda, &alphabet_set);

        let nfa_filename = format!("{}.nfa", tokenid);
        write_nfa_file(&nfa_filename, &nfa, lambda, &alphabet_set);

        write!(writer, "{}.tt {}", tokenid, tokenid).unwrap();
        for extra in &parts[2..] {
            write!(writer, " {}", extra).unwrap();
        }
        writeln!(writer).unwrap();
    }
}

fn build_regex_cfg(alphabet_set: &HashSet<char>) -> CFG {
    let mut prods: HashMap<String, HashSet<Vec<String>>> = HashMap::new();
    let mut nonterms: HashMap<String, bool> = HashMap::new();
    let mut terms: HashSet<String> = HashSet::new();

    // Declare closures as mutable because they mutate the HashMaps
    let mut nt = |s: &str| {
        nonterms.insert(s.to_string(), false);
        s.to_string()
    };
    let mut t = |s: &str| {
        terms.insert(s.to_string());
        s.to_string()
    };

    let regex = nt("regex");
    let regexp = nt("regex'");
    let term = nt("term");
    let termp = nt("term'");
    let factor = nt("factor");
    let base = nt("base");

    t("|"); t("*"); t("+"); t("("); t(")");

    for c in alphabet_set {
        t(&c.to_string());
    }

    // Add productions using `.into_iter().collect()` for HashSet
    prods.insert(
        regex.clone(),
        vec![vec![term.clone(), regexp.clone()]].into_iter().collect(),
    );

    prods.insert(
        regexp.clone(),
        vec![
            vec!["|".to_string(), term.clone(), regexp.clone()],
            vec!["".to_string()],
        ]
        .into_iter()
        .collect(),
    );

    prods.insert(
        term.clone(),
        vec![vec![factor.clone(), termp.clone()]].into_iter().collect(),
    );

    prods.insert(
        termp.clone(),
        vec![
            vec![factor.clone(), termp.clone()],
            vec!["".to_string()],
        ]
        .into_iter()
        .collect(),
    );

    prods.insert(
        factor.clone(),
        vec![
            vec![base.clone(), "*".to_string()],
            vec![base.clone(), "+".to_string()],
            vec![base.clone()],
        ]
        .into_iter()
        .collect(),
    );

    // Base productions: parentheses and characters from alphabet
    let mut base_prods: HashSet<Vec<String>> = HashSet::new();
    base_prods.insert(vec!["(".to_string(), regex.clone(), ")".to_string()]);
    for c in alphabet_set {
        base_prods.insert(vec![c.to_string()]);
    }
    prods.insert(base.clone(), base_prods);

    // Return the CFG
    CFG {
        start: regex.clone(),
        nonterms,
        terms,
        prods,
    }
}

fn parse_regex_ll1(regex: &str, alphabet_set: &HashSet<char>) -> Result<AST, ()> {
    // 1. Build a CFG for regex parsing
    // For simplicity, define regex grammar directly here
    // regex -> term regex'
    // regex' -> '|' term regex' | lambda
    // term -> factor term'
    // term' -> factor term' | lambda
    // factor -> base '*' | base '+' | base
    // base -> '(' regex ')' | char
    let cfg = build_regex_cfg(alphabet_set);
    let llt = make_llt(&cfg).map_err(|_| ())?;

    // 2. Tokenize the regex string
    let tokens: Vec<String> = regex.chars().map(|c| c.to_string()).collect();
    let pos = 0;

    // 3. Recursive function to parse a nonterminal
    fn parse_nonterminal(
        nonterm: &str,
        tokens: &[String],
        pos: &mut usize,
        cfg: &CFG,
        llt: &HashMap<(String, String), Vec<String>>,
    ) -> Result<AST, ()> {
        if *pos >= tokens.len() && nonterm == "regex'" {
            return Ok(AST::Lambda);
        }

        let lookahead = if *pos < tokens.len() {
            tokens[*pos].clone()
        } else {
            "$".to_string()
        };

        let prod = llt.get(&(nonterm.to_string(), lookahead.clone())).ok_or(())?;
        let mut ast_nodes = Vec::new();

        for sym in prod {
            if cfg.terms.contains(sym) {
                // Terminal → AST node
                if sym == "|" {
                    ast_nodes.push(AST::Char('|'));
                } else if sym == "*" {
                    let last = ast_nodes.pop().ok_or(())?;
                    ast_nodes.push(AST::Star(Box::new(last)));
                } else if sym == "+" {
                    let last = ast_nodes.pop().ok_or(())?;
                    ast_nodes.push(AST::Plus(Box::new(last)));
                } else if sym == "(" || sym == ")" {
                    // parentheses are handled by recursion, skip
                } else {
                    // Alphabet char
                    ast_nodes.push(AST::Char(sym.chars().next().unwrap()));
                }
                *pos += 1;
            } else {
                // Nonterminal → recurse
                let node = parse_nonterminal(sym, tokens, pos, cfg, llt)?;
                if let AST::Lambda = node {
                    // ignore
                } else {
                    ast_nodes.push(node);
                }
            }
        }

        // Combine nodes in sequence if more than 1
        let seq_ast = ast_nodes
            .into_iter()
            .reduce(|a, b| AST::Seq(Box::new(a), Box::new(b)))
            .unwrap_or(AST::Lambda);
        Ok(seq_ast)
    }

    // 4. Parse starting from "regex"
    let mut pos = 0;
    let ast = parse_nonterminal("regex", &tokens, &mut pos, &cfg, &llt)?;
    Ok(ast)
}

fn check_ast_semantics(ast: &AST) -> Result<(), ()> {
    match ast {
        AST::Range(a, b) => {
            if a > b {
                return Err(());
            }
        }
        AST::Seq(l, r) | AST::Alt(l, r) => {
            check_ast_semantics(l)?;
            check_ast_semantics(r)?;
        }
        AST::Star(x) | AST::Plus(x) => check_ast_semantics(x)?,
        _ => {}
    }
    Ok(())
}

fn ast_to_nfa(ast: &AST, lambda: char, alphabet_set: &HashSet<char>) -> NFA {
    // Simple placeholder: in practice use Thompson's construction
    // Here we just create a 2-state NFA for single char sequences
    match ast {
        AST::Char(c) => NFA {
            states: 2,
            transitions: vec![(0, 1, *c)],
            lambda_transitions: vec![],
            start: 0,
            accept: 1,
        },
        AST::Seq(l, r) => {
            let left = ast_to_nfa(l, lambda, alphabet_set);
            let right = ast_to_nfa(r, lambda, alphabet_set);
            let mut transitions = left.transitions.clone();
            transitions.extend(right.transitions.iter().map(|(f, t, s)| (f + left.states - 1, t + left.states - 1, *s)));
            let mut lambda_transitions = left.lambda_transitions.clone();
            lambda_transitions.extend(right.lambda_transitions.iter().map(|(f, t)| (f + left.states - 1, t + left.states - 1)));
            // Connect left accept → right start via lambda
            lambda_transitions.push((left.accept, left.states));
            NFA {
                states: left.states + right.states,
                transitions,
                lambda_transitions,
                start: left.start,
                accept: left.states + right.accept,
            }
        }
        _ => NFA {
            states: 1,
            transitions: vec![],
            lambda_transitions: vec![],
            start: 0,
            accept: 0,
        },
    }
}

/// Write an NFA to a file in a cross-platform way
fn write_nfa_file(filename: &str, nfa: &NFA, lambda: char, alphabet_set: &HashSet<char>) {
    let path = Path::new(filename);
    let file = File::create(&path).expect("Failed to create NFA file");
    let mut writer = BufWriter::new(file);

    // Write the header: number of states + alphabet (lambda + all chars)
    write!(writer, "{} {}", nfa.states, alphabet_encode_char(lambda)).unwrap();
    for c in alphabet_set {
        write!(writer, " {}", alphabet_encode_char(*c)).unwrap();
    }
    writeln!(writer).unwrap();

    // Write normal transitions
    for (from, to, sym) in &nfa.transitions {
        writeln!(writer, "- {} {} {}", from, to, alphabet_encode_char(*sym)).unwrap();
    }

    // Write lambda transitions
    for (from, to) in &nfa.lambda_transitions {
        writeln!(writer, "- {} {} {}", from, to, alphabet_encode_char(lambda)).unwrap();
    }

    // Write accept state
    writeln!(writer, "+ {} {}", nfa.accept, nfa.accept).unwrap();

    // --- CROSS-PLATFORM FILE PERMISSIONS ---
    // #[cfg(unix)]
    // {
    //     use std::os::unix::fs::PermissionsExt;
    //     let perms = std::fs::Permissions::from_mode(0o555); // r-xr-xr-x
    //     std::fs::set_permissions(path, perms).unwrap();
    // }

    #[cfg(windows)]
    {
        // Windows doesn’t have execute bits, so we only ensure file is writable
        // You can make it read-only if desired:
        // let perms = std::fs::Permissions::from_readonly(true);
        // std::fs::set_permissions(path, perms).unwrap();
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
    let reader = io::BufReader::new(file);

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
            match File::create(scan_u_path) {
                Ok(_) => {
                    let _ = fs::remove_file(scan_u_path);
                }
                Err(_) => process::exit(1),
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
