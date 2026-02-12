use std::env;
use std::fs::File;
use std::io::{self, BufRead, BufReader, Write};
use std::process;
use std::collections::HashMap;

type StateId = usize;

struct Alphabet {
    chars: Vec<u8>,
    index_map: HashMap<u8, usize>,
}

struct DFA {
    token_id: String,
    constant_value: Option<Vec<u8>>,
    transitions: Vec<Vec<Option<StateId>>>,
    accepting: Vec<bool>,
}

struct ScannerDef {
    alphabet: Alphabet,
    dfas: Vec<DFA>,
}

struct Token {
    token_id: String,
    value: Vec<u8>,
    line: usize,
    column: usize,
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 4 {
        eprintln!("Usage: LUTHOR1 <scan_def> <source> <output>");
        process::exit(1);
    }

    let def = load_scanner_definition(&args[1]).unwrap_or_else(|e| {
        eprintln!("Error: {}", e);
        process::exit(1);
    });

    let source = std::fs::read(&args[2]).unwrap_or_else(|e| {
        eprintln!("Error reading source: {}", e);
        process::exit(1);
    });

    let tokens = scan_source(&source, &def);

    let mut out = File::create(&args[3]).unwrap_or_else(|e| {
        eprintln!("Error creating output: {}", e);
        process::exit(1);
    });

    for t in tokens {
        let encoded: String = t.value.iter().map(|&b| encode_char(b)).collect();
        writeln!(out, "{} {} {} {}", t.token_id, encoded, t.line, t.column).unwrap();
    }
}

// ============ Helper Functions ============
fn parse_alphabet(line: &str) -> Alphabet {
    let mut chars = Vec::new();
    for field in line.split_whitespace() {
        let mut i = 0;
        while i < field.len() {
            if &field[i..i+1] == "x" || &field[i..i+1] == "X" {
                let byte = u8::from_str_radix(&field[i+1..i+3], 16).expect("Failed to parse hex value");
                chars.push(byte);
                i += 3;
            } else {
                chars.push(field.as_bytes()[i]);
                i += 1;
            }
        }
    }
    let index_map = chars.iter().enumerate().map(|(i, &c)| (c, i)).collect();
    Alphabet { chars, index_map }
}

fn decode_value(s: &str) -> Vec<u8> {
    let mut out = Vec::new();
    let mut i = 0;

    while i < s.len() {
        if &s[i..i+1] == "x" || &s[i..i+1] == "X"  && s[i+1..i+3].chars().all(|c| c.is_ascii_hexdigit()) {
            let byte = u8::from_str_radix(&s[i+1..i+3], 16).expect("Failed to parse hex value");
            out.push(byte);
            i += 3;
        } else {
            out.push(s.as_bytes()[i]);
            i += 1;
        }
    }
    out
}

fn encode_char(c: u8) -> String {
    if !(c != b'x' && c != b'\\' && c != b':' && c.is_ascii_graphic() && !c.is_ascii_whitespace()) {
        format!("x{:02x}", c)
    }
    else {
        (c as char).to_string()
    }
}

// ============ Loading Scan Definition ============
fn load_scanner_definition(path: &str) -> io::Result<ScannerDef> {
    let file = File::open(path)?;
    let reader = BufReader::new(file);

    let mut lines: Vec<String> = reader.lines().filter_map(Result::ok).filter(|l| !l.trim().is_empty()).collect();

    if lines.is_empty() {
        return Err(io::Error::new(io::ErrorKind::Other, "Empty definition file"));
    }

    let alphabet = parse_alphabet(&lines[0]);
    let mut dfas = Vec::new();

    for line in lines.drain(1..) {
        let parts: Vec<&str> = line.split_whitespace().collect();
        let table_path = parts[0];
        let token_id = parts[1].to_string();
        let const_val = if parts.len() == 3 {
            Some(decode_value(parts[2]))
        } else {
            None
        };

        let dfa = load_dfa(table_path, token_id, const_val, alphabet.chars.len())?;
        dfas.push(dfa);
    }

    Ok(ScannerDef { alphabet, dfas })
}

// ============ Loading DFA ============
fn load_dfa(path: &str, token_id: String, constant_value: Option<Vec<u8>>, alpha_len: usize) -> io::Result<DFA> {
    let f = File::open(path)?;
    let reader = BufReader::new(f);

    let mut transitions = Vec::new();
    let mut accepting = Vec::new();

    for line in reader.lines() {
        let l = line?;
        let parts: Vec<&str> = l.split_whitespace().collect();
        accepting.push(parts[0] == "+");
        let row: Vec<Option<StateId>> = parts[2..].iter().take(alpha_len).map(|&t| if t == "E" { None } else { Some(t.parse().unwrap()) }).collect();
        transitions.push(row);
    }

    Ok(DFA { token_id, constant_value, transitions, accepting })
}

// ============ Scanner ============
fn scan_source(source: &[u8], def: &ScannerDef) -> Vec<Token> {
    let mut tokens = Vec::new();
    let mut i = 0;
    let mut line = 1;
    let mut col = 1;

    while i < source.len() {
        let start_i = i;
        let start_line = line;
        let start_col = col;

        let mut states: Vec<Option<StateId>> = vec![Some(0); def.dfas.len()];
        let mut best_match: Option<(usize, usize)> = None;
        let mut j = i;

        loop {
            if j >= source.len() {
                break;
            }

            let c = source[j];

            if !def.alphabet.index_map.contains_key(&c) {
                eprintln!("Invalid character at line {}, column {}: {:?}", line, col, c as char);
                process::exit(1);
            }

            let col_idx = match def.alphabet.index_map.get(&c) {
                Some(&idx) => idx,
                None => {
                    for s in states.iter_mut() { *s = None; }
                    break;
                }
            };

            let mut any_alive = false;
            for (idx, dfa) in def.dfas.iter().enumerate() {
                if let Some(st) = states[idx] {
                    if let Some(next) = dfa.transitions[st][col_idx] {
                        states[idx] = Some(next);
                        any_alive = true;
                        if dfa.accepting[next] {
                            let length = j - start_i + 1;
                            best_match = match best_match {
                                None => Some((idx, length)),
                                Some((b_idx, b_len)) => {
                                    if length > b_len || (length == b_len && idx < b_idx) {
                                        Some((idx, length))
                                    } else {
                                        Some((b_idx, b_len))
                                    }
                                }
                            };
                        }
                    } else {
                        states[idx] = None;
                    }
                }
            }

            if !any_alive {
                break;
            }
            j += 1;
        }

        if let Some((dfa_idx, len)) = best_match {
            let dfa = &def.dfas[dfa_idx];
            let lexeme = &source[start_i..start_i + len];
            let val = dfa.constant_value.clone().unwrap_or_else(|| lexeme.to_vec());

            tokens.push(Token {
                token_id: dfa.token_id.clone(),
                value: val,
                line: start_line,
                column: start_col,
            });

            for &b in lexeme {
                if b == b'\n' {
                    line += 1;
                    col = 1;
                } else {
                    col += 1;
                }
            }
            i += len;
        } else {
            if source[i] == b'\n' {
                line += 1;
                col = 1;
            } else {
                col += 1;
            }
            i += 1;
        }
    }

    tokens
}
