use std::collections::HashMap;
use std::fs;
use std::io;
  
#[derive(Debug, Clone)]
pub struct Rule {
    pub lhs: String,
    pub rhs: Vec<String>,
}
 
impl Rule {
    pub fn rhs_len(&self) -> usize { self.rhs.len() }
}
 
#[derive(Debug, Clone, PartialEq)]
pub enum LRAction {
    Shift(usize),
    Reduce(usize),
    ReduceAccept(usize),
    Error,
}
  
pub struct LRData {
    pub rules: Vec<Rule>,
    pub table: HashMap<(usize, String), LRAction>,
}
 
impl LRData {
    pub fn lookup(&self, state: usize, symbol: &str) -> LRAction {
        self.table
            .get(&(state, symbol.to_string()))
            .cloned()
            .unwrap_or(LRAction::Error)
    }
 
    pub fn expected(&self, state: usize) -> Vec<String> {
        let mut v: Vec<String> = self.table.keys()
            .filter(|(s, _)| *s == state)
            .map(|(_, sym)| sym.clone())
            .collect();
        v.sort();
        v.dedup();
        v
    }
}
 
pub fn load_grammar(path: &str) -> io::Result<Vec<Rule>> {
    let content = fs::read_to_string(path)?;
 
    let mut rules: Vec<Rule> = vec![Rule { lhs: String::new(), rhs: vec![] }];
 
    let mut current_lhs: Option<String> = None;
 
    for raw in content.lines() {
        let line = raw.trim();
 
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
 
        if let Some(arrow) = line.find("->") {
            let lhs = line[..arrow].trim().to_string();
            let rhs_part = line[arrow + 2..].trim();
            current_lhs = Some(lhs.clone());
            parse_alternatives(&lhs, rhs_part, &mut rules);
        } else if line.starts_with('|') {
            if let Some(ref lhs) = current_lhs {
                let rhs_part = line[1..].trim();
                parse_alternatives(lhs, rhs_part, &mut rules);
            }
        }
    }
 
    Ok(rules)
}
 
fn parse_alternatives(lhs: &str, rhs_str: &str, rules: &mut Vec<Rule>) {
    for alt in rhs_str.split('|') {
        let alt = alt.trim();
        let rhs = if alt.is_empty() || alt == "lambda" {
            vec![]
        } else {
            alt.split_whitespace()
                .filter(|s| *s != "lambda")
                .map(|s| s.to_string())
                .collect()
        };
        rules.push(Rule { lhs: lhs.to_string(), rhs });
    }
}
  
fn parse_cell(cell: &str) -> LRAction {
    if let Some(rest) = cell.strip_prefix("R-") {
        if let Ok(n) = rest.parse::<usize>() { return LRAction::ReduceAccept(n); }
    }
    if let Some(rest) = cell.strip_prefix("r-") {
        if let Ok(n) = rest.parse::<usize>() { return LRAction::Reduce(n); }
    }
    if let Some(rest) = cell.strip_prefix("sh-") {
        if let Ok(n) = rest.parse::<usize>() { return LRAction::Shift(n); }
    }
    LRAction::Error
}
 
pub fn load_lr_table(path: &str, rules: &[Rule]) -> io::Result<LRData> {
    let content = fs::read_to_string(path)?;
    let mut lines = content.lines();
 
    let header_line = lines.next().ok_or_else(|| {
        io::Error::new(io::ErrorKind::InvalidData, "LR table file is empty")
    })?;
    let headers: Vec<&str> = header_line.split(',').collect();
 
    let mut table: HashMap<(usize, String), LRAction> = HashMap::new();
 
    for raw in lines {
        if raw.trim().is_empty() { continue; }
        let cells: Vec<&str> = raw.split(',').collect();
        if cells.is_empty() { continue; }
 
        let state: usize = match cells[0].trim().parse() {
            Ok(n)  => n,
            Err(_) => continue,
        };
 
        for (col, cell) in cells.iter().enumerate().skip(1) {
            let cell = cell.trim();
            if cell.is_empty() { continue; }
            if col >= headers.len() { continue; }
            let symbol = headers[col];
 
            let action = parse_cell(cell);
            if action != LRAction::Error {
                table.insert((state, symbol.to_string()), action);
            }
        }
    }
 
    Ok(LRData { rules: rules.to_vec(), table })
}
  
pub fn load_lr_data() -> io::Result<LRData> {
    let grammar_paths = ["zlang-pure.cfg", "zlang.cfg"];
    let mut rules = None;
    for p in &grammar_paths {
        match load_grammar(p) {
            Ok(r) => { rules = Some(r); break; }
            Err(_) => {}
        }
    }
    let rules = rules.ok_or_else(|| {
        io::Error::new(io::ErrorKind::NotFound,
            "Cannot find grammar file (tried zlang-pure.cfg, zlang.cfg)")
    })?;
 
    load_lr_table("zlang.lr", &rules)
}
