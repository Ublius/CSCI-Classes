mod ast;
mod lr_table;
mod parser;
mod semantic;
mod symtable;
mod token;

use std::env;
use std::process;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 4 {
        eprintln!("Usage: {} <program.tok> <ast.dat> <symbol.dat>", args[0]);
        process::exit(1);
    }

    let token_file = &args[1];
    let ast_file = &args[2];
    let symbol_file = &args[3];

    let tokens = match token::read_token_stream(token_file) {
        Ok(tokens) => tokens,
        Err(_) => process::exit(1),
    };

    let lr_data = match lr_table::load_lr_data() {
        Ok(data) => data,
        Err(e) => {
            eprintln!("Error loading LR data: {}", e);
            process::exit(1);
        }
    };

    let parse_result = match parser::parse(&tokens, &lr_data) {
        Ok(result) => result,
        Err(e) => {
            println!("OUTPUT :SYNTAX: {} {} :SYNTAX:", e.line, e.col);
            if !e.detail.is_empty() {
                eprintln!("REASON {}", e.detail);
            }
            process::exit(2);
        }
    };

    let parser::ParseResult { root, mut arena } = parse_result;

    if let Err(_) = ast::write_ast_file(&arena, root, ast_file) {
        process::exit(1);
    }

    let mut analyzer = semantic::SemanticAnalyzer::new(symbol_file.clone());
    analyzer.analyze(&mut arena, root);

    if analyzer.io_failed {
        process::exit(1);
    }

    for msg in analyzer.messages() {
        println!("OUTPUT {}", msg);
    }

    if analyzer.has_semantic_errors() {
        process::exit(3);
    }

    process::exit(0);
}
