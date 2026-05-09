use std::collections::HashMap;

use crate::ast::{Arena, NodeId, NodeKind};
use crate::symtable::{SymKind, Symbol, SymbolTable};

#[derive(Debug, Clone, Copy, PartialEq)]
pub enum Ty {
    Int,
    Float,
    Bool,
    Str,
    Func,
    Error,
    Unknown,
}

impl Ty {
    pub fn name(&self) -> &'static str {
        match self {
            Ty::Int => "int",
            Ty::Float => "float",
            Ty::Bool => "bool",
            Ty::Str => "string",
            Ty::Func => "function",
            Ty::Error | Ty::Unknown => "?",
        }
    }
    pub fn from_basetype(s: &str) -> Ty {
        match s {
            "int" => Ty::Int,
            "float" => Ty::Float,
            "bool" => Ty::Bool,
            "string" => Ty::Str,
            _ => Ty::Unknown,
        }
    }
    pub fn is_numeric(&self) -> bool {
        matches!(self, Ty::Int | Ty::Float)
    }
    pub fn is_known(&self) -> bool {
        !matches!(self, Ty::Error | Ty::Unknown)
    }
}

pub struct DiagMessage {
    pub line: usize,
    pub col: usize,
    pub class: String,
    pub id: String,
    pub is_error: bool,
}

pub struct SemanticAnalyzer {
    sym_path: String,
    pub messages: Vec<DiagMessage>,
    has_errors: bool,
    pub io_failed: bool,
    pub table: SymbolTable,
    // Track unique combos so we don't dup
    emitted: HashMap<(usize, usize, String, String), bool>,
}

impl SemanticAnalyzer {
    pub fn new(sym_path: String) -> Self {
        SemanticAnalyzer {
            sym_path,
            messages: Vec::new(),
            has_errors: false,
            io_failed: false,
            table: SymbolTable::new(),
            emitted: HashMap::new(),
        }
    }

    pub fn has_semantic_errors(&self) -> bool {
        self.has_errors
    }

    pub fn messages(&self) -> impl Iterator<Item = String> + '_ {
        self.messages
            .iter()
            .map(|m| format!(":{}: {} {} :{}:", m.class, m.line, m.col, m.id))
    }

    fn emit(
        &mut self,
        line: usize,
        col: usize,
        class: &str,
        id: &str,
        is_error: bool,
        allow_dup: bool,
    ) {
        let key = (line, col, class.to_string(), id.to_string());
        if !allow_dup {
            if self.emitted.contains_key(&key) {
                return;
            }
            self.emitted.insert(key, true);
        }
        if is_error {
            self.has_errors = true;
        }
        self.messages.push(DiagMessage {
            line,
            col,
            class: class.to_string(),
            id: id.to_string(),
            is_error,
        });
    }

    fn warn(&mut self, line: usize, col: usize, id: &str) {
        if id == "NOIDENT" || id == "CONST" {
            return;
        }
        self.emit(line, col, "WARN", id, false, false);
    }

    fn err(&mut self, line: usize, col: usize, id: &str) {
        if id == "NOIDENT" || id == "FNCONV" {
            return;
        }
        self.emit(line, col, "ERROR", id, true, false);
    }

    fn domain_msg(&mut self, line: usize, col: usize, ty: Ty) {
        let n = ty.name();
        self.emit(line, col, "DOMAIN", n, false, false);
    }

    pub fn analyze(&mut self, arena: &mut Arena, root: NodeId) {
        self.preregister_globals(arena, root);
        self.visit_module(arena, root);
        self.emit_unused_for_scope(0);
    }

    fn preregister_globals(&mut self, arena: &Arena, root: NodeId) {
        let kids = arena.get(root).children.clone();
        for c in kids {
            let kind = arena.get(c).kind.clone();
            match kind {
                NodeKind::FunSig => self.collect_func_sig(arena, c, false),
                NodeKind::FuncDef => {
                    if let Some(&sig) = arena.get(c).children.first() {
                        if arena.get(sig).kind == NodeKind::FunSig {
                            self.collect_func_sig(arena, sig, true);
                        }
                    }
                }
                _ => {}
            }
        }
    }

    fn collect_func_sig(&mut self, arena: &Arena, sig: NodeId, is_def: bool) {
        let kids = arena.get(sig).children.clone();
        if kids.len() < 2 {
            return;
        }
        let type_node = arena.get(kids[0]);
        let name_node = arena.get(kids[1]);
        let base_ty = label_type(&type_node.label);
        let fname = decode_lexeme(&name_node.lexeme);
        let fline = name_node.line;
        let fcol = name_node.col;
        let mut params: Vec<String> = Vec::new();
        if kids.len() >= 3 {
            let pl = kids[2];
            if arena.get(pl).kind == NodeKind::ParamList {
                for &p in &arena.get(pl).children.clone() {
                    if arena.get(p).kind == NodeKind::Param {
                        let pkids = arena.get(p).children.clone();
                        if !pkids.is_empty() {
                            params.push(label_type(&arena.get(pkids[0]).label));
                        }
                    }
                }
            }
        }

        let existing = self.table.lookup_global(&fname);
        match existing {
            None => {
                let kind = if is_def {
                    SymKind::FuncDef
                } else {
                    SymKind::FuncProto
                };
                self.table.insert(Symbol {
                    name: fname,
                    base_ty,
                    is_const: true,
                    kind,
                    param_tys: params,
                    scope: 0,
                    line: fline,
                    col: fcol,
                    initialized: true,
                    used: false,
                });
            }
            Some(idx) => {
                let prev_kind = self.table.symbols[idx].kind.clone();
                if is_def && matches!(prev_kind, SymKind::FuncProto) {
                    self.table.symbols[idx].kind = SymKind::FuncDef;
                    self.table.symbols[idx].param_tys = params;
                } else if is_def {
                    self.warn(fline, fcol, "CONST");
                } else {
                    self.warn(fline, fcol, "REIDENT");
                }
            }
        }
    }

    fn visit_module(&mut self, arena: &mut Arena, root: NodeId) {
        let kids = arena.get(root).children.clone();
        for c in kids {
            self.visit_top(arena, c);
        }
    }

    fn visit_top(&mut self, arena: &mut Arena, node: NodeId) {
        let kind = arena.get(node).kind.clone();
        match kind {
            NodeKind::DeclList => self.visit_decl_list(arena, node),
            NodeKind::FunSig => { /* already collected */ }
            NodeKind::FuncDef => self.visit_function(arena, node),
            NodeKind::Emit => self.visit_emit(arena, node),
            NodeKind::EmitSymtable => self.visit_emit_symtable(arena, node),
            NodeKind::Rand => self.visit_rand(arena, node),
            NodeKind::If | NodeKind::IfElse | NodeKind::While | NodeKind::DoWhile => {
                self.visit_control(arena, node);
            }
            NodeKind::Assign => {
                let _ = self.visit_assign(arena, node, false, None);
            }
            _ => {}
        }
    }

    // Visit a DeclList (DECLLIST | GOTDECLLIST | GFTDECLLIST)
    fn visit_decl_list(&mut self, arena: &mut Arena, node: NodeId) {
        let kids = arena.get(node).children.clone();
        if kids.len() < 2 {
            return;
        }
        let type_lex = arena.get(kids[0]).label.clone();
        let (base_ty, is_const) = parse_type_label(&type_lex);
        let declids_node = kids[1];
        let declid_kids = arena.get(declids_node).children.clone();
        for d in declid_kids {
            self.visit_declid(arena, d, &base_ty, is_const);
        }
    }

    // declid is either an id leaf or an Assign node
    fn visit_declid(&mut self, arena: &mut Arena, node: NodeId, base_ty: &str, is_const: bool) {
        let kind = arena.get(node).kind.clone();
        match kind {
            NodeKind::Assign => {
                // ASSIGN(id, RHS).  Declare id first.
                let n = arena.get(node).clone();
                let assign_line = n.line;
                let assign_col = n.col;
                let n_kids = n.children.clone();
                if n_kids.is_empty() {
                    return;
                }
                let id_node = arena.get(n_kids[0]).clone();
                let id_name = decode_lexeme(&id_node.lexeme);
                let id_line = id_node.line;
                let id_col = id_node.col;
                self.declare_var(&id_name, base_ty, is_const, id_line, id_col);
                if n_kids.len() >= 2 {
                    let rhs_ty = self.visit_decl_assign_rhs(arena, n_kids[1], base_ty, is_const);
                    let lhs_ty = Ty::from_basetype(base_ty);
                    let rhs_call = _rhs_is_funcall(arena, n_kids[1]);
                    self.check_assign_compatible(assign_line, assign_col, lhs_ty, rhs_ty, rhs_call);
                    self.mark_initialized(&id_name);
                }
            }
            NodeKind::Leaf => {
                // Plain id
                let id_node = arena.get(node);
                let id_name = decode_lexeme(&id_node.lexeme);
                let id_line = id_node.line;
                let id_col = id_node.col;
                self.declare_var(&id_name, base_ty, is_const, id_line, id_col);
            }
            _ => {}
        }
    }

    // RHS of an Assign in a DeclList: may be another Assign (cascade), or an EXPR
    // Returns the result type for type-compat checks at the outer level.
    fn visit_decl_assign_rhs(
        &mut self,
        arena: &mut Arena,
        node: NodeId,
        base_ty: &str,
        is_const: bool,
    ) -> Ty {
        let kind = arena.get(node).kind.clone();
        if kind == NodeKind::Assign {
            let n = arena.get(node).clone();
            let assign_line = n.line;
            let assign_col = n.col;
            let n_kids = n.children.clone();
            if n_kids.is_empty() {
                return Ty::Error;
            }
            let id_node = arena.get(n_kids[0]).clone();
            let id_name = decode_lexeme(&id_node.lexeme);
            let id_line = id_node.line;
            let id_col = id_node.col;
            self.declare_var(&id_name, base_ty, is_const, id_line, id_col);
            if n_kids.len() >= 2 {
                let rhs_ty = self.visit_decl_assign_rhs(arena, n_kids[1], base_ty, is_const);
                let lhs_ty = Ty::from_basetype(base_ty);
                let rhs_call = _rhs_is_funcall(arena, n_kids[1]);
                self.check_assign_compatible(assign_line, assign_col, lhs_ty, rhs_ty, rhs_call);
                self.mark_initialized(&id_name);
                // Mark inner LHS used (its value is read by the outer cascade)
                if let Some(idx) = self.table.lookup(&id_name) {
                    self.table.symbols[idx].used = true;
                }
                return rhs_ty;
            }
            Ty::Error
        } else {
            self.visit_expr(arena, node)
        }
    }

    fn declare_var(&mut self, name: &str, base_ty: &str, is_const: bool, line: usize, col: usize) {
        // Check current scope for redeclaration
        if let Some(_) = self.table.lookup_in_current(name) {
            self.warn(line, col, "REIDENT");
            return;
        }
        let mut effective_const = is_const;
        if base_ty == "string" {
            effective_const = true;
        }
        let scope = self.table.current_scope();
        self.table.insert(Symbol {
            name: name.to_string(),
            base_ty: base_ty.to_string(),
            is_const: effective_const,
            kind: SymKind::Var,
            param_tys: Vec::new(),
            scope,
            line,
            col,
            initialized: false,
            used: false,
        });
    }

    fn mark_initialized(&mut self, name: &str) {
        if let Some(idx) = self.table.lookup(name) {
            self.table.symbols[idx].initialized = true;
        }
    }

    fn mark_used(&mut self, name: &str) {
        if let Some(idx) = self.table.lookup(name) {
            self.table.symbols[idx].used = true;
        }
    }

    // visit_function: process FuncDef
    fn visit_function(&mut self, arena: &mut Arena, node: NodeId) {
        let kids = arena.get(node).children.clone();
        if kids.len() < 5 {
            return;
        }
        let sig = kids[0];
        let ret_id_node = kids[1];
        // kids[2] is "assign:=", kids[3] is the EXPR, kids[4] is STEPS
        let ret_expr = kids[3];
        let body = kids[4];

        // Mark function as used? No, function is unused unless called.
        let sig_kids = arena.get(sig).children.clone();
        if sig_kids.len() < 2 {
            return;
        }
        let ret_type_label = arena.get(sig_kids[0]).label.clone();
        let fname_node = arena.get(sig_kids[1]).clone();
        let fname = decode_lexeme(&fname_node.lexeme);
        let (ret_base_ty, _) = parse_type_label(&ret_type_label);

        // Push function scope
        self.table.push_scope();

        // Add parameters
        if sig_kids.len() >= 3 {
            let pl = sig_kids[2];
            if arena.get(pl).kind == NodeKind::ParamList {
                let pkids = arena.get(pl).children.clone();
                for p in pkids {
                    if arena.get(p).kind == NodeKind::Param {
                        let pk = arena.get(p).children.clone();
                        if pk.len() >= 2 {
                            let pty_label = arena.get(pk[0]).label.clone();
                            let pid = arena.get(pk[1]).clone();
                            let pname = decode_lexeme(&pid.lexeme);
                            let (pbase, pconst) = parse_type_label(&pty_label);
                            // Param REIDENT check (against this scope)
                            if self.table.lookup_in_current(&pname).is_some() {
                                self.warn(pid.line, pid.col, "REIDENT");
                            } else {
                                let _ = pconst;
                                let scope = self.table.current_scope();
                                self.table.insert(Symbol {
                                    name: pname,
                                    base_ty: pbase,
                                    is_const: true,
                                    kind: SymKind::Var,
                                    param_tys: Vec::new(),
                                    scope,
                                    line: pid.line,
                                    col: pid.col,
                                    initialized: true,
                                    used: false,
                                });
                            }
                        }
                    }
                }
            }
        }

        // Declare return id at function scope
        let ret_id = arena.get(ret_id_node).clone();
        let ret_name = decode_lexeme(&ret_id.lexeme);
        let ret_line = ret_id.line;
        let ret_col = ret_id.col;
        let ret_already = self.table.lookup_in_current(&ret_name).is_some();
        if ret_already {
            self.warn(ret_line, ret_col, "REIDENT");
            if let Some(idx) = self.table.lookup_in_current(&ret_name) {
                if self.table.symbols[idx].is_const {
                    self.warn(ret_line, ret_col, "CONST");
                }
            }
        } else {
            let scope = self.table.current_scope();
            self.table.insert(Symbol {
                name: ret_name.clone(),
                base_ty: ret_base_ty.clone(),
                is_const: ret_base_ty == "string",
                kind: SymKind::Var,
                param_tys: Vec::new(),
                scope,
                line: ret_line,
                col: ret_col,
                initialized: false,
                used: false,
            });
        }

        let r_ty = self.visit_expr(arena, ret_expr);
        let lhs_ty = Ty::from_basetype(&ret_base_ty);
        // Use the `=` token's location (kids[2] is the assign:= leaf)
        let (eq_line, eq_col) = if kids.len() >= 3 {
            let an = arena.get(kids[2]);
            if an.line > 0 {
                (an.line, an.col)
            } else {
                (ret_line, ret_col)
            }
        } else {
            (ret_line, ret_col)
        };
        let rhs_call = _rhs_is_funcall(arena, ret_expr);
        self.check_assign_compatible(eq_line, eq_col, lhs_ty, r_ty, rhs_call);
        self.mark_initialized(&ret_name);
        // Treat ret variable as used (return value is implicit use)
        self.mark_used(&ret_name);

        self.visit_steps(arena, body);

        self.emit_unused_for_scope(self.table.current_scope());
        self.table.pop_scope();

        let _ = fname;
    }

    fn visit_steps(&mut self, arena: &mut Arena, node: NodeId) {
        let kind = arena.get(node).kind.clone();
        match kind {
            NodeKind::BraceStmts => {
                self.table.push_scope();
                let kids = arena.get(node).children.clone();
                for c in kids {
                    self.visit_stmt(arena, c);
                }
                self.emit_unused_for_scope(self.table.current_scope());
                self.table.pop_scope();
            }
            NodeKind::Steps => {
                let kids = arena.get(node).children.clone();
                for c in kids {
                    self.visit_steps(arena, c);
                }
            }
            _ => self.visit_stmt(arena, node),
        }
    }

    fn visit_stmt(&mut self, arena: &mut Arena, node: NodeId) {
        let kind = arena.get(node).kind.clone();
        match kind {
            NodeKind::DeclList => self.visit_decl_list(arena, node),
            NodeKind::Assign => {
                let _ = self.visit_assign(arena, node, false, None);
            }
            NodeKind::If | NodeKind::IfElse | NodeKind::While | NodeKind::DoWhile => {
                self.visit_control(arena, node);
            }
            NodeKind::Emit => self.visit_emit(arena, node),
            NodeKind::EmitSymtable => self.visit_emit_symtable(arena, node),
            NodeKind::Rand => self.visit_rand(arena, node),
            NodeKind::BraceStmts => self.visit_steps(arena, node),
            _ => {}
        }
    }

    fn visit_assign(
        &mut self,
        arena: &mut Arena,
        node: NodeId,
        inner: bool,
        _decl_ty: Option<&str>,
    ) -> Ty {
        let nclone = arena.get(node).clone();
        let kids = nclone.children.clone();
        if kids.len() < 2 {
            return Ty::Error;
        }
        let eq_line = nclone.line;
        let eq_col = nclone.col;

        let rhs_ty = self.visit_assign_rhs(arena, kids[1]);

        let id_node = arena.get(kids[0]).clone();
        let id_name = decode_lexeme(&id_node.lexeme);
        let id_line = id_node.line;
        let id_col = id_node.col;

        let lookup = self.table.lookup(&id_name);
        match lookup {
            None => {
                self.err(id_line, id_col, "NOIDENT");
                rhs_ty
            }
            Some(idx) => {
                let sym = self.table.symbols[idx].clone();
                let lhs_ty = if sym.is_function() {
                    Ty::Func
                } else {
                    Ty::from_basetype(&sym.base_ty)
                };

                if sym.is_const {
                    self.warn(id_line, id_col, "CONST");
                }
                if sym.is_function() {
                    if rhs_ty.is_known() && rhs_ty != Ty::Error && rhs_ty != Ty::Func {
                        self.err(eq_line, eq_col, "CONV");
                    }
                } else {
                    self.check_assign_compatible(
                        eq_line,
                        eq_col,
                        lhs_ty,
                        rhs_ty,
                        _rhs_is_funcall(arena, kids[1]),
                    );
                }

                self.table.symbols[idx].initialized = true;
                if inner {
                    // Inner LHS: mark used (since outer reads its value)
                    self.table.symbols[idx].used = true;
                }
                if sym.is_function() {
                    rhs_ty
                } else {
                    rhs_ty
                }
            }
        }
    }

    fn visit_assign_rhs(&mut self, arena: &mut Arena, node: NodeId) -> Ty {
        let kind = arena.get(node).kind.clone();
        if kind == NodeKind::Assign {
            self.visit_assign(arena, node, true, None)
        } else {
            self.visit_expr(arena, node)
        }
    }

    fn first_loc(&self, arena: &Arena, node: NodeId) -> (usize, usize) {
        let n = arena.get(node);
        if n.line > 0 {
            (n.line, n.col)
        } else if let Some(&c) = n.children.first() {
            self.first_loc(arena, c)
        } else {
            (0, 0)
        }
    }

    fn check_assign_compatible(
        &mut self,
        line: usize,
        col: usize,
        lhs: Ty,
        rhs: Ty,
        rhs_is_call: bool,
    ) {
        if !lhs.is_known() || !rhs.is_known() {
            return;
        }
        if lhs == rhs {
            return;
        }
        if lhs == Ty::Float && rhs == Ty::Int {
            return;
        }
        if lhs == Ty::Int && rhs == Ty::Float {
            if rhs_is_call {
                self.err(line, col, "FNCONV");
            } else {
                self.err(line, col, "CONV");
            }
            return;
        }
        if rhs_is_call {
            self.err(line, col, "FNCONV");
        } else {
            self.err(line, col, "CONV");
        }
    }

    fn visit_control(&mut self, arena: &mut Arena, node: NodeId) {
        let kind = arena.get(node).kind.clone();
        let kids = arena.get(node).children.clone();
        match kind {
            NodeKind::If => {
                if !kids.is_empty() {
                    let _ = self.visit_expr(arena, kids[0]);
                }
                if kids.len() >= 2 {
                    self.visit_steps(arena, kids[1]);
                }
            }
            NodeKind::IfElse => {
                if !kids.is_empty() {
                    let _ = self.visit_expr(arena, kids[0]);
                }
                if kids.len() >= 2 {
                    self.visit_steps(arena, kids[1]);
                }
                if kids.len() >= 3 {
                    self.visit_steps(arena, kids[2]);
                }
            }
            NodeKind::While => {
                if !kids.is_empty() {
                    let _ = self.visit_expr(arena, kids[0]);
                }
                if kids.len() >= 2 {
                    self.visit_steps(arena, kids[1]);
                }
            }
            NodeKind::DoWhile => {
                // do BRACESTMTS while (BEXPR)
                if !kids.is_empty() {
                    self.visit_steps(arena, kids[0]);
                }
                if kids.len() >= 2 {
                    let _ = self.visit_expr(arena, kids[1]);
                }
            }
            _ => {}
        }
    }

    fn visit_emit(&mut self, arena: &mut Arena, node: NodeId) {
        let kids = arena.get(node).children.clone();
        if kids.len() == 3 {
            let id_node = arena.get(kids[0]).clone();
            let name = decode_lexeme(&id_node.lexeme);
            match self.table.lookup(&name) {
                None => self.err(id_node.line, id_node.col, "NOIDENT"),
                Some(idx) => {
                    let sym = self.table.symbols[idx].clone();
                    self.table.symbols[idx].used = true;
                    if sym.is_function() {
                        self.err(id_node.line, id_node.col, "EXPR");
                    } else {
                        let bt = Ty::from_basetype(&sym.base_ty);
                        if !sym.initialized {
                            self.warn(id_node.line, id_node.col, "UNINIT");
                        }
                        if bt != Ty::Str && bt.is_known() {
                            self.err(id_node.line, id_node.col, "CONV");
                        }
                    }
                }
            }
            for ai in 1..3 {
                let a = kids[ai];
                let aloc = self.first_loc(arena, a);
                let aty = self.visit_expr(arena, a);
                if aty == Ty::Str {
                    self.err(aloc.0, aloc.1, "CONV");
                }
            }
        } else if kids.len() == 1 {
            let aexpr = kids[0];
            let aexpr_loc = self.first_loc(arena, aexpr);
            let ty = self.visit_expr(arena, aexpr);
            if ty == Ty::Str {
                self.err(aexpr_loc.0, aexpr_loc.1, "CONV");
            }
        }
    }

    fn visit_emit_symtable(&mut self, _arena: &mut Arena, _node: NodeId) {
        if let Err(_) = self.table.write_symtable(&self.sym_path) {
            self.io_failed = true;
        }
    }

    fn visit_rand(&mut self, arena: &mut Arena, node: NodeId) {
        let kids = arena.get(node).children.clone();
        if kids.is_empty() {
            return;
        }
        let id_node = arena.get(kids[0]).clone();
        let name = decode_lexeme(&id_node.lexeme);
        match self.table.lookup(&name) {
            None => {
                self.err(id_node.line, id_node.col, "NOIDENT");
            }
            Some(idx) => {
                let sym = self.table.symbols[idx].clone();
                if sym.is_const {
                    self.warn(id_node.line, id_node.col, "CONST");
                }
                let bt = Ty::from_basetype(&sym.base_ty);
                if bt == Ty::Str {
                    self.err(id_node.line, id_node.col, "CONV");
                }
                self.table.symbols[idx].initialized = true;
                self.table.symbols[idx].used = true;
            }
        }
        for &c in kids.iter().skip(1) {
            let _ = self.visit_expr(arena, c);
        }
    }

    fn emit_unused_for_scope(&mut self, scope: usize) {
        let mut emitted = Vec::new();
        for sym in &self.table.symbols {
            if sym.scope == scope && !sym.used {
                emitted.push((sym.line, sym.col));
            }
        }
        for (l, c) in emitted {
            self.warn(l, c, "UNUSED");
        }
    }

    fn visit_expr(&mut self, arena: &mut Arena, node: NodeId) -> Ty {
        let nclone = arena.get(node).clone();
        let kind = nclone.kind.clone();
        let label = nclone.label.clone();
        let line = nclone.line;
        let col = nclone.col;
        let kids = nclone.children.clone();
        match kind {
            NodeKind::Leaf => {
                if label.starts_with("intval") {
                    return Ty::Int;
                }
                if label.starts_with("floatval") {
                    return Ty::Float;
                }
                if label.starts_with("stringval") {
                    return Ty::Str;
                }
                if label.starts_with("id:") {
                    // Identifier reference
                    let name = decode_lexeme(&nclone.lexeme);
                    match self.table.lookup(&name) {
                        None => {
                            self.err(line, col, "NOIDENT");
                            Ty::Error
                        }
                        Some(idx) => {
                            let sym = self.table.symbols[idx].clone();
                            self.table.symbols[idx].used = true;
                            if sym.is_function() {
                                // Function used as value: EXPR error
                                self.err(line, col, "EXPR");
                                return Ty::Error;
                            }
                            if !sym.initialized {
                                self.warn(line, col, "UNINIT");
                            }
                            Ty::from_basetype(&sym.base_ty)
                        }
                    }
                } else if label == "?" || kids.is_empty() {
                    Ty::Unknown
                } else {
                    Ty::Unknown
                }
            }
            NodeKind::Plus => {
                if kids.len() != 2 {
                    return Ty::Error;
                }
                let lt = self.visit_expr(arena, kids[0]);
                let rt = self.visit_expr(arena, kids[1]);
                self.binop_arith(line, col, lt, rt, true, arena, node)
            }
            NodeKind::Mult => {
                if kids.len() != 2 {
                    return Ty::Error;
                }
                let lt = self.visit_expr(arena, kids[0]);
                let rt = self.visit_expr(arena, kids[1]);
                let is_mod = label.starts_with("MULT:%");
                self.binop_arith(line, col, lt, rt, !is_mod, arena, node)
            }
            NodeKind::Bools => {
                if kids.len() != 2 {
                    return Ty::Error;
                }
                let lt = self.visit_expr(arena, kids[0]);
                let rt = self.visit_expr(arena, kids[1]);
                let any_func =
                    self.is_funcall_value(arena, kids[0]) || self.is_funcall_value(arena, kids[1]);
                let _ = any_func;
                if !lt.is_known() || !rt.is_known() {
                    return Ty::Bool;
                }
                if !lt.is_numeric() || !rt.is_numeric() {
                    let pos = self.expr_error_pos(arena, kids[0], kids[1], line, col);
                    self.err(pos.0, pos.1, "EXPR");
                }
                Ty::Bool
            }
            NodeKind::Unary => {
                if kids.len() != 2 {
                    return Ty::Error;
                }
                let op = label.trim_start_matches("UNARY:").to_string();
                let inner = self.visit_expr(arena, kids[1]);
                if !inner.is_known() {
                    return Ty::Error;
                }
                match op.as_str() {
                    "!" => {
                        if inner != Ty::Bool {
                            self.err(line, col, "EXPR");
                            return Ty::Error;
                        }
                        Ty::Bool
                    }
                    "~" => {
                        if inner != Ty::Int {
                            self.err(line, col, "EXPR");
                            return Ty::Error;
                        }
                        Ty::Int
                    }
                    "+" | "-" => {
                        if !inner.is_numeric() {
                            self.err(line, col, "EXPR");
                            return Ty::Error;
                        }
                        inner
                    }
                    _ => Ty::Error,
                }
            }
            NodeKind::Cast => {
                let cty = label.trim_start_matches("CAST:");
                let target_ty = Ty::from_basetype(cty);
                let inner_node = if kids.len() >= 2 {
                    kids[1]
                } else if kids.len() == 1 {
                    kids[0]
                } else {
                    return Ty::Error;
                };
                let inner_ty = self.visit_expr(arena, inner_node);
                if !inner_ty.is_known() {
                    return target_ty;
                }
                if inner_ty == Ty::Func {
                    let pos = self.first_loc(arena, inner_node);
                    self.err(pos.0, pos.1, "EXPR");
                    return Ty::Error;
                }
                if inner_ty == Ty::Str {
                    let pos = self.first_loc(arena, inner_node);
                    self.err(pos.0, pos.1, "EXPR");
                    return Ty::Error;
                }
                target_ty
            }
            NodeKind::Domain => {
                let inner = if !kids.is_empty() {
                    self.visit_expr(arena, kids[0])
                } else {
                    Ty::Error
                };
                let ty = if inner.is_known() { inner } else { Ty::Float };
                self.domain_msg(line, col, ty);
                inner
            }
            NodeKind::FunCall => self.visit_funcall(arena, node),
            NodeKind::Assign => self.visit_assign(arena, node, true, None),
            NodeKind::Module
            | NodeKind::ModParts
            | NodeKind::DeclList
            | NodeKind::DeclIds
            | NodeKind::DeclId
            | NodeKind::FuncDef
            | NodeKind::FunSig
            | NodeKind::ParamList
            | NodeKind::Param
            | NodeKind::Steps
            | NodeKind::BraceStmts
            | NodeKind::BStmts
            | NodeKind::BStmt
            | NodeKind::SoloStmt
            | NodeKind::If
            | NodeKind::IfElse
            | NodeKind::While
            | NodeKind::DoWhile
            | NodeKind::Emit
            | NodeKind::EmitSymtable
            | NodeKind::Rand
            | NodeKind::Mod
            | NodeKind::ArgList
            | NodeKind::DollarSign => Ty::Unknown,
        }
    }

    fn binop_arith(
        &mut self,
        line: usize,
        col: usize,
        lt: Ty,
        rt: Ty,
        allow_float: bool,
        _arena: &Arena,
        _node: NodeId,
    ) -> Ty {
        if !lt.is_known() || !rt.is_known() {
            return Ty::Error;
        }
        let l_ok = lt.is_numeric();
        let r_ok = rt.is_numeric();
        if !l_ok || !r_ok {
            self.err(line, col, "EXPR");
            return Ty::Error;
        }
        if !allow_float && (lt == Ty::Float || rt == Ty::Float) {
            // mod requires int
            self.err(line, col, "EXPR");
            return Ty::Error;
        }
        if lt == Ty::Float || rt == Ty::Float {
            Ty::Float
        } else {
            Ty::Int
        }
    }

    fn expr_error_pos(
        &self,
        arena: &Arena,
        lhs: NodeId,
        rhs: NodeId,
        op_line: usize,
        op_col: usize,
    ) -> (usize, usize) {
        if let Some(p) = self.func_id_loc(arena, lhs) {
            return p;
        }
        if let Some(p) = self.func_id_loc(arena, rhs) {
            return p;
        }
        (op_line, op_col)
    }

    fn func_id_loc(&self, arena: &Arena, node: NodeId) -> Option<(usize, usize)> {
        let n = arena.get(node);
        if n.kind == NodeKind::Leaf && n.label.starts_with("id:") {
            let name = decode_lexeme(&n.lexeme);
            if let Some(idx) = self.table.lookup(&name) {
                if self.table.symbols[idx].is_function() {
                    return Some((n.line, n.col));
                }
            }
        }
        None
    }

    fn is_funcall_value(&self, arena: &Arena, node: NodeId) -> bool {
        let n = arena.get(node);
        n.kind == NodeKind::FunCall
    }

    fn visit_funcall(&mut self, arena: &mut Arena, node: NodeId) -> Ty {
        let nclone = arena.get(node).clone();
        let kids = nclone.children.clone();
        if kids.is_empty() {
            return Ty::Error;
        }
        let id_node = arena.get(kids[0]).clone();
        let name = decode_lexeme(&id_node.lexeme);
        let id_line = id_node.line;
        let id_col = id_node.col;
        let mut arg_tys: Vec<(Ty, NodeId)> = Vec::new();
        for &c in kids.iter().skip(1) {
            let t = self.visit_expr(arena, c);
            arg_tys.push((t, c));
        }
        match self.table.lookup(&name) {
            None => {
                self.err(id_line, id_col, "NOIDENT");
                Ty::Error
            }
            Some(idx) => {
                let sym = self.table.symbols[idx].clone();
                self.table.symbols[idx].used = true;
                if !sym.is_function() {
                    // Calling a non-function: CALL error
                    self.err(id_line, id_col, "CALL");
                    return Ty::Error;
                }
                if sym.param_tys.len() != arg_tys.len() {
                    self.err(id_line, id_col, "CALL");
                } else {
                    for (i, (at, _an)) in arg_tys.iter().enumerate() {
                        let pt = Ty::from_basetype(&sym.param_tys[i]);
                        let at = *at;
                        if !at.is_known() || !pt.is_known() {
                            continue;
                        }
                        if at == pt {
                            continue;
                        }
                        if pt == Ty::Int && at == Ty::Bool {
                            continue;
                        }
                        // FNCONV at function name col
                        self.err(id_line, id_col, "FNCONV");
                    }
                }
                Ty::from_basetype(&sym.base_ty)
            }
        }
    }
}

fn _rhs_is_funcall(arena: &Arena, node: NodeId) -> bool {
    let kind = arena.get(node).kind.clone();
    kind == NodeKind::FunCall
}

fn label_type(label: &str) -> String {
    if let Some(t) = label.strip_prefix("type:") {
        return t.trim().to_string();
    }
    label.to_string()
}

fn parse_type_label(label: &str) -> (String, bool) {
    let s = label_type(label);
    if let Some(rest) = s.strip_prefix("const ") {
        (rest.trim().to_string(), true)
    } else {
        (s, false)
    }
}

fn decode_lexeme(s: &str) -> String {
    let bytes = s.as_bytes();
    let mut out = String::new();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i] == b'x' && i + 2 < bytes.len() {
            let h1 = bytes[i + 1];
            let h2 = bytes[i + 2];
            if h1.is_ascii_hexdigit() && h2.is_ascii_hexdigit() {
                let v = (hex_val(h1) << 4) | hex_val(h2);
                out.push(v as char);
                i += 3;
                continue;
            }
        }
        out.push(bytes[i] as char);
        i += 1;
    }
    out
}

fn hex_val(b: u8) -> u8 {
    match b {
        b'0'..=b'9' => b - b'0',
        b'a'..=b'f' => b - b'a' + 10,
        b'A'..=b'F' => b - b'A' + 10,
        _ => 0,
    }
}
