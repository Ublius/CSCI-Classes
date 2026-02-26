use std::collections::{HashMap, HashSet};
use std::env;
use std::fs;


pub struct Grammar {
    nonterminals: Vec<String>,
    terminals: Vec<String>,
    productions: HashMap<String, Vec<Vec<String>>>,
    start: String,
}

impl Grammar {
    pub fn new() -> Grammar {
        Grammar { 
            nonterminals: Vec::new(),
            terminals: Vec::new(), 
            productions: HashMap::new(),
            start: "".to_string(),
        }
    }

    pub fn is_current_nonterminal(&self, value: &String) -> bool {
        self.nonterminals.contains(value)
    }

    pub fn is_current_terminal(&self, value: &String) -> bool {
        self.terminals.contains(value)
    }

    pub fn print(&self) {
        println!("Nonterminals:");
        for nt in &self.nonterminals {
            println!("{}", nt);
        }
        println!("Terminals:");
        for t in &self.terminals {
            println!("{}", t);
        }
        println!("Start: {}", self.start);
        println!("Productions:");
        for nt in &self.nonterminals {
            let ps: &Vec<Vec<String>> = match self.productions.get(nt) {
                Some(v) => v,
                None => {
                    eprintln!("Insert better error here");
                    std::process::exit(1);
                }
            };
            for p in ps {
                let mut string: String = p[0].clone();
                for i in 1..p.len() {
                    string.push(' ');
                    string.push_str(&p[i]);
                }
                println!("{} -> {}", nt, string);
            }
        }
    }

    pub fn from_string(contents: &str) -> Grammar {
        let split_contents: Vec<&str> = contents.split("->").collect();
        let mut current_nonterminal = split_contents[0].trim().to_string();
        let mut grammar = Grammar::new();
        grammar.nonterminals.push(current_nonterminal.clone());
        grammar.productions.insert(current_nonterminal.clone(), Vec::new());

        for i in 1..split_contents.len() {
            let mut next_nonterminal = String::new();
            let current_prods: Vec<&str> = split_contents[i].split("|").collect();

            for j in 0..current_prods.len() {
                let p = current_prods[j];
                let mut collected: Vec<&str> = p.split_whitespace().collect();
                // If we are on the last production for a nonterminal, the very last character will be the next nonterminal
                if j == current_prods.len() - 1 && i < split_contents.len() - 1 {
                    if let Some(l) = collected.pop() {
                        next_nonterminal = l.to_string();
                    }
                }
                for c in &collected {
                    if *c == "$" { // Maybe add error check if multiple nonterminals see $
                        grammar.start = current_nonterminal.clone();
                    }
                    let cstring = c.to_string();

                    // check if equal to lowercase to see if it contains a capital letter (and is therefore a nonterminal)
                    if !grammar.is_current_terminal(&cstring) && cstring == cstring.to_lowercase() && cstring != "lambda" {
                        grammar.terminals.push(cstring.clone());
                    }
                }
                if let Some(ps) = grammar.productions.get_mut(&current_nonterminal) {
                    ps.push(collected.into_iter().map(|s| s.to_string()).collect());
                }
            }
            if i < split_contents.len() - 1 {
                current_nonterminal = next_nonterminal;
                if !grammar.is_current_nonterminal(&current_nonterminal) {
                    grammar.nonterminals.push(current_nonterminal.clone());
                    grammar.productions.insert(current_nonterminal.clone(), Vec::new());
                }
            }
        }

        grammar
    }

    pub fn from_file(filename: &str) -> Result<Grammar, std::io::Error> {
        let contents = fs::read_to_string(filename)?;
        Ok(Grammar::from_string(&contents))
    }


    pub fn derives_to_lambda(&self, nonterminal: &String) -> bool {
        if let Some(prods) = self.productions.get(nonterminal) {
            for rhs in prods {
                if rhs.len() == 1 && rhs[0] == "lambda" {
                    return true;
                }
                let mut all_lambda = true;
                for symbol in rhs {
                    if self.terminals.contains(symbol) || symbol == "$" {
                        all_lambda = false;
                        break;
                    }
                    if !self.derives_to_lambda(symbol) {
                        all_lambda = false;
                        break;
                    }
                }
                if all_lambda {
                    return true;
                }
            }
        }
        false
    }

    pub fn first_set(&self, sequence: &Vec<String>, visited: &mut HashSet<String>) -> (HashSet<String>, HashSet<String>) {
        let mut first = HashSet::new();

        if sequence.is_empty() {
            return (first, visited.clone());
        }

        let x = &sequence[0];
        let beta = &sequence[1..];

        if self.terminals.contains(x) || x == "$" {
            first.insert(x.clone());
            return (first, visited.clone());
        }

        if !visited.contains(x) {
            visited.insert(x.clone());

            if let Some(prods) = self.productions.get(x) {
                for rhs in prods {
                    let (g, _) = self.first_set(rhs, visited);
                    first.extend(g);
                }
            }
        }

        if self.derives_to_lambda(x) && !beta.is_empty() {
            let beta_vec = beta.to_vec();
            let (g, _) = self.first_set(&beta_vec, visited);
            first.extend(g);
        }

        (first, visited.clone())
    }
}


fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 2 {
        eprintln!("Usage: {} <grammar file>", args[0]);
        std::process::exit(1);
    }

    let grammar_filename = &args[1];

    let grammar = match Grammar::from_file(grammar_filename) {
        Ok(g) => g,
        Err(e) => {
            eprintln!("Cannot open file {}: {}", grammar_filename, e);
            std::process::exit(1);
        }
    };
    // grammar.print();

    // Dervies to lambda test
    for nt in &grammar.nonterminals {
        println!("{} derives to lambda: {}", nt, grammar.derives_to_lambda(nt));
    }

    // First Set Test
    // for nt in &grammar.nonterminals {
    //     let mut visited = HashSet::new();
    //     let (first, _) = grammar.first_set(&vec![nt.clone()], &mut visited);
    //     println!("First set of {}: {:?}", nt, first);
    // }
}