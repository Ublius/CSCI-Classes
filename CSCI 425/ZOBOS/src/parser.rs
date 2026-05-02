use std::collections::VecDeque;

use crate::ast::{Arena, AstNode, NodeId, NodeKind};
use crate::lr_table::{LRAction, LRData};
use crate::token::Token;

#[derive(Debug)]
pub struct ParseError {
    pub line: usize,
    pub col: usize,
    pub detail: String,
}

pub struct ParseResult {
    pub root: NodeId,
    pub arena: Arena,
}

struct Entry {
    state: usize,
    node_id: NodeId,
}

pub fn parse(tokens: &[Token], lr: &LRData) -> Result<ParseResult, ParseError> {
    let mut arena = Arena::new();

    let mut input: VecDeque<Token> = VecDeque::from(tokens.to_vec());

    let mut stack: Vec<Entry> = Vec::new();
    stack.push(Entry {
        state: 0,
        node_id: 0,
    });

    let mut last_line = 1;
    let mut last_col = 1;

    let mut symbol = input.pop_front();

    loop {
        let state = stack.last().unwrap().state;

        let current_symbol = match symbol {
            Some(ref tok) => tok.clone(),
            None => Token {
                line: last_line,
                col: last_col,
                kind: "$".to_string(),
                lexeme: "$".to_string(),
                node_id: None,
            },
        };

        if current_symbol.line > 0 {
            last_line = current_symbol.line;
            last_col = current_symbol.col;
        }

        let action = lr.lookup(state, &current_symbol.kind);

        match action {
            LRAction::Shift(new_state) => {
                let node_id = if let Some(id) = current_symbol.node_id {
                    id
                } else {
                    let mut node = AstNode::new_leaf(
                        arena.len(),
                        &current_symbol.kind,
                        &current_symbol.lexeme,
                        current_symbol.line,
                        current_symbol.col,
                    );
                    node.label = format!("{}:{}", current_symbol.kind, current_symbol.lexeme);
                    arena.alloc(node)
                };

                stack.push(Entry {
                    state: new_state,
                    node_id,
                });

                symbol = input.pop_front();
            }

            LRAction::Reduce(rule_idx) => {
                let rule = &lr.rules[rule_idx];
                let rhs_len = rule.rhs_len();

                let mut children: Vec<NodeId> = Vec::new();
                for _ in 0..rhs_len {
                    if let Some(entry) = stack.pop() {
                        children.push(entry.node_id);
                    }
                }
                children.reverse();

                let parent = sdt_reduce(&mut arena, rule_idx, &rule.lhs, &children);
                let parent_id = arena.alloc(parent);

                let top_state = stack.last().unwrap().state;

                let goto = lr.lookup(top_state, &rule.lhs);

                match goto {
                    LRAction::Shift(new_state) => {
                        stack.push(Entry {
                            state: new_state,
                            node_id: parent_id,
                        });
                    }
                    _ => {
                        return Err(ParseError {
                            line: last_line,
                            col: last_col,
                            detail: format!("Missing GOTO for state {} on {}", top_state, rule.lhs),
                        });
                    }
                }
            }

            LRAction::ReduceAccept(rule_idx) => {
                let rule = &lr.rules[rule_idx];
                let rhs_len = rule.rhs_len();

                let mut children: Vec<NodeId> = Vec::new();
                for _ in 0..rhs_len {
                    if let Some(entry) = stack.pop() {
                        children.push(entry.node_id);
                    }
                }
                children.reverse();

                let root = sdt_reduce(&mut arena, rule_idx, &rule.lhs, &children);
                let root_id = arena.alloc(root);

                return Ok(ParseResult {
                    root: root_id,
                    arena,
                });
            }

            LRAction::Error => {
                let expected = lr.expected(state);
                return Err(ParseError {
                    line: if current_symbol.line > 0 {
                        current_symbol.line
                    } else {
                        last_line
                    },
                    col: if current_symbol.col > 0 {
                        current_symbol.col
                    } else {
                        last_col
                    },
                    detail: format!(
                        "Parsing error; input: {}; expected={}",
                        current_symbol.kind,
                        if expected.is_empty() {
                            "?".into()
                        } else {
                            expected.join("|")
                        }
                    ),
                });
            }
        }
    }
}

fn sdt_reduce(arena: &mut Arena, _rule_idx: usize, lhs: &str, children: &[NodeId]) -> AstNode {
    let (line, col) = first_loc(arena, children);

    match lhs {
        "MODULE" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| arena.get(c).kind != NodeKind::DollarSign)
                .collect();
            if kept.len() == 1 {
                let mod_parts = arena.get(kept[0]).clone();
                let mut n = mk_node(NodeKind::Module, "MODULE", line, col);
                n.children = mod_parts.children.clone();
                n
            } else {
                mk_children(NodeKind::Module, "MODULE", line, col, &kept)
            }
        }

        "MODPARTS" => {
            let mut list: Vec<NodeId> = Vec::new();
            for &c in children {
                let kind = arena.get(c).kind.clone();
                match kind {
                    NodeKind::ModParts => {
                        list.extend(arena.get(c).children.clone());
                    }
                    NodeKind::FuncDef => {
                        list.push(c);
                    }
                    _ if is_term(arena, c, "sc") => { /* drop */ }
                    NodeKind::DollarSign => { /* drop */ }
                    _ => list.push(c),
                }
            }
            if list.is_empty() {
                return mk_node(NodeKind::ModParts, "MODPARTS", line, col);
            }
            mk_children(NodeKind::ModParts, "MODPARTS", line, col, &list)
        }

        "FUNTYPE" => {
            let c = children[0];
            let lex = arena.get(c).lexeme.clone();
            AstNode::new_leaf(0, &format!("type:{}", lex), &lex, line, col)
        }

        "OTHERTYPE" => {
            let label: String = children
                .iter()
                .map(|&c| arena.get(c).lexeme.clone())
                .collect::<Vec<_>>()
                .join(" ");
            AstNode::new_leaf(0, &format!("type:{}", label), &label, line, col)
        }

        "GOTDECLLIST" | "GFTDECLLIST" => mk_children(NodeKind::DeclList, lhs, line, col, children),

        "DECLTYPE" => pass_through(arena, children),
        "DECLID" => pass_through(arena, children),

        "DECLIDS" => {
            let mut list: Vec<NodeId> = Vec::new();
            for &c in children {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::DeclIds {
                    list.extend(arena.get(c).children.clone());
                } else if !is_term(arena, c, "comma") {
                    list.push(c);
                }
            }
            mk_children(NodeKind::DeclIds, "DECLIDS", line, col, &list)
        }

        "DECLLIST" => mk_children(NodeKind::DeclList, "DECLLIST", line, col, children),

        "ASSIGN" => {
            let mut assign_line = line;
            let mut assign_col = col;
            for &c in children {
                if is_term(arena, c, "assign") {
                    let n = arena.get(c);
                    assign_line = n.line;
                    assign_col = n.col;
                    break;
                }
            }
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "assign"))
                .collect();
            if !kept.is_empty() {
                let id_node = arena.get(kept[0]);
                let id_lex = id_node.lexeme.clone();
                let id_line = id_node.line;
                let id_col = id_node.col;
                let id_leaf =
                    AstNode::new_leaf(0, &format!("id:{}", id_lex), &id_lex, id_line, id_col);
                let id_nid = arena.alloc(id_leaf);
                let rhs = if kept.len() >= 2 { kept[1] } else { kept[0] };
                let mut n = mk_node(NodeKind::Assign, "=", assign_line, assign_col);
                n.children = vec![id_nid, rhs];
                n
            } else {
                mk_children(NodeKind::Assign, "=", assign_line, assign_col, children)
            }
        }

        "FUNSIG" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "lparen") && !is_term(arena, c, "rparen"))
                .collect();
            mk_children(NodeKind::FunSig, "FUNSIG", line, col, &kept)
        }

        "PARAMLIST" => {
            if children.is_empty() {
                return mk_node(NodeKind::ParamList, "PARAMLIST", line, col);
            }
            let mut params: Vec<NodeId> = Vec::new();
            let mut ty: Option<NodeId> = None;
            for &c in children {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::ParamList {
                    params.extend(arena.get(c).children.clone());
                } else if is_term(arena, c, "comma") {
                } else if is_type_node(arena, c) {
                    ty = Some(c);
                } else {
                    if let Some(ty_c) = ty.take() {
                        let id_lex = arena.get(c).lexeme.clone();
                        let id_lin = arena.get(c).line;
                        let id_col = arena.get(c).col;
                        let id_n = arena.alloc(AstNode::new_leaf(
                            0,
                            &format!("id:{}", id_lex),
                            &id_lex,
                            id_lin,
                            id_col,
                        ));
                        let mut param = mk_node(NodeKind::Param, "PARAM", id_lin, id_col);
                        param.children = vec![ty_c, id_n];
                        let pid = arena.alloc(param);
                        params.push(pid);
                    } else {
                        params.push(c);
                    }
                }
            }
            mk_children(NodeKind::ParamList, "PARAMLIST", line, col, &params)
        }

        "FUNCTION" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "returns"))
                .collect();
            mk_children(NodeKind::FuncDef, "FUNCTION", line, col, &kept)
        }

        "BRACESTMTS" => {
            let mut stmts: Vec<NodeId> = Vec::new();
            for &c in children {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::BStmts {
                    stmts.extend(arena.get(c).children.clone());
                } else if !is_term(arena, c, "lbrace") && !is_term(arena, c, "rbrace") {
                    stmts.push(c);
                }
            }
            mk_children(NodeKind::BraceStmts, "BRACESTMTS", line, col, &stmts)
        }

        "BSTMTS" => {
            if children.is_empty() {
                return mk_node(NodeKind::BStmts, "BSTMTS", line, col);
            }
            let mut list: Vec<NodeId> = Vec::new();
            for &c in children {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::BStmts {
                    list.extend(arena.get(c).children.clone());
                } else {
                    list.push(c);
                }
            }
            mk_children(NodeKind::BStmts, "BSTMTS", line, col, &list)
        }

        "BSTMT" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "sc"))
                .collect();
            if kept.len() == 1 {
                return arena.get(kept[0]).clone();
            }
            mk_children(NodeKind::BStmt, "BSTMT", line, col, &kept)
        }

        "SOLOSTMT" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "sc"))
                .collect();
            if kept.len() == 1 {
                return arena.get(kept[0]).clone();
            }
            mk_children(NodeKind::SoloStmt, "SOLOSTMT", line, col, &kept)
        }

        "STEPS" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "sc"))
                .collect();
            if kept.len() == 1 {
                return arena.get(kept[0]).clone();
            }
            if kept.is_empty() {
                return mk_node(NodeKind::Steps, "STEPS", line, col);
            }
            mk_children(NodeKind::Steps, "STEPS", line, col, &kept)
        }

        "IF" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| {
                    !is_term(arena, c, "if")
                        && !is_term(arena, c, "lparen")
                        && !is_term(arena, c, "rparen")
                })
                .collect();
            mk_children(NodeKind::If, "IF", line, col, &kept)
        }

        "IFELSE" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| {
                    !is_term(arena, c, "if")
                        && !is_term(arena, c, "else")
                        && !is_term(arena, c, "lparen")
                        && !is_term(arena, c, "rparen")
                })
                .collect();
            mk_children(NodeKind::IfElse, "IFELSE", line, col, &kept)
        }

        "WHILE" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| {
                    !is_term(arena, c, "while")
                        && !is_term(arena, c, "lparen")
                        && !is_term(arena, c, "rparen")
                })
                .collect();
            mk_children(NodeKind::While, "WHILE", line, col, &kept)
        }

        "DOWHILE" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| {
                    !is_term(arena, c, "do")
                        && !is_term(arena, c, "while")
                        && !is_term(arena, c, "lparen")
                        && !is_term(arena, c, "rparen")
                        && !is_term(arena, c, "sc")
                })
                .collect();
            mk_children(NodeKind::DoWhile, "DOWHILE", line, col, &kept)
        }

        "EXPR" | "AEXPR" => pass_through(arena, children),

        "BEXPR" => {
            if children.len() == 3 {
                let bools_node = arena.get(children[1]);
                let op_lex = bools_node.lexeme.clone();
                let bline = bools_node.line;
                let bcol = bools_node.col;
                let mut n = mk_node(NodeKind::Bools, &format!("BOOLS:{}", op_lex), bline, bcol);
                n.children = vec![children[0], children[2]];
                n
            } else {
                pass_through(arena, children)
            }
        }

        "BOOLS" => pass_through(arena, children),

        "SUM" => {
            if children.len() == 1 {
                return pass_through(arena, children);
            }
            let op = arena.get(children[1]);
            let op_lex = op.lexeme.clone();
            let oline = op.line;
            let ocol = op.col;
            let mut n = mk_node(NodeKind::Plus, &format!("PLUS:{}", op_lex), oline, ocol);
            n.children = vec![children[0], children[2]];
            n
        }

        "PLUS" => pass_through(arena, children),

        "PRODUCT" => {
            if children.len() == 1 {
                return pass_through(arena, children);
            }
            let op = arena.get(children[1]);
            let op_lex = op.lexeme.clone();
            let oline = op.line;
            let ocol = op.col;
            let mut n = mk_node(NodeKind::Mult, &format!("MULT:{}", op_lex), oline, ocol);
            n.children = vec![children[0], children[2]];
            n
        }

        "TIMES" => pass_through(arena, children),

        "VALUE" => match children.len() {
            1 => pass_through(arena, children),
            3 => arena.get(children[1]).clone(),
            4 => {
                let dom = arena.get(children[0]);
                let (dline, dcol) = (dom.line, dom.col);
                let expr = children[2];
                let mut n = mk_node(NodeKind::Domain, "DOMAIN", dline, dcol);
                n.children = vec![expr];
                n
            }
            _ => pass_through(arena, children),
        },

        "UNARY" => {
            if children.len() == 2 {
                let op = arena.get(children[0]);
                let op_lex = op.lexeme.clone();
                let oline = op.line;
                let ocol = op.col;
                let mut n = mk_node(NodeKind::Unary, &format!("UNARY:{}", op_lex), oline, ocol);
                n.children = vec![children[0], children[1]];
                n
            } else {
                pass_through(arena, children)
            }
        }

        "CAST" => {
            let type_leaf = arena.get(children[0]);
            let ty_lex = type_leaf.lexeme.clone();
            let tline = type_leaf.line;
            let tcol = type_leaf.col;
            let expr = children[2];
            let mut n = mk_node(NodeKind::Cast, &format!("CAST:{}", ty_lex), tline, tcol);
            n.children = vec![children[0], expr];
            n
        }

        "FUNCALL" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "lparen") && !is_term(arena, c, "rparen"))
                .collect();
            let mut fchildren: Vec<NodeId> = Vec::new();
            for &c in &kept {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::ArgList {
                    fchildren.extend(arena.get(c).children.clone());
                } else {
                    let lex = arena.get(c).lexeme.clone();
                    let cl = arena.get(c).line;
                    let cc = arena.get(c).col;
                    let id_n =
                        arena.alloc(AstNode::new_leaf(0, &format!("id:{}", lex), &lex, cl, cc));
                    fchildren.push(id_n);
                }
            }
            mk_children(NodeKind::FunCall, "FUNCALL", line, col, &fchildren)
        }

        "ARGLIST" => {
            if children.is_empty() {
                return mk_node(NodeKind::ArgList, "ARGLIST", line, col);
            }
            let mut list: Vec<NodeId> = Vec::new();
            for &c in children {
                let kind = arena.get(c).kind.clone();
                if kind == NodeKind::ArgList {
                    list.extend(arena.get(c).children.clone());
                } else if !is_term(arena, c, "comma") {
                    list.push(c);
                }
            }
            mk_children(NodeKind::ArgList, "ARGLIST", line, col, &list)
        }

        "EMIT" => {
            let has_symtable = children.iter().any(|&c| is_term(arena, c, "symtable"));
            if has_symtable {
                return mk_node(NodeKind::EmitSymtable, "EMITSYMTABLE", line, col);
            }
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| {
                    !is_term(arena, c, "emit")
                        && !is_term(arena, c, "comma")
                        && !is_term(arena, c, "lparen")
                        && !is_term(arena, c, "rparen")
                })
                .collect();
            mk_children(NodeKind::Emit, "EMIT", line, col, &kept)
        }

        "RAND" => {
            let kept: Vec<NodeId> = children
                .iter()
                .copied()
                .filter(|&c| !is_term(arena, c, "rand") && !is_term(arena, c, "comma"))
                .collect();
            mk_children(NodeKind::Rand, "RAND", line, col, &kept)
        }

        _ => {
            if children.len() == 1 {
                return arena.get(children[0]).clone();
            }
            mk_children(NodeKind::Leaf, lhs, line, col, children)
        }
    }
}

fn first_loc(arena: &Arena, children: &[NodeId]) -> (usize, usize) {
    children
        .first()
        .map(|&c| {
            let n = arena.get(c);
            (n.line, n.col)
        })
        .unwrap_or((0, 0))
}

fn mk_node(kind: NodeKind, label: &str, line: usize, col: usize) -> AstNode {
    AstNode::new_node(0, kind, label, line, col)
}

fn mk_children(
    kind: NodeKind,
    label: &str,
    line: usize,
    col: usize,
    children: &[NodeId],
) -> AstNode {
    let mut n = AstNode::new_node(0, kind, label, line, col);
    n.children = children.to_vec();
    n
}

fn pass_through(arena: &mut Arena, children: &[NodeId]) -> AstNode {
    if children.len() == 1 {
        arena.get(children[0]).clone()
    } else {
        let (line, col) = first_loc(arena, children);
        mk_children(NodeKind::Leaf, "?", line, col, children)
    }
}

fn is_term(arena: &Arena, id: NodeId, name: &str) -> bool {
    if id == usize::MAX {
        return false;
    }
    let node = arena.get(id);
    node.label.starts_with(name) || node.lexeme == name
}

fn is_type_node(arena: &Arena, id: NodeId) -> bool {
    let lab = &arena.get(id).label;
    lab.starts_with("type:") || matches!(lab.as_str(), "bool" | "int" | "float" | "string")
}
