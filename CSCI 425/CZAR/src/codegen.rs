use std::collections::HashMap;
use std::fs;
use std::io::{self, Write};

use crate::ast::{Arena, NodeId, NodeKind};
use crate::symtable::SymbolTable;

const MAX_SPILL_SLOTS: usize = 16;

pub struct CodeGenerator {
    rn: usize,
    fn_regs: usize,
}

impl CodeGenerator {
    pub fn new(rn: usize, fn_regs: usize) -> Self {
        CodeGenerator { rn, fn_regs }
    }

    pub fn generate(
        &mut self,
        arena: &Arena,
        root: NodeId,
        symtable: &mut SymbolTable,
        output_path: &str,
    ) -> io::Result<()> {
        let mut ctx = CgenCtx::new(self.rn, self.fn_regs);

        ctx.data_seg(symtable, arena, root);
        ctx.compute_reg_needs(arena, root);

        let init_instrs = ctx.gen_global_instrs(arena, root, symtable);
        let func_instrs = ctx.gen_func_instrs(arena, root, symtable);

        ctx.write_czr(output_path, symtable, &init_instrs, &func_instrs)
    }
}

struct CgenCtx {
    rn: usize,
    fn_regs: usize,
    alloc_rn: usize,
    alloc_fn: usize,

    free_gp: Vec<bool>,
    free_fp: Vec<bool>,
    temp_counter: usize,

    addr_map: HashMap<String, usize>,
    string_literals: HashMap<String, usize>,
    string_values: HashMap<String, String>,
    data_size: usize,

    reg_needs: HashMap<NodeId, usize>,

    local_offsets: HashMap<String, i32>,
    next_local_offset: i32,

    spill_base: usize,
    gp_spill_slots: Vec<bool>,
    fp_spill_slots: Vec<bool>,
}

impl CgenCtx {
    fn new(rn: usize, fn_regs: usize) -> Self {
        let alloc_rn = rn;
        let alloc_fn = fn_regs;

        CgenCtx {
            rn,
            fn_regs,
            alloc_rn,
            alloc_fn,
            addr_map: HashMap::new(),
            string_literals: HashMap::new(),
            string_values: HashMap::new(),
            data_size: 0,
            reg_needs: HashMap::new(),
            local_offsets: HashMap::new(),
            next_local_offset: -4,
            free_gp: vec![true; alloc_rn],
            free_fp: vec![true; alloc_fn],
            temp_counter: 0,
            spill_base: 0,
            gp_spill_slots: vec![true; MAX_SPILL_SLOTS],
            fp_spill_slots: vec![true; MAX_SPILL_SLOTS],
        }
    }

    fn sp(&self) -> String {
        if self.rn >= 1 { format!("R{}", self.rn - 1) } else { "R0".to_string() }
    }
    fn fp(&self) -> String {
        if self.rn >= 2 { format!("R{}", self.rn - 2) } else { "R1".to_string() }
    }
    fn w0(&self) -> String {
        if self.rn >= 3 { format!("R{}", self.rn - 3) } else { "R2".to_string() }
    }
    fn w1(&self) -> String {
        if self.rn >= 4 { format!("R{}", self.rn - 4) } else { "R3".to_string() }
    }

    fn alloc_gp(&mut self) -> Option<String> {
        for i in 0..self.free_gp.len() {
            if self.free_gp[i] {
                self.free_gp[i] = false;
                return Some(format!("R{}", i));
            }
        }
        None
    }

    fn alloc_fp(&mut self) -> Option<String> {
        for i in 0..self.free_fp.len() {
            if self.free_fp[i] {
                self.free_fp[i] = false;
                return Some(format!("F{}", i));
            }
        }
        None
    }

    fn free_gp_reg(&mut self, reg: &str) {
        if let Some(idx) = reg.strip_prefix('R').and_then(|s| s.parse::<usize>().ok()) {
            if idx < self.free_gp.len() {
                self.free_gp[idx] = true;
            }
        }
    }

    fn free_fp_reg(&mut self, reg: &str) {
        if let Some(idx) = reg.strip_prefix('F').and_then(|s| s.parse::<usize>().ok()) {
            if idx < self.free_fp.len() {
                self.free_fp[idx] = true;
            }
        }
    }

    fn free_gp_count(&self) -> usize {
        self.free_gp.iter().filter(|&&f| f).count()
    }

    fn free_fp_count(&self) -> usize {
        self.free_fp.iter().filter(|&&f| f).count()
    }

    fn gp_spill_addr(&self, slot: usize) -> usize {
        self.spill_base + slot * 4
    }

    fn fp_spill_addr(&self, slot: usize) -> usize {
        self.spill_base + MAX_SPILL_SLOTS * 4 + slot * 4
    }

    fn alloc_spill_gp(&mut self) -> usize {
        for (i, free) in self.gp_spill_slots.iter_mut().enumerate() {
            if *free {
                *free = false;
                return i;
            }
        }
        panic!("GP spill area exhausted — increase MAX_SPILL_SLOTS");
    }

    fn alloc_spill_fp(&mut self) -> usize {
        for (i, free) in self.fp_spill_slots.iter_mut().enumerate() {
            if *free {
                *free = false;
                return i;
            }
        }
        panic!("FP spill area exhausted — increase MAX_SPILL_SLOTS");
    }

    fn free_spill_gp(&mut self, slot: usize) {
        self.gp_spill_slots[slot] = true;
    }

    fn free_spill_fp(&mut self, slot: usize) {
        self.fp_spill_slots[slot] = true;
    }

    fn data_seg(&mut self, symtable: &SymbolTable, arena: &Arena, root: NodeId) {
        let mut offset: usize = 0;

        self.collect_string_literals(arena, root);

        for sym in symtable
            .symbols
            .iter()
            .filter(|s| s.scope == 0 && !s.is_function() && s.base_ty != "string")
        {
            let (size, align) = type_size_align(&sym.base_ty);
            if align > 0 && offset % align != 0 {
                offset += align - (offset % align);
            }
            self.addr_map.insert(sym.name.clone(), offset);
            offset += size;
        }

        for sym in symtable
            .symbols
            .iter()
            .filter(|s| s.scope == 0 && !s.is_function() && s.base_ty == "string")
        {
            self.addr_map.insert(sym.name.clone(), offset);
            if let Some(value) = self.string_values.get(&sym.name) {
                self.string_literals.insert(sym.name.clone(), offset);
            }
            offset += if let Some(value) = self.string_values.get(&sym.name) {
                value.len() + 1
            } else {
                1
            };
        }

        self.data_size = offset;

        let sb = align_up(self.data_size, 4);
        self.spill_base = sb;
        self.data_size = sb + MAX_SPILL_SLOTS * 4 * 2;
    }

    fn collect_string_literals(&mut self, arena: &Arena, id: NodeId) {
        let node = arena.get(id);

        if node.kind == NodeKind::Assign {
            let kids = &node.children;
            if kids.len() >= 2 {
                let rhs = arena.get(kids[1]);
                if rhs.kind == NodeKind::Leaf && rhs.label.starts_with("stringval") {
                    let name_node = arena.get(kids[0]);
                    let name = decode_lex(&name_node.lexeme);
                    let string_value = rhs.lexeme.clone();
                    self.string_values.insert(name, string_value);
                }
            }
        }

        for &child in &node.children {
            self.collect_string_literals(arena, child);
        }
    }

    fn compute_reg_needs(&mut self, arena: &Arena, id: NodeId) -> usize {
        let node = arena.get(id);
        let kind = node.kind.clone();
        let kids = node.children.clone();

        let needs: usize = match kind {
            NodeKind::Leaf | NodeKind::DollarSign => 1,

            NodeKind::Unary | NodeKind::Cast | NodeKind::Domain => {
                let child = kids.get(if kids.len() >= 2 { 1 } else { 0 }).copied();
                child
                    .map(|c| self.compute_reg_needs(arena, c))
                    .unwrap_or(1)
                    .max(1)
            }

            NodeKind::Plus | NodeKind::Mult | NodeKind::Bools => {
                if kids.len() == 2 {
                    let ln = self.compute_reg_needs(arena, kids[0]);
                    let rn = self.compute_reg_needs(arena, kids[1]);
                    if ln == rn { ln + 1 } else { ln.max(rn) }
                } else {
                    1
                }
            }

            NodeKind::Assign => {
                if kids.len() >= 2 {
                    self.compute_reg_needs(arena, kids[1]).max(1)
                } else {
                    1
                }
            }

            NodeKind::FunCall => kids
                .iter()
                .skip(1)
                .map(|&c| self.compute_reg_needs(arena, c))
                .max()
                .unwrap_or(1)
                .max(1),

            _ => {
                for &c in &kids {
                    self.compute_reg_needs(arena, c);
                }
                1
            }
        };

        self.reg_needs.insert(id, needs);
        needs
    }

    fn gen_expr(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        is_fp: bool,
    ) -> (Vec<String>, String) {
        let node = arena.get(id).clone();
        let kids = node.children.clone();
        let label = node.label.clone();

        match node.kind {
            NodeKind::Leaf => {
                let reg = if is_fp {
                    self.alloc_fp()
                        .expect("FP register exhausted in leaf — spill logic gap")
                } else {
                    self.alloc_gp()
                        .expect("GP register exhausted in leaf — spill logic gap")
                };
                
                let lexeme = &node.lexeme;

                let instrs = if label.starts_with("intval") {
                    let value = if label.contains(':') {
                        label.split(':').nth(1).unwrap_or("0")
                    } else if !lexeme.is_empty() && lexeme != "intval" {
                        lexeme
                    } else {
                        "0"
                    };
                    vec![format!("LD {} #{}", reg, value)]
                } else if label.starts_with("floatval") {
                    let value = if label.contains(':') {
                        label.split(':').nth(1).unwrap_or("0.0")
                    } else if !lexeme.is_empty() && lexeme != "floatval" {
                        lexeme
                    } else {
                        "0.0"
                    };
                    vec![format!("LD {} #{}", reg, value)]
                } else if label.starts_with("id:") {
                    let name = decode_lex(&node.lexeme);
                    if let Some(&addr) = self.addr_map.get(&name) {
                        vec![format!("LD {} @{}", reg, addr)]
                    } else if let Some(&off) = self.local_offsets.get(&name) {
                        vec![format!("LD {} @{}({})", reg, off, self.fp())]
                    } else {
                        vec![format!("LD {} #0", reg)]
                    }
                } else {
                    vec![format!("LD {} #0", reg)]
                };
                (instrs, reg)
            }

            NodeKind::Plus | NodeKind::Mult | NodeKind::Bools => {
                if kids.len() != 2 {
                    return (vec![], "R0".to_string());
                }

                let l_fp = node_is_float(arena, kids[0], symtable);
                let r_fp = node_is_float(arena, kids[1], symtable);
                let res_fp = l_fp || r_fp;

                let ln = self.reg_needs.get(&kids[0]).copied().unwrap_or(1);
                let rn_val = self.reg_needs.get(&kids[1]).copied().unwrap_or(1);

                let mut instrs = Vec::new();

                let eval_left_first = ln >= rn_val;
                let (fst_kid, fst_fp, snd_kid, snd_fp, snd_needs) = if eval_left_first {
                    (kids[0], l_fp, kids[1], r_fp, rn_val)
                } else {
                    (kids[1], r_fp, kids[0], l_fp, ln)
                };

                let (fst_instrs, fst_reg) = self.gen_expr(arena, fst_kid, symtable, fst_fp);
                instrs.extend(fst_instrs);

                let spill: Option<usize> = if fst_fp == snd_fp {
                    let free_now =
                        if fst_fp { self.free_fp_count() } else { self.free_gp_count() };
                    if snd_needs > free_now {
                        let slot =
                            if fst_fp { self.alloc_spill_fp() } else { self.alloc_spill_gp() };
                        let addr =
                            if fst_fp { self.fp_spill_addr(slot) } else { self.gp_spill_addr(slot) };
                        instrs.push(format!("ST {} @{}", fst_reg, addr));
                        if fst_fp { self.free_fp_reg(&fst_reg); } else { self.free_gp_reg(&fst_reg); }
                        Some(slot)
                    } else {
                        None
                    }
                } else {
                    None
                };

                let (snd_instrs, snd_reg) = self.gen_expr(arena, snd_kid, symtable, snd_fp);
                instrs.extend(snd_instrs);

                let fst_reg = if let Some(slot) = spill {
                    let addr =
                        if fst_fp { self.fp_spill_addr(slot) } else { self.gp_spill_addr(slot) };
                    let reg = if fst_fp {
                        self.alloc_fp()
                            .expect("FP reg available after spill + second-child eval")
                    } else {
                        self.alloc_gp()
                            .expect("GP reg available after spill + second-child eval")
                    };
                    instrs.push(format!("LD {} @{}", reg, addr));
                    if fst_fp { self.free_spill_fp(slot); } else { self.free_spill_gp(slot); }
                    reg
                } else {
                    fst_reg
                };

                let (left_reg, right_reg) = if eval_left_first {
                    (fst_reg.clone(), snd_reg.clone())
                } else {
                    (snd_reg.clone(), fst_reg.clone())
                };

                let mnemonic = match node.kind {
                    NodeKind::Plus => {
                        if label.trim_start_matches("PLUS:") == "-" { "SUB" } else { "ADD" }
                    }
                    NodeKind::Mult => match label.trim_start_matches("MULT:") {
                        "/" => "DIV",
                        "%" => "REM",
                        _ => "MUL",
                    },
                    NodeKind::Bools => match label.trim_start_matches("BOOLS:") {
                        "<"  => "LT",
                        "<=" => "LTE",
                        "==" => "EQ",
                        ">=" => "GTE",
                        ">"  => "GT",
                        "!=" => "NEQ",
                        _    => "LT",
                    },
                    _ => "ADD",
                };

                let result_reg = if fst_fp == res_fp {
                    fst_reg.clone()
                } else if res_fp {
                    self.alloc_fp().expect("FP reg for mixed-type binary op result")
                } else {
                    self.alloc_gp().expect("GP reg for mixed-type binary op result")
                };
                instrs.push(format!("{} {} {} {}", mnemonic, result_reg, left_reg, right_reg));
                if fst_fp == res_fp {
                    if snd_reg != result_reg {
                        if snd_fp { self.free_fp_reg(&snd_reg); } else { self.free_gp_reg(&snd_reg); }
                    }
                } else {
                    if fst_fp { self.free_fp_reg(&fst_reg); } else { self.free_gp_reg(&fst_reg); }
                    if snd_fp { self.free_fp_reg(&snd_reg); } else { self.free_gp_reg(&snd_reg); }
                }
                (instrs, result_reg)
            }

            NodeKind::Unary => {
                let inner = if kids.len() >= 2 {
                    kids[1]
                } else if !kids.is_empty() {
                    kids[0]
                } else {
                    return (vec![], "R0".to_string());
                };
                let inner_fp = node_is_float(arena, inner, symtable);
                let (mut instrs, inner_reg) = self.gen_expr(arena, inner, symtable, inner_fp);

                let result_reg = if inner_fp {
                    self.alloc_fp().expect("FP reg for unary op result")
                } else {
                    self.alloc_gp().expect("GP reg for unary op result")
                };

                let op = label.trim_start_matches("UNARY:");
                match op {
                    "-" => instrs.push(format!("CHS {} {}", result_reg, inner_reg)),
                    "!" => instrs.push(format!("NOT {} {}", result_reg, inner_reg)),
                    "~" => instrs.push(format!("COMPL {} {}", result_reg, inner_reg)),
                    _   => instrs.push(format!("MV {} {}", result_reg, inner_reg)),
                }
                if inner_fp { self.free_fp_reg(&inner_reg); } else { self.free_gp_reg(&inner_reg); }
                (instrs, result_reg)
            }

            NodeKind::Cast => {
                let inner = if kids.len() >= 2 {
                    kids[1]
                } else if !kids.is_empty() {
                    kids[0]
                } else {
                    return (vec![], "R0".to_string());
                };
                let inner_fp = node_is_float(arena, inner, symtable);
                let target_fp = label.contains("float");
                let (mut instrs, inner_reg) = self.gen_expr(arena, inner, symtable, inner_fp);

                let result_reg = if target_fp {
                    self.alloc_fp().expect("FP reg for cast result")
                } else {
                    self.alloc_gp().expect("GP reg for cast result")
                };

                if inner_fp && !target_fp {
                    instrs.push(format!("INT {} {}", result_reg, inner_reg));
                    self.free_fp_reg(&inner_reg);
                } else if !inner_fp && target_fp {
                    instrs.push(format!("FLOAT {} {}", result_reg, inner_reg));
                    self.free_gp_reg(&inner_reg);
                } else {
                    instrs.push(format!("MV {} {}", result_reg, inner_reg));
                    if inner_fp { self.free_fp_reg(&inner_reg); } else { self.free_gp_reg(&inner_reg); }
                }
                (instrs, result_reg)
            }

            NodeKind::Assign => {
                if kids.len() < 2 {
                    return (vec![], "R0".to_string());
                }
                let rhs_fp = node_is_float(arena, kids[1], symtable);
                let (mut instrs, rhs_reg) = self.gen_expr(arena, kids[1], symtable, rhs_fp);
                let name = decode_lex(&arena.get(kids[0]).lexeme);

                if let Some(&addr) = self.addr_map.get(&name) {
                    instrs.push(format!("ST {} @{}", rhs_reg, addr));
                } else if let Some(&off) = self.local_offsets.get(&name) {
                    instrs.push(format!("ST {} @{}({})", rhs_reg, off, self.fp()));
                }
                if rhs_fp { self.free_fp_reg(&rhs_reg); } else { self.free_gp_reg(&rhs_reg); }
                (instrs, rhs_reg)
            }

            NodeKind::Domain => {
                if kids.is_empty() {
                    return (vec![], "R0".to_string());
                }
                self.gen_expr(arena, kids[0], symtable, is_fp)
            }

            NodeKind::FunCall => self.gen_funcall(arena, id, symtable, is_fp),

            _ => (vec![], "R0".to_string()),
        }
    }

    fn gen_funcall(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        is_fp: bool,
    ) -> (Vec<String>, String) {
        let kids = arena.get(id).children.clone();
        if kids.is_empty() {
            return (vec![], "R0".to_string());
        }

        let fname = decode_lex(&arena.get(kids[0]).lexeme);
        let args = &kids[1..];
        let mut instrs = Vec::new();

        for i in 0..self.alloc_rn.min(3) {
            instrs.push(format!("PUSH R{}", i));
        }
        for i in 0..self.alloc_fn.min(3) {
            instrs.push(format!("PUSHF F{}", i));
        }

        for &arg in args.iter().rev() {
            let arg_fp = node_is_float(arena, arg, symtable);
            let (arg_instrs, arg_reg) = self.gen_expr(arena, arg, symtable, arg_fp);
            instrs.extend(arg_instrs);
            if arg_fp {
                instrs.push(format!("PUSHF {}", arg_reg));
                self.free_fp_reg(&arg_reg);
            } else {
                instrs.push(format!("PUSH {}", arg_reg));
                self.free_gp_reg(&arg_reg);
            }
        }

        instrs.push(format!("PUSH {}", self.fp()));
        instrs.push(format!("CALL @{}", fname));
        instrs.push(format!("POP {}", self.fp()));

        for _ in args.iter() {
            instrs.push("POP R0".to_string());
        }

        for i in (0..self.alloc_fn.min(3)).rev() {
            instrs.push(format!("POPF F{}", i));
        }
        for i in (0..self.alloc_rn.min(3)).rev() {
            instrs.push(format!("POP R{}", i));
        }
        let result_reg = if is_fp { "F0".to_string() } else { "R0".to_string() };
        (instrs, result_reg)
    }

    fn gen_stmt(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        in_func: bool,
    ) -> Vec<String> {
        let node = arena.get(id);
        let kind = &node.kind;

        match kind {
            NodeKind::Emit     => self.gen_emit(arena, id, symtable),
            NodeKind::Assign   => self.gen_assign(arena, id, symtable),
            NodeKind::DeclList => self.gen_decl_list(arena, id, symtable),
            NodeKind::Rand     => self.gen_rand(arena, id, symtable),
            NodeKind::If       => self.gen_if(arena, id, symtable, in_func),
            NodeKind::IfElse   => self.gen_ifelse(arena, id, symtable, in_func),
            NodeKind::While    => self.gen_while(arena, id, symtable, in_func),
            NodeKind::DoWhile  => self.gen_dowhile(arena, id, symtable, in_func),
            NodeKind::BraceStmts
            | NodeKind::Steps
            | NodeKind::BStmts
            | NodeKind::BStmt
            | NodeKind::SoloStmt => {
                let mut instrs = Vec::new();
                for &child in &node.children {
                    instrs.extend(self.gen_stmt(arena, child, symtable, in_func));
                }
                instrs
            }
            _ => {
                let mut instrs = Vec::new();
                for &child in &node.children {
                    instrs.extend(self.gen_stmt(arena, child, symtable, in_func));
                }
                instrs
            }
        }
    }

    fn gen_decl_list(&mut self, arena: &Arena, id: NodeId, symtable: &SymbolTable) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 2 {
            return vec![];
        }
        arena
            .get(kids[1])
            .children
            .clone()
            .into_iter()
            .flat_map(|d| self.gen_declid(arena, d, symtable))
            .collect()
    }

    fn gen_declid(&mut self, arena: &Arena, id: NodeId, symtable: &SymbolTable) -> Vec<String> {
        if arena.get(id).kind == NodeKind::Assign {
            self.gen_assign(arena, id, symtable)
        } else {
            vec![]
        }
    }

    fn gen_assign(&mut self, arena: &Arena, id: NodeId, symtable: &SymbolTable) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 2 {
            return vec![];
        }

        let name = decode_lex(&arena.get(kids[0]).lexeme);
        let rhs = kids[1];
        let rhs_node = arena.get(rhs);
        if rhs_node.kind == NodeKind::Leaf && rhs_node.label.starts_with("stringval") {
            return vec![];
        }
        let is_fp = node_is_float(arena, rhs, symtable);
        let (mut instrs, reg) = self.gen_expr(arena, rhs, symtable, is_fp);
        if let Some(&addr) = self.addr_map.get(&name) {
            instrs.push(format!("ST {} @{}", reg, addr));
        } else if let Some(&off) = self.local_offsets.get(&name) {
            instrs.push(format!("ST {} @{}({})", reg, off, self.fp()));
        }
        if is_fp { self.free_fp_reg(&reg); } else { self.free_gp_reg(&reg); }
        instrs
    }

    fn gen_emit(&mut self, arena: &Arena, id: NodeId, symtable: &SymbolTable) -> Vec<String> {
        let node = arena.get(id);
        let kids = node.children.clone();
        if kids.is_empty() {
            return vec![];
        }
        if kids.len() == 3 {
            let id_node = arena.get(kids[0]);
            let name = decode_lex(&id_node.lexeme);
            let (offset_instrs, offset_reg) = self.gen_expr(arena, kids[1], symtable, false);
            let (length_instrs, length_reg) = self.gen_expr(arena, kids[2], symtable, false);
            let mut instrs = Vec::new();
            instrs.extend(offset_instrs);
            instrs.extend(length_instrs);
            if let Some(&addr) = self.addr_map.get(&name) {
                instrs.push(format!("EMIT @{} {} {}", addr, offset_reg, length_reg));
            } else {
                instrs.push(format!(
                    "EMIT @str_{} {} {}",
                    self.temp_counter, offset_reg, length_reg
                ));
            }
            self.free_gp_reg(&offset_reg);
            self.free_gp_reg(&length_reg);
            return instrs;
        }
        let expr_id = kids[0];
        let is_fp = node_is_float(arena, expr_id, symtable);
        let (mut instrs, reg) = self.gen_expr(arena, expr_id, symtable, is_fp);
        instrs.push(format!("EMIT {}", reg));
        if is_fp { self.free_fp_reg(&reg); } else { self.free_gp_reg(&reg); }
        instrs
    }

    fn gen_rand(&mut self, arena: &Arena, id: NodeId, symtable: &SymbolTable) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.is_empty() {
            return vec![];
        }

        let name = decode_lex(&arena.get(kids[0]).lexeme);
        let is_fp = symtable
            .lookup(&name)
            .map(|i| symtable.symbols[i].base_ty == "float")
            .unwrap_or(false);

        if is_fp {
            let r0 = self.alloc_fp().expect("FP reg for rand");
            let mut instrs = vec![format!("RAND {}", r0)];
            if let Some(&addr) = self.addr_map.get(&name) {
                instrs.push(format!("ST {} @{}", r0, addr));
            }
            self.free_fp_reg(&r0);
            return instrs;
        }

        if kids.len() < 3 {
            let r0 = self.alloc_gp().expect("GP reg for rand");
            let mut instrs = vec![format!("RAND {}", r0)];
            if let Some(&addr) = self.addr_map.get(&name) {
                instrs.push(format!("ST {} @{}", r0, addr));
            }
            self.free_gp_reg(&r0);
            return instrs;
        }

        let lo = kids[1];
        let hi = kids[2];

        let (lo_instrs, lo_reg) = self.gen_expr(arena, lo, symtable, false);
        let (hi_instrs, hi_reg) = self.gen_expr(arena, hi, symtable, false);
        let r0 = self.alloc_gp().expect("GP reg for rand result");

        let mut instrs = Vec::new();
        instrs.extend(lo_instrs);
        instrs.extend(hi_instrs);
        instrs.push(format!("RAND {} {} {}", r0, lo_reg, hi_reg));
        self.free_gp_reg(&lo_reg);
        self.free_gp_reg(&hi_reg);
        if let Some(&addr) = self.addr_map.get(&name) {
            instrs.push(format!("ST {} @{}", r0, addr));
        }
        self.free_gp_reg(&r0);
        instrs
    }

    fn gen_if(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        in_func: bool,
    ) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 2 {
            return vec![];
        }

        let pred_fp = node_is_float(arena, kids[0], symtable);
        let (pred_instrs, cond_reg) = self.gen_expr(arena, kids[0], symtable, pred_fp);
        let body_instrs = self.gen_stmt(arena, kids[1], symtable, in_func);
        let body_len = count_instrs(&body_instrs);

        let mut instrs = pred_instrs;
        instrs.push(format!("IFZ {} @{}i(PC)", cond_reg, body_len));
        instrs.extend(body_instrs);
        if pred_fp { self.free_fp_reg(&cond_reg); } else { self.free_gp_reg(&cond_reg); }
        instrs
    }

    fn gen_ifelse(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        in_func: bool,
    ) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 3 {
            return vec![];
        }

        let pred_fp = node_is_float(arena, kids[0], symtable);
        let (pred_instrs, cond_reg) = self.gen_expr(arena, kids[0], symtable, pred_fp);
        let true_instrs  = self.gen_stmt(arena, kids[1], symtable, in_func);
        let false_instrs = self.gen_stmt(arena, kids[2], symtable, in_func);
        let true_len  = count_instrs(&true_instrs);
        let false_len = count_instrs(&false_instrs);

        let mut instrs = pred_instrs;
        instrs.push(format!("IFZ {} @{}i(PC)", cond_reg, true_len + 1));
        instrs.extend(true_instrs);
        instrs.push(format!("JUMP @{}i(PC)", false_len));
        instrs.extend(false_instrs);
        if pred_fp { self.free_fp_reg(&cond_reg); } else { self.free_gp_reg(&cond_reg); }
        instrs
    }

    fn gen_while(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        in_func: bool,
    ) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 2 {
            return vec![];
        }

        let pred_fp = node_is_float(arena, kids[0], symtable);
        let (pred_instrs, cond_reg) = self.gen_expr(arena, kids[0], symtable, pred_fp);
        let body_instrs = self.gen_stmt(arena, kids[1], symtable, in_func);
        let pred_len = count_instrs(&pred_instrs);
        let body_len = count_instrs(&body_instrs);

        let mut instrs = pred_instrs;
        instrs.push(format!("IFZ {} @{}i(PC)", cond_reg, body_len + 1));
        instrs.extend(body_instrs);
        instrs.push(format!("JUMP @-{}i(PC)", pred_len + body_len + 2));
        if pred_fp { self.free_fp_reg(&cond_reg); } else { self.free_gp_reg(&cond_reg); }
        instrs
    }

    fn gen_dowhile(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
        in_func: bool,
    ) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 2 {
            return vec![];
        }

        let body_instrs = self.gen_stmt(arena, kids[0], symtable, in_func);
        let pred_fp = node_is_float(arena, kids[1], symtable);
        let (pred_instrs, cond_reg) = self.gen_expr(arena, kids[1], symtable, pred_fp);
        let body_len = count_instrs(&body_instrs);
        let pred_len = count_instrs(&pred_instrs);

        let mut instrs = body_instrs;
        instrs.extend(pred_instrs);
        instrs.push(format!("IFNZ {} @-{}i(PC)", cond_reg, body_len + pred_len + 1));
        if pred_fp { self.free_fp_reg(&cond_reg); } else { self.free_gp_reg(&cond_reg); }
        instrs
    }

    fn gen_global_instrs(
        &mut self,
        arena: &Arena,
        root: NodeId,
        symtable: &SymbolTable,
    ) -> Vec<String> {
        let root_node = arena.get(root);
        let mut instrs = Vec::new();

        for &child in &root_node.children {
            let child_node = arena.get(child);
            match child_node.kind {
                NodeKind::Module | NodeKind::ModParts => {
                    for &grandchild in &child_node.children {
                        instrs.extend(self.gen_stmt(arena, grandchild, symtable, false));
                    }
                }
                _ => {
                    instrs.extend(self.gen_stmt(arena, child, symtable, false));
                }
            }
        }

        instrs
    }

    fn gen_func_instrs(
        &mut self,
        arena: &Arena,
        root: NodeId,
        symtable: &SymbolTable,
    ) -> Vec<String> {
        arena
            .get(root)
            .children
            .clone()
            .into_iter()
            .filter(|&c| arena.get(c).kind == NodeKind::FuncDef)
            .flat_map(|c| self.gen_funcdef(arena, c, symtable))
            .collect()
    }

    fn gen_funcdef(
        &mut self,
        arena: &Arena,
        id: NodeId,
        symtable: &SymbolTable,
    ) -> Vec<String> {
        let kids = arena.get(id).children.clone();
        if kids.len() < 5 {
            return vec![];
        }

        let sig_kids = arena.get(kids[0]).children.clone();
        if sig_kids.len() < 2 {
            return vec![];
        }

        let fname = decode_lex(&arena.get(sig_kids[1]).lexeme);
        let ret_expr = kids[3];
        let body = kids[4];

        let mut local_ctx = self.clone_for_func();
        let mut param_offset: i32 = 4;
        if sig_kids.len() >= 3 {
            let params = arena.get(sig_kids[2]).children.clone();
            for &p in &params {
                let pk = arena.get(p).children.clone();
                if pk.len() >= 2 {
                    let pname = decode_lex(&arena.get(pk[1]).lexeme);
                    local_ctx.local_offsets.insert(pname, param_offset);
                    param_offset += 4;
                }
            }
        }

        let mut instrs = Vec::new();

        instrs.push(format!("label @{} {}", fname, fname));
        instrs.push(format!("PUSH {}", self.w0()));
        instrs.push(format!("PUSH {}", self.w1()));
        instrs.push(format!("PUSH RA"));
        instrs.push(format!("MOV {} {}", self.fp(), self.sp()));

        instrs.extend(local_ctx.gen_stmt(arena, body, symtable, true));

        let ret_fp = node_is_float(arena, ret_expr, symtable);
        let (ret_instrs, ret_reg) = local_ctx.gen_expr(arena, ret_expr, symtable, ret_fp);
        instrs.extend(ret_instrs);

        if ret_fp {
            instrs.push(format!("PUSHF {}", ret_reg));
        } else {
            instrs.push(format!("PUSH {}", ret_reg));
        }

        instrs.push("POP RA".to_string());
        instrs.push(format!("POP {}", self.w1()));
        instrs.push(format!("POP {}", self.w0()));
        instrs.push("RETURN".to_string());

        instrs
    }

    fn clone_for_func(&self) -> CgenCtx {
        CgenCtx {
            rn: self.rn,
            fn_regs: self.fn_regs,
            alloc_rn: self.alloc_rn,
            alloc_fn: self.alloc_fn,
            addr_map: self.addr_map.clone(),
            string_literals: self.string_literals.clone(),
            string_values: self.string_values.clone(),
            data_size: self.data_size,
            reg_needs: self.reg_needs.clone(),
            local_offsets: HashMap::new(),
            next_local_offset: -4,
            free_gp: vec![true; self.alloc_rn],
            free_fp: vec![true; self.alloc_fn],
            temp_counter: 0,
            spill_base: self.spill_base,
            gp_spill_slots: vec![true; MAX_SPILL_SLOTS],
            fp_spill_slots: vec![true; MAX_SPILL_SLOTS],
        }
    }

    fn write_czr(
        &self,
        path: &str,
        symtable: &SymbolTable,
        init_instrs: &[String],
        func_instrs: &[String],
    ) -> io::Result<()> {
        let mut f = fs::File::create(path)?;

        for (name, &addr) in &self.string_literals {
            if let Some(value) = self.string_values.get(name) {
                let escaped = value.replace(' ', "x20");
                writeln!(f, "data @{} :{}:", addr, escaped)?;
            }
        }

        {
            let mut globals: Vec<_> = symtable
                .symbols
                .iter()
                .filter(|s| s.scope == 0 && !s.is_function())
                .collect();
            globals.sort_by_key(|s| self.addr_map.get(&s.name).copied().unwrap_or(usize::MAX));

            for sym in &globals {
                if let Some(&addr) = self.addr_map.get(&sym.name) {
                    writeln!(f, "label @{} {}", addr, sym.name)?;
                }
            }
        }

        writeln!(f)?;

        let init_base = align_up(self.data_size, 4);
        writeln!(f, "init @{}", init_base)?;

        for instr in init_instrs {
            writeln!(f, "    {}", instr)?;
        }
        writeln!(f, "    return")?;

        if !func_instrs.is_empty() {
            writeln!(f)?;
            for instr in func_instrs {
                if instr.trim_start().starts_with("label ") {
                    writeln!(f, "{}", instr.trim())?;
                } else {
                    writeln!(f, "    {}", instr)?;
                }
            }
        }

        Ok(())
    }
}

fn type_size_align(base_ty: &str) -> (usize, usize) {
    match base_ty {
        "int" | "float" | "bool" => (4, 4),
        "string" => (0, 1),
        _ => (4, 4),
    }
}

fn node_is_float(arena: &Arena, id: NodeId, symtable: &SymbolTable) -> bool {
    let node = arena.get(id);
    match node.kind {
        NodeKind::Leaf => {
            let lab = &node.label;
            if lab.starts_with("floatval") || lab.starts_with("floatlit") {
                return true;
            }
            if lab.starts_with("id:") {
                let name = decode_lex(&node.lexeme);
                if let Some(i) = symtable.lookup(&name) {
                    return symtable.symbols[i].base_ty == "float";
                }
            }
            false
        }
        NodeKind::Plus | NodeKind::Mult => {
            let kids = &node.children;
            kids.len() == 2
                && (node_is_float(arena, kids[0], symtable)
                    || node_is_float(arena, kids[1], symtable))
        }
        NodeKind::Unary => {
            let kids = &node.children;
            if kids.len() >= 2 {
                node_is_float(arena, kids[1], symtable)
            } else if !kids.is_empty() {
                node_is_float(arena, kids[0], symtable)
            } else {
                false
            }
        }
        NodeKind::Cast => node.label.contains("float"),
        NodeKind::FunCall => {
            let kids = &node.children;
            if !kids.is_empty() {
                let name = decode_lex(&arena.get(kids[0]).lexeme);
                if let Some(i) = symtable.lookup(&name) {
                    return symtable.symbols[i].base_ty == "float";
                }
            }
            false
        }
        NodeKind::Domain => {
            let kids = &node.children;
            if !kids.is_empty() { node_is_float(arena, kids[0], symtable) } else { false }
        }
        NodeKind::Bools => false,
        _ => false,
    }
}

fn count_instrs(instrs: &[String]) -> usize {
    instrs
        .iter()
        .filter(|s| {
            let t = s.trim();
            !t.is_empty() && !t.starts_with(';') && !t.starts_with("label ")
        })
        .count()
}

fn align_up(val: usize, align: usize) -> usize {
    if align == 0 || val % align == 0 { val } else { val + align - val % align }
}

fn decode_lex(s: &str) -> String {
    let bytes = s.as_bytes();
    let mut out = String::new();
    let mut i = 0;
    while i < bytes.len() {
        if bytes[i] == b'x' && i + 2 < bytes.len() {
            let h1 = bytes[i + 1];
            let h2 = bytes[i + 2];
            if h1.is_ascii_hexdigit() && h2.is_ascii_hexdigit() {
                let v = (hex_nibble(h1) << 4) | hex_nibble(h2);
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

fn hex_nibble(b: u8) -> u8 {
    match b {
        b'0'..=b'9' => b - b'0',
        b'a'..=b'f' => b - b'a' + 10,
        b'A'..=b'F' => b - b'A' + 10,
        _ => 0,
    }
}