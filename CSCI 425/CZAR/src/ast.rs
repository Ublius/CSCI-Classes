use std::collections::HashMap;
use std::fs;
use std::io::{self, Write};
 
 
#[derive(Debug, Clone, PartialEq)]
pub enum NodeKind {
    Module,
    ModParts,
    DeclList,
    DeclIds,
    DeclId,
    FuncDef,
    FunSig,
    ParamList,
    Param,
    Steps,
    BraceStmts,
    BStmts,
    BStmt,
    SoloStmt,
    If,
    IfElse,
    While,
    DoWhile,
    Assign,
    Emit,
    EmitSymtable,
    Rand,
    Bools,
    Plus,
    Mult,
    Mod,
    Unary,
    Cast,
    Domain,
    FunCall,
    ArgList,
    Leaf,
    DollarSign,
}
 
 
#[derive(Debug, Clone, PartialEq)]
pub enum Type {
    Int,
    Float,
    Bool,
    Str,
    Fun { ret: Box<Type>, params: Vec<Type> },
    Error,
}
 
impl Type {
    pub fn name(&self) -> &'static str {
        match self {
            Type::Int         => "int",
            Type::Float       => "float",
            Type::Bool        => "bool",
            Type::Str         => "string",
            Type::Fun { .. }  => "function",
            Type::Error       => "<error>",
        }
    }
    pub fn from_str(s: &str) -> Option<Type> {
        match s {
            "int"    => Some(Type::Int),
            "float"  => Some(Type::Float),
            "bool"   => Some(Type::Bool),
            "string" => Some(Type::Str),
            _        => None,
        }
    }
    pub fn is_error(&self) -> bool { *self == Type::Error }
}
 
 
pub type NodeId = usize;
 
#[derive(Debug, Clone)]
pub struct AstNode {
    pub id:       NodeId,
    pub kind:     NodeKind,
    pub label:    String,
    pub children: Vec<NodeId>,
    pub line:     usize,
    pub col:      usize,
    pub lexeme:   String,
    pub ty:       Option<Type>,
}
 
impl AstNode {
    pub fn new_leaf(id: NodeId, label: &str, lexeme: &str, line: usize, col: usize) -> Self {
        let kind = if label == "$" || lexeme == "$" { NodeKind::DollarSign } else { NodeKind::Leaf };
        AstNode { id, kind, label: label.to_string(), children: vec![], line, col, lexeme: lexeme.to_string(), ty: None }
    }
 
    pub fn new_node(id: NodeId, kind: NodeKind, label: &str, line: usize, col: usize) -> Self {
        AstNode { id, kind, label: label.to_string(), children: vec![], line, col, lexeme: String::new(), ty: None }
    }
}
 
 
pub struct Arena {
    nodes: Vec<AstNode>,
}
 
impl Arena {
    pub fn new() -> Self { Arena { nodes: Vec::new() } }
 
    pub fn alloc(&mut self, mut node: AstNode) -> NodeId {
        let id = self.nodes.len();
        node.id = id;
        self.nodes.push(node);
        id
    }
 
    pub fn get(&self, id: NodeId) -> &AstNode { &self.nodes[id] }
    pub fn get_mut(&mut self, id: NodeId) -> &mut AstNode { &mut self.nodes[id] }
 
    pub fn set_ty(&mut self, id: NodeId, ty: Type) { self.nodes[id].ty = Some(ty); }
 
    pub fn len(&self) -> usize { self.nodes.len() }
}
 
 
pub fn compute_path_ids(arena: &Arena, root: NodeId) -> HashMap<NodeId, String> {
    let mut map = HashMap::new();
    assign_paths(arena, root, "0", &mut map);
    map
}
 
fn assign_paths(arena: &Arena, id: NodeId, path: &str, map: &mut HashMap<NodeId, String>) {
    map.insert(id, path.to_string());
    let children = arena.get(id).children.clone();
    for (i, child) in children.into_iter().enumerate() {
        let child_path = format!("{}-{}", path, i);
        assign_paths(arena, child, &child_path, map);
    }
}
  
pub fn write_ast_file(arena: &Arena, root: NodeId, path: &str) -> io::Result<()> {
    let path_ids = compute_path_ids(arena, root);
    let mut part1: Vec<String> = Vec::new();
    let mut part2: Vec<String> = Vec::new();
    collect_nodes(arena, root, &path_ids, &mut part1, &mut part2);
 
    let mut out = fs::File::create(path)?;
    for l in &part1 { writeln!(out, "{}", l)?; }
    writeln!(out)?;
    for l in &part2 { writeln!(out, "{}", l)?; }
    Ok(())
}
 
fn collect_nodes(
    arena:    &Arena,
    id:       NodeId,
    path_ids: &HashMap<NodeId, String>,
    part1:    &mut Vec<String>,
    part2:    &mut Vec<String>,
) {
    let node    = arena.get(id);
    let my_path = &path_ids[&id];
 
    part1.push(format!("{} {}", my_path, node.label));
 
    if !node.children.is_empty() {
        let child_paths: Vec<&str> = node.children.iter()
            .map(|c| path_ids[c].as_str())
            .collect();
        part2.push(format!("{} {}", my_path, child_paths.join(" ")));
    }
 
    let children = node.children.clone();
    for child in children {
        collect_nodes(arena, child, path_ids, part1, part2);
    }
}
