open Javascript_ast
open Javascript_heap
open Javascript_main
open Util

(*
 * Check javascript_ast.ml for the following useful functionality:
 * - str_float               -- convert a float to a string
 * - to_num, to_bool, to_str -- do the JavaScript automatic type conversion
 * - read_environment        -- look up a variable's value in the environment
 * - bind_environment        -- add a variable binding to the environment
 * - empty_env               -- the empty environment
 *)

(*
 * Check javascript_heap.ml for the following useful functionality:
 * -  heap_alloc             -- allocate a new reference cell
 * -  heap_assign            -- update the value in a reference cell
 * -  heap_deref             -- retrieve the value from a reference
 *)

exception UnboundVariable   of string
exception ImmutableVar      of string
exception InvalidCall       of expr_t
exception UnimplementedExpr of expr_t
exception RefError of expr_t
(* evaluate a program *)
let rec eval (env : environment_t) (h:heap_t) (p: program_t) : value_t*heap_t =
  match p with
  | ExprProgram(_, e) -> eval_expr env h e
  | StmtProgram(_, s, p1) ->
      let env', h' = eval_stmt env h s in
      eval env' h' p1

and eval_block (env : environment_t) (h : heap_t) (b : block_t) : value_t * heap_t =
  match b with
  | ReturnBlock(_, e) ->
      (* Return expression evaluates on current heap *)
      eval_expr env h e
  | StmtBlock(_, s, b1) ->
      (* Evaluate statement, then the rest of the block *)
      let env', h1 = eval_stmt env h s in
      eval_block env' h1 b1


and to_address = function
  | NumVal n -> int_of_float n
  | v        -> raise (Invalid_argument
                         ("heap address must be a number, got "
                          ^ str_value v))

  (* evaluate a single statement *)
and eval_stmt (env : environment_t) (h : heap_t) (s : stmt_t) : environment_t * heap_t =
  match s with
  | ConstStmt(_, x, e) ->
      let v, h' = eval_expr env h e in
      (bind_environment env x Immutable v, h')
  | LetStmt(_, x, e) ->
      (* 1. Evaluate initializer expression *)
      let v, h1 = eval_expr env h e in
      (* 2. Allocate new mutable cell in heap *)
      let ref_val, h2 = heap_alloc h1 v in
      (* 3. Bind x to that reference *)
      (bind_environment env x Mutable ref_val, h2)
  | AssignStmt(_, lhs, e) ->
    (* 1. evaluate RHS *)
    let v_rhs, h1 = eval_expr env h e in

    (* 2. easy case: lhs is a plain variable *)
    (match lhs with
     | VarExpr(_, name) ->
         (match read_environment env name with
          | Some (Mutable , RefVal addr) ->
              let h2 = heap_assign h1 addr v_rhs in
              (env, h2)
          | Some (Immutable, _) -> raise (ImmutableVar name)
          | Some (Mutable , _)  -> raise (Failure "mutable var not a RefVal")
          | None                -> raise (UnboundVariable name))

     | _ ->
         (* 3. general case: evaluate LHS expression *)
         let lhs_val, h2 = eval_expr env h1 lhs in
         (match lhs_val with
          | RefVal addr ->
              let h3 = heap_assign h2 addr v_rhs in
              (env, h3)

          | _ ->
              (* normalise position so tests match *)
              let err_expr =
                match lhs with
                | ValExpr(_, v)    -> ValExpr(NoPos, v)
                | VarExpr(_, name) -> VarExpr(NoPos, name)
                | _                -> lhs
              in
              raise (RefError err_expr)))



(* evaluate a value *)
and eval_expr (env:environment_t) (h:heap_t) (e:expr_t) : value_t*heap_t =
  (*Printf.printf "%s %s\n" (str_expr e) (str_environment_simple env);*)
  match e with
  | BlockExpr(p,b) -> eval_block env h b
  | BopExpr(_,e1,MinusBop,e2) ->
     let v1,h' = (eval_expr env h e1) in
     let v2,h' = (eval_expr env h e2) in
     NumVal(to_num v1  -. to_num v2 ),h'
  (* TODO *)
  | VarExpr(p, name) -> (* Variable Expression *)
    (match read_environment env name with
     | Some (Immutable, v)          -> v, h
     | Some (Mutable  , RefVal addr) ->
         heap_deref h addr, h
     | Some (Mutable  , _) ->        (* should never happen *)
         raise (Failure "mutable var not a RefVal")
     | None ->
         raise (UnboundVariable name))

  | ValExpr(p,v) -> v, h

  | FuncExpr(_, lambda) ->ClosureVal(env,lambda), h

  | CallExpr(_, fn_expr, arg_exprs) ->
    (* 1. Evaluate the function expression *)
    let fn_val, h1 = eval_expr env h fn_expr in

    (* 2. Evaluate arguments left-to-right, threading the heap *)
    let rec eval_args h = function
      | []      -> [], h
      | e :: es ->
          let v, h'   = eval_expr env h e in
          let vs, h'' = eval_args h' es in
          v :: vs, h''
    in
    let arg_vals, h2 = eval_args h1 arg_exprs in

    (* 3. Apply the function value *)
    (match fn_val with
     | ClosureVal (closure_env, (name_opt, params, body, _pos)) ->
         if List.length params <> List.length arg_vals then
           raise (InvalidCall fn_expr);

         (* 3a. Start the call-time environment with the closure’s env *)
         let call_env =
           match name_opt with
           | Some fname ->  (* named function – allow recursion *)
               bind_environment closure_env fname Immutable fn_val
           | None -> closure_env
         in
         (* 3b. Bind each parameter to its corresponding argument *)
         let call_env =
           List.fold_left2
             (fun e (param, _) argv ->
                 bind_environment e param Immutable argv)
             call_env params arg_vals
         in
         (* 3c. Evaluate the body as a block *)
         eval_block call_env h2 body

     | _ ->
         raise (InvalidCall fn_expr))

  (* Add Lab 6 expr using h heap*)
  | BopExpr(_, e1, PlusBop, e2) ->
    let v1, h' = eval_expr env h e1 in
    let v2, h'' = eval_expr env h' e2 in
    (match (v1, v2) with
     | (StrVal(_), _) | (_, StrVal(_)) -> StrVal(to_str v1 ^ to_str v2), h''
     | _ -> NumVal(to_num v1 +. to_num v2), h'')
  | BopExpr(_, e1, TimesBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      NumVal(to_num v1 *. to_num v2), h''
  | BopExpr(_, e1, DivBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      NumVal(to_num v1 /. to_num v2), h''
  | UopExpr(_, PosUop, e1) ->
      let v1, h' = eval_expr env h e1 in
      NumVal(+. to_num v1), h'
  | UopExpr(_, NegUop, e1) ->
      let v1, h' = eval_expr env h e1 in
      NumVal(-. to_num v1), h'
  | UopExpr(_, NotUop, e1) ->
      let v1, h' = eval_expr env h e1 in
      BoolVal(not (to_bool v1)), h'
  | BopExpr(_, e1, EqBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_))
      | (NumVal(_), NumVal(_))
      | (BoolVal(_), BoolVal(_))
      | (UndefVal, UndefVal) -> BoolVal(v1 = v2), h''
      | _ -> BoolVal(false), h'')
  | BopExpr(_, e1, NeqBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_))
      | (NumVal(_), NumVal(_))
      | (BoolVal(_), BoolVal(_))
      | (UndefVal, UndefVal) -> BoolVal(v1 <> v2), h''
      | _ -> BoolVal(true), h'')
  | BopExpr(_, e1, LtBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(v1 < v2), h''
      | _ -> BoolVal(to_num v1 < to_num v2), h'')
  | BopExpr(_, e1, LteBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(v1 <= v2), h''
      | _ -> BoolVal(to_num v1 <= to_num v2), h'')
  | BopExpr(_, e1, GtBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(v1 > v2), h''
      | _ -> BoolVal(to_num v1 > to_num v2), h'')
  | BopExpr(_, e1, GteBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      let v2, h'' = eval_expr env h' e2 in
      (match (v1, v2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(v1 >= v2), h''
      | _ -> BoolVal(to_num v1 >= to_num v2), h'')
  | BopExpr(_, e1, AndBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      if to_bool v1 then eval_expr env h' e2 else v1, h'
  | BopExpr(_, e1, OrBop, e2) ->
      let v1, h' = eval_expr env h e1 in
      if to_bool v1 then v1, h' else eval_expr env h' e2
  | IfExpr(_, cond, e_then, e_else) ->
      let v_cond, h' = eval_expr env h cond in
      if to_bool v_cond then eval_expr env h' e_then else eval_expr env h' e_else
  | PrintExpr(_, e) ->
      let v, h' = eval_expr env h e in
      print_endline (to_str v);
      UndefVal, h'

  | _ -> raise (UnimplementedExpr(e))


(*********)
(* Tests *)
(*********)

let test_group name tests =
  (name, compose (eval empty_env empty_heap) parse_string, eq_value_heap_value, eq_exn,
   Some((fun (x : string) -> x),
        (fun ((v,h) : value_t*heap_t) -> str_value v)),
   (* None, *)
   (List.map
      (fun (name,js,expected) ->
        (name,js,(match expected with
                  | Ok(v) -> Ok(v,empty_heap)
                  | Error(v) -> Error(v))))
     tests))

(* basic tests for the evaluator (do not modify) *)
let simple_expr_eval_tests =
  test_group "Simple Expression Evaluation"
    [
      (None, "1 + true",                     Ok(NumVal(2.0)));
      (None, "false + true",                 Ok(NumVal(1.0)));
      (None, "100 || 200",                   Ok(NumVal(100.0)));
      (None, "-false",                       Ok(NumVal(0.0)));
      (None, "1 + 1",                        Ok(NumVal(2.0)));
      (None, "3 + (4 + 5)",                  Ok(NumVal(12.0)));
      (None, "3 * (4 + 5)",                  Ok(NumVal(27.0)));
      (None, "-6 * 90 - 8",                  Ok(NumVal(-548.0)));
      (None, "-100 + 50",                    Ok(NumVal(-50.0)));
      (None, "true && (false || true)",      Ok(BoolVal(true)));
      (None, "true && (false || !true)",     Ok(BoolVal(false)));
      (None, "1 < 2",                        Ok(BoolVal(true)));
      (None, "100 === 100",                  Ok(BoolVal(true)));
      (None, "100 === 101",                  Ok(BoolVal(false)));
      (None, "100 !== 200",                  Ok(BoolVal(true)));
      (None, "true === true",                Ok(BoolVal(true)));
      (None, "0 / 0",                        Ok(NumVal(nan)));
      (None, "console.log(\"Hello World\")", Ok(UndefVal));
      (None, "(1 < 2) ? 123 : 124",          Ok(NumVal(123.0)));
      (None, "\"aaa\" < \"aaaa\"",           Ok(BoolVal(true)));
      (None, "\"bbb\" < \"aaa\"",            Ok(BoolVal(false)));
      (None, "\"hello\"+\" \"+\"world\"",    Ok(StrVal("hello world")));
      (None, "const x = 1; x+1",             Ok(NumVal(2.0)));
      (None, "const x=1; const y=2; x+y",    Ok(NumVal(3.0)));
      (None, "const x=3; const y=x*2+1; y",  Ok(NumVal(7.0)));
    ]

let fact_js = "function factorial(n){return (n <= 1) ? 1 : (n * factorial(n-1));}"
let fib_js = "function fib(x){return x<=0 ? 0 : (x===1 ? 1 : fib(x-1)+fib(x-2));}"
let scopes_js =
"(function (x) {
    return function(f) {
        return function (x) {
            return f(0);
        }(2);
    } (function (y) {return x;});
} (1))"

let readme1_js =
  "const f = function(x){ return x+1; };
   const r = f(2);
   r+3"

let readme2_js =
  "const x = 5;
   const f = function(y){ return x + y; };
   (function(z) { const x = 7; return f(6); })(0)"

(* basic tests for the evaluator (do not modify) *)
let simple_func_eval_tests =
  test_group "Simple Function Definition Evaluation"
    [
      (None, "function test(x){const x = 123; return x;}",
       Ok(ClosureVal(StringMap.empty,(
                Some("test"),
                [("x",None)],
                StmtBlock(NoPos,
                          ConstStmt(NoPos,
                                    "x",
                                    ValExpr(NoPos,NumVal(123.0))),
                          ReturnBlock(NoPos,VarExpr(NoPos,"x"))),
                None))));
      (None, fact_js,
       Ok(ClosureVal(StringMap.empty,(
                Some("factorial"),
                [("n",None)],
                ReturnBlock(NoPos,IfExpr(NoPos,
                                         BopExpr(NoPos,VarExpr(NoPos,"n"),LteBop,ValExpr(NoPos,NumVal(1.0))),
                                         ValExpr(NoPos,NumVal(1.0)),
                                         BopExpr(NoPos,VarExpr(NoPos,"n"),TimesBop,CallExpr(NoPos,VarExpr(NoPos,"factorial"),[BopExpr(NoPos,VarExpr(NoPos,"n"),MinusBop,ValExpr(NoPos,NumVal(1.0)))]))
                  )),
                None))));

    ]
(* note - you can use the following to print a program for debugging *)
(* let _ = Printf.printf "RESULT = %s\n" (str_program (parse_string "const x = 1 + 1; x * 2")) *)


let simple_call_eval_tests =
  test_group "Simple Call Evaluation"
    [
      (None, "const f = function(x){return x+1;}; f(1)",                    Ok(NumVal(2.0)));
      (None, "const y = 5; const f = function(x){return x+1;}; f(y)",       Ok(NumVal(6.0)));
      (Some("recursion"), "const f = function t(x){return x===0 ? 0 : x+t(x-1);}; f(5)", Ok(NumVal(15.0)));

      (Some("Readme 1"), readme1_js, Ok(NumVal(6.0)));
      (Some("Readme 2"), readme2_js, Ok(NumVal(11.0)));
      (Some("Lecture Scoping Test"), scopes_js, Ok(NumVal(1.0)))
    ]


(** Basic (instructor-provided) tests for mutability (do not modify.) *)
let simple_mut_eval_tests =
  test_group "Simple Mutability Evaluation"
    [
      (None, "1 = 100; 0",                   Error(RefError(ValExpr(NoPos,(NumVal(1.0))))));
      (None, "const x=3; x=1; x+1",          Error(ImmutableVar("x")));
      (None, "let x=3; x+1",                 Ok(NumVal(4.0)));
      (None, "let x=3; x=1; x+1",            Ok(NumVal(2.0)));
      (None, "let x=3; let y=4; x=x+y+5; y=10+y+x; x+y",            Ok(NumVal(38.0)));
      (None, "let x=3; x=true; x",            Ok(BoolVal(true)));
      (Some("static scope"), "let x=5; const f=function(y){return x+y;}; x=10; (function(z){const x=7; return f(6);})(0)", Ok(NumVal(16.0)));
]

let mut_eval_tests =
  test_group "Mutability Evaluation"
    [
      (None, "let x = 3; x + 1", Ok(NumVal(4.0)));
      (None, "let x = 3; x = 5; x + 1", Ok(NumVal(6.0)));
      (None, "let x = 3; const y = x + 2; x = 10; x + y", Ok(NumVal(15.0)));
      (None, "let x = 3; x = true; x", Ok(BoolVal(true)));
      (None, "const x = 3; x = 5; x", Error(ImmutableVar("x")));
    ]
