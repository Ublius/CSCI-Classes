use std::env;
use std::collections::{HashMap, HashSet};
use std::process;

struct Grammar {
    start_symbol: String,
    non_terminals: HashSet<String>,
    terminals: HashSet<String>,
    productions: HashMap<String, Vec<Vec<String>>>,
    numbered_rules: Vec<(String, Vec<String>)>,
}

impl Grammar {
    fn new() -> Self {
        Grammar {
            start_symbol: String::new(),
            non_terminals: HashSet::new(),
            terminals: HashSet::new(),
            productions: HashMap::new(),
            numbered_rules: Vec::new(),
        }
    }

    fn from_file(filename: &str) -> Self {
        let contents = std::fs::read_to_string(filename).expect("Failed to read grammar file");

        let mut grammar = Grammar::new();
        let mut current_lhs = String::new();

        for line in contents.lines() {
            let line = line.trim();
            if line.is_empty() {continue};

            let tokens: Vec<&str> = line.split_whitespace().collect();
            if tokens.contains(&"->") {
                current_lhs = tokens[0].to_string();
                grammar.non_terminals.insert(current_lhs.clone());
                let rhs = tokens[2..].join(" ");
                grammar.process_rhs(&current_lhs, &rhs);
            } else if tokens[0] == "|" {
                let rhs = tokens[1..].join(" ");
                grammar.process_rhs(&current_lhs, &rhs);
            } else {
                eprintln!("Invalid grammar line: {}", line);
                process::exit(1);
            }
        }

        grammar.identify_terminals();
        grammar.identify_start_symbol();
        grammar
    }

    fn process_rhs(&mut self, lhs: &str, rhs: &str) {
        let altnernatives: Vec<&str> = rhs.split('|').map(|s| s.trim()).collect();
        for alt in altnernatives {
            let symbols: Vec<String> = alt.split_whitespace().map(|s| s.to_string()).collect();
            self.productions.entry(lhs.to_string()).or_insert_with(Vec::new).push(symbols.clone());
            self.numbered_rules.push((lhs.to_string(), symbols));
        }
    }

    fn identify_terminals(&mut self) {
        for rhs_list in self.productions.values() {
            for rhs in rhs_list {
                for symbol in rhs {
                    if !self.non_terminals.contains(symbol) && symbol != "lambda" {
                        self.terminals.insert(symbol.clone());
                    }
                }
            }
        }
    }

    fn identify_start_symbol(&mut self) {
        for (lhs, rhs) in &self.numbered_rules {
            if rhs.contains(&"$".to_string()) {
                self.start_symbol = lhs.clone();
                return;
            }
        }
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("Usage: LUTHOR1 <cfg_file>");
        process::exit(1);
    }

   let grammar = Grammar::from_file(&args[1]);

    println!("\nNon-Terminals:");
    let mut nts: Vec<_> = grammar.non_terminals.iter().collect();
    nts.sort();
    println!("{}", nts.iter().map(|s| s.to_string()).collect::<Vec<_>>().join(" "));

    println!("\nGrammar Symbols:");
    let mut symbols: Vec<_> = grammar.non_terminals.union(&grammar.terminals).collect();
    symbols.sort();
    println!("{}", symbols.iter().map(|s| s.to_string()).collect::<Vec<_>>().join(" "));

    println!("\nGrammar Rules:");
    for (i, (lhs, rhs)) in grammar.numbered_rules.iter().enumerate() {
        println!("({}) {} -> {}", i + 1, lhs, rhs.join(" "));
    }

    println!("\nGrammar Start Symbol or Goal: {}", grammar.start_symbol);
}
