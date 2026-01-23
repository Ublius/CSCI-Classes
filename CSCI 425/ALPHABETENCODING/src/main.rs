use std::env;
use std::fs::File;
use std::io::{self, Read, Write};

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 3 {
        eprintln!("Usage:");
        eprintln!("  {} encode <input file path>", args[0]);
        eprintln!("  {} decode <single token> <output file path>", args[0]);
        std::process::exit(1);
    }

    match args[1].as_str() {
        "encode" => {
            if args.len() != 3 {
                eprintln!("encode expects exactly 1 argument");
                std::process::exit(1);
            }

            let file_path = &args[2];
            encode(file_path);
        }

        "decode" => {
            if args.len() != 4 {
                eprintln!("decode expects exactly 2 arguments");
                std::process::exit(1);
            }

            let token = &args[2];
            let file_path = &args[3];
            decode(token, file_path);
        }

        _ => {
            eprintln!("Unknown command: {}", args[1]);
            std::process::exit(1);
        }
    }
}

fn decode(token: &str, file_path: &str) {
    let mut file = File::create(file_path).expect("Failed to create file");
    let mut it = token.chars();

    while let Some(char) = it.next() {
        if char == 'x' || char == 'X' {
            let hex_value = format!(
                "{}{}",
                it.next().expect("Expected hex digit after 'x'"),
                it.next().expect("Expected hex digit after 'x'")
            );
            let byte = u8::from_str_radix(&hex_value, 16).expect("Failed to parse hex value");
            file.write_all(&[byte]).expect("Failed to write to file");
        } else {
            let byte = char as u8;
            file.write_all(&[byte]).expect("Failed to write to file");
        }
    }
}

fn encode(file_path: &str) {
    let mut file = File::open(file_path).expect("Failed to open file");
    let mut buffer = Vec::new();
    file.read_to_end(&mut buffer).expect("Failed to read file");

    if buffer.is_empty() {
        eprintln!("Input file is empty");
        std::process::exit(1);
    }

    print!("OUTPUT :");
    for byte in buffer {
        if byte.is_ascii_alphanumeric() {
            print!("{}", byte as char);
        } else {
            print!("x{:02X}", byte);
        }
    }
    print!(":\n");
}