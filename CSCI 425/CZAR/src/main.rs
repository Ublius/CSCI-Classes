mod ast;
mod codegen;
mod lr_table;
mod parser;
mod semantic;
mod symtable;
mod token;

use std::env;
use std::process;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() < 5 {
        eprintln!("Usage: {} <R_N, F_N> <program.tok> <program.def> <program.czr>", args[0]);
        eprintln!("  R_N,F_N: Number of general purpose and floating point registers (e.g., 16,16)");
        process::exit(1);
    }

    let reg_counts: Vec<&str> = args[1].split(',').collect();
    if reg_counts.len() != 2 {
        eprintln!("ERROR: Invalid register format. Expected 'R_N,F_N'");
        process::exit(1);
    }
    let rn_regs: usize = reg_counts[0].parse().unwrap_or(16);
    let fn_regs: usize = reg_counts[1].parse().unwrap_or(16);
    
    let token_file = &args[2];
    let def_file = &args[3];
    let output_file = &args[4];

    let tokens = match token::read_token_stream(token_file) {
        Ok(tokens) => {
            if tokens.is_empty() {
                process::exit(1);
            }
            tokens
        }
        Err(err) => {
            process::exit(1);
        }
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

    let mut arena = parse_result.arena;
    let root = parse_result.root;

    let mut analyzer = semantic::SemanticAnalyzer::new(def_file.to_string());
    analyzer.analyze(&mut arena, root);

    if analyzer.has_semantic_errors() {
        eprintln!("ERROR: Semantic analysis failed");
        for msg in analyzer.messages() {
            eprintln!("{}", msg);
        }
        process::exit(3);
    }

    let mut generator = codegen::CodeGenerator::new(rn_regs, fn_regs);
    match generator.generate(&arena, root, &mut analyzer.table, output_file) {
        Ok(_) => {
            eprintln!("SUCCESS: Generated {}", output_file);
            process::exit(0);
        }
        Err(e) => {
            eprintln!("ERROR: Code generation failed: {}", e);
            process::exit(1);
        }
    }
}
