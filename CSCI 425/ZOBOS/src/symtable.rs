use std::fs;
use std::io::{self, Write};

#[derive(Debug, Clone, PartialEq)]
pub enum SymKind {
    Var,
    FuncProto,
    FuncDef,
}

#[derive(Debug, Clone)]
pub struct Symbol {
    pub name:        String,
    pub base_ty:     String,
    pub is_const:    bool,
    pub kind:        SymKind,
    pub param_tys:   Vec<String>,
    pub scope:       usize,
    pub line:        usize,
    pub col:         usize,
    pub initialized: bool,
    pub used:        bool,
}

impl Symbol {
    pub fn type_str(&self) -> String {
        let prefix = if self.is_const { "const " } else { "" };
        match self.kind {
            SymKind::Var => format!("{}{}", prefix, self.base_ty),
            SymKind::FuncDef => {
                if self.param_tys.is_empty() {
                    format!("const {}//", self.base_ty)
                } else {
                    format!("const {}//{}", self.base_ty, self.param_tys.join("/"))
                }
            }
            SymKind::FuncProto => {
                if self.param_tys.is_empty() {
                    format!("{}//", self.base_ty)
                } else {
                    format!("{}//{}", self.base_ty, self.param_tys.join("/"))
                }
            }
        }
    }

    pub fn is_function(&self) -> bool {
        matches!(self.kind, SymKind::FuncProto | SymKind::FuncDef)
    }
}

pub struct SymbolTable {
    pub symbols:    Vec<Symbol>,
    pub scope_stack: Vec<usize>,
    pub next_scope: usize,
}

impl SymbolTable {
    pub fn new() -> Self {
        SymbolTable {
            symbols:     Vec::new(),
            scope_stack: vec![0],
            next_scope:  1,
        }
    }

    pub fn current_scope(&self) -> usize {
        *self.scope_stack.last().unwrap_or(&0)
    }

    pub fn push_scope(&mut self) -> usize {
        let n = self.next_scope;
        self.next_scope += 1;
        self.scope_stack.push(n);
        n
    }

    pub fn pop_scope(&mut self) {
        if self.scope_stack.len() > 1 {
            self.scope_stack.pop();
        }
    }

    pub fn lookup_in_current(&self, name: &str) -> Option<usize> {
        let cur = self.current_scope();
        for (i, s) in self.symbols.iter().enumerate() {
            if s.scope == cur && s.name == name {
                return Some(i);
            }
        }
        None
    }

    pub fn lookup_global(&self, name: &str) -> Option<usize> {
        for (i, s) in self.symbols.iter().enumerate() {
            if s.scope == 0 && s.name == name {
                return Some(i);
            }
        }
        None
    }

    pub fn lookup(&self, name: &str) -> Option<usize> {
        for &scope in self.scope_stack.iter().rev() {
            for (i, s) in self.symbols.iter().enumerate() {
                if s.scope == scope && s.name == name {
                    return Some(i);
                }
            }
        }
        None
    }

    pub fn insert(&mut self, sym: Symbol) -> usize {
        let i = self.symbols.len();
        self.symbols.push(sym);
        i
    }

    pub fn write_symtable(&self, path: &str) -> io::Result<()> {
        let mut sorted: Vec<&Symbol> = self.symbols.iter().collect();
        sorted.sort_by(|a, b| a.scope.cmp(&b.scope).then(a.name.cmp(&b.name)));

        let mut f = fs::File::create(path)?;
        for sym in sorted {
            writeln!(f, "{},{},{}", sym.scope, sym.type_str(), sym.name)?;
        }
        Ok(())
    }
}
