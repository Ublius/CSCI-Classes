use std::fs;
use std::io;
 
#[derive(Debug, Clone)]
pub struct Token {
    pub line:    usize,
    pub col:     usize,
    pub kind:    String,
    pub lexeme:  String,
    pub node_id: Option<usize>,
}
 
impl Token {
    pub fn eof(line: usize, col: usize) -> Self {
        Token { line, col, kind: "$".to_string(), lexeme: "$".to_string(), node_id: None }
    }
}

pub fn read_token_stream(path: &str) -> io::Result<Vec<Token>> {
    let content = fs::read_to_string(path)?;
    let mut tokens = Vec::new();
 
    for raw in content.lines() {
        let s = raw.trim();
        if s.is_empty() || s.starts_with('#') { continue; }
 
        let parts: Vec<&str> = s.splitn(4, |c: char| c.is_ascii_whitespace())
            .filter(|p| !p.is_empty())
            .collect();
 
        if parts.len() < 4 { continue; }

        let kind         = parts[0].to_string();
        let lexeme = if parts.len() >= 2 {
            let raw_lex = parts[1].trim();
            if raw_lex.starts_with('"') && raw_lex.ends_with('"') && raw_lex.len() >= 2 {
                raw_lex[1..raw_lex.len()-1].to_string()
            } else {
                raw_lex.to_string()
            }
        } else {
            kind.clone()
        };
        let line: usize = parts[2].parse().unwrap_or(0);
        let col:  usize = parts[3].parse().unwrap_or(0);
        tokens.push(Token { line, col, kind, lexeme, node_id: None });
    }
    Ok(tokens)
}
