use std::env;
use std::fs::File;
use std::io::{self, Read};

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
    // TODO: Decoding logic will be implemented here
}

fn encode(file_path: &str) {
    let mut file = File::open(file_path).expect("Failed to open file");
    let mut buffer = Vec::new();
    file.read_to_end(&mut buffer).expect("Failed to read file");

    print!(":");
    for byte in buffer {
        if byte.is_ascii_alphanumeric() {
            print!("{}", byte as char);
        } else {
            print!("x{:02X}", byte);
        }
    }
    print!(":\n");
}