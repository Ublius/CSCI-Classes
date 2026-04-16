open Javascript_ast
open Javascript_env
open Javascript_main
open Util

(*
 * Check javascript_ast.ml and javascript_env.ml for the following useful functionality:
 * - str_float               -- convert a float to a string
 * - to_num, to_bool, to_str -- do the JavaScript automatic type conversion
 * - bind_environment        -- add a variable binding to the environment
 * - read_environment        -- look up a variable's value in the environment
 * - empty_env               -- the empty environment
 *)

(*
 * (eval env p) should reduce a program in initial environment env to a *value*
 * In general, if Node.js produces a *value* for an example JavaScript
 * program, your evaluator should produce that same value.
 * You should support basic JavaScript (recursive higher-order) functions
 * with lexical scoping.
 *
 * See the assignment README for more details.
 *)

(* evaluate a program *)
let rec eval (env : environment_t) (p: program_t) : value_t = match p with
  | ExprProgram(_,e) -> eval_expr env e
  | StmtProgram(_, s, p1) ->
    eval (eval_stmt env s) p1;

(* evaluate a block *)
and eval_block (env:environment_t) (p:block_t) : value_t = match p with
  | ReturnBlock(_, e) -> eval_expr env e
  | StmtBlock(_, s, b) -> eval_block (eval_stmt env s) b

(* evaluate a statement *)
and eval_stmt (env:environment_t) (s:stmt_t) : environment_t = match s with
  | ConstStmt(_, id, e1) -> bind_environment env id Immutable (eval_expr env e1)
  | _ -> raise (UnimplementedStmt(s))

(* evaluate a value *)
and eval_expr (env:environment_t) (e:expr_t) : value_t =
  (*Printf.printf "%s %s\n" (str_expr e) (str_environment_simple env);*)
  match e with
  | BlockExpr(p,b) -> eval_block env b
  | BopExpr(_,e1,MinusBop,e2) ->
     NumVal(to_num (eval_expr env e1) -. to_num (eval_expr env e2))

  | VarExpr(p, name) -> (* Variable Expression *)
    (match read_environment env name with
    | Some(_, value) -> value
    | None -> raise (UndeclaredVar(name))
    )
  | ValExpr(p,v) -> v

  | FuncExpr(_, lambda) ->ClosureVal(env,lambda)

    | CallExpr(_, fn_expr, arg_exprs) ->
      (* 1) evaluate the function expression *)
      let fn_val = eval_expr env fn_expr in
      (* 2) evaluate all the arguments *)
      let arg_vals = List.map (eval_expr env) arg_exprs in
      (match fn_val with
       | ClosureVal (closure_env, (name_opt, params, body, _)) ->
           (* 3) start the function’s call‐env with its define‐time env *)
           let fn_env =
             match name_opt with
             | Some fname ->
                 (* if it’s a named function, bind its own name for recursion *)
                 bind_environment closure_env fname Immutable fn_val
             | None ->
                 closure_env
           in
           (* 4) bind each parameter to its corresponding argument *)
           let fn_env =
             List.fold_left2
               (fun e (param,_) argv -> bind_environment e param Immutable argv)
               fn_env
               params
               arg_vals
           in
           (* 5) evaluate the function body as a block and return its value *)
           eval_block fn_env body

       | _ ->
           (* not a function value? *)
           raise (InvalidCall fn_expr)
      )


  (* Add Lab 6 expr *)
  | BopExpr(_,e1,PlusBop,e2) ->
    (let eval_e1 = eval_expr env e1 in
      let eval_e2 = eval_expr env e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), _)
      | (_, StrVal(_)) -> StrVal(to_str eval_e1 ^ to_str eval_e2)
      | _ -> NumVal(to_num eval_e1 +. to_num eval_e2))
  | BopExpr(_,e1,TimesBop,e2) ->
     NumVal(to_num (eval_expr env e1) *. to_num (eval_expr env e2))
  | BopExpr(_,e1,DivBop,e2) ->
     NumVal(to_num (eval_expr env e1) /. to_num (eval_expr env e2))
  | UopExpr(_,PosUop,e1) ->
     NumVal(+. (to_num (eval_expr env e1)))
  | UopExpr(_,NegUop,e1) ->
     NumVal(-. to_num (eval_expr env e1))
  | UopExpr(_, NotUop, e1) ->
     BoolVal (not(to_bool(eval_expr env e1)))
  | BopExpr(_,e1,EqBop,e2) ->
      (let eval_e1 = eval_expr env e1 in
       let eval_e2 = eval_expr env e2 in
       match (eval_e1, eval_e2) with
       | (StrVal(_), StrVal(_))
       | (NumVal(_), NumVal(_))
       | (BoolVal(_), BoolVal(_))
       | (UndefVal, UndefVal) -> BoolVal(eval_e1 = eval_e2)
       | _ -> BoolVal(false))
  | BopExpr(_,e1,NeqBop,e2) ->
     (let eval_e1 = eval_expr env e1 in
      let eval_e2 = eval_expr env e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_))
      | (NumVal(_), NumVal(_))
      | (BoolVal(_), BoolVal(_))
      | (UndefVal, UndefVal)-> BoolVal(eval_e1 <> eval_e2)
      | _ -> BoolVal(true))
  | BopExpr(_,e1,LtBop,e2) ->
    (let eval_e1 = eval_expr env e1 in
    let eval_e2 = eval_expr env e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_),StrVal(_)) -> BoolVal(eval_e1 < eval_e2)
      | _ -> BoolVal(to_num eval_e1 < to_num eval_e2))
  | BopExpr(_,e1,LteBop,e2) ->
    (let eval_e1 = eval_expr env e1 in
      let eval_e2 = eval_expr env e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 <= eval_e2)
      | _ -> BoolVal(to_num eval_e1 <= to_num eval_e2))
  | BopExpr(_,e1,GtBop,e2) ->
    (let eval_e1 = eval_expr env e1 in
      let eval_e2 = eval_expr env e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 > eval_e2)
      | _ -> BoolVal(to_num eval_e1 > to_num eval_e2))
  | BopExpr(_,e1,GteBop,e2) ->
        (let eval_e1 = eval_expr env e1 in
         let eval_e2 = eval_expr env e2 in
         match (eval_e1, eval_e2) with
         | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 >= eval_e2)
         | _ -> BoolVal(to_num eval_e1 >= to_num eval_e2))
  | BopExpr(_,e1,AndBop,e2) ->
      (match (to_bool (eval_expr env e1), eval_expr env e2) with
        | (true, v2) ->  eval_expr env e2
        | (false, v2) -> eval_expr env e1)
  | BopExpr(_, e1, OrBop, e2) ->
     (let v1 = eval_expr env e1 in if to_bool v1 then v1 else (eval_expr env e2))
  | IfExpr(_, cond, e_then, e_else) ->
      if to_bool (eval_expr env cond) then eval_expr env e_then else eval_expr env e_else
  | PrintExpr(_, e) -> 
      print_endline (to_str (eval_expr env e));
      UndefVal
  | _ -> raise (UnimplementedExpr(e))



(*********)
(* Tests *)
(*********)

let test_group name tests =
  (name, compose (eval empty_env) parse_string, eq_value, eq_exn,
   Some((fun (x : string) -> x),str_value),
   (* None, *)
   tests)

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
    ]

let simple_var_eval_tests =
  test_group "Simple Variable Evaluation"
    [
      (None, "const x = 1; x+1",            Ok(NumVal(2.0)));
      (None, "const x=1; const y=2; x+y",   Ok(NumVal(3.0)));
      (None, "const x=3; const y=x*2+1; y", Ok(NumVal(7.0)));
      (None, "const x = 1; y",              Error(UndeclaredVar("y")));
    ]


let var_eval_tests =
  test_group "Variable Evaluation"
    [
      (None, "const x = true; x + 1", Ok(NumVal(2.0)));
      (None, "const x = 1; x + 1", Ok(NumVal(2.0)));
      (None, "x + 1", Error(UndeclaredVar("x")));
      (None, "const x = 1; const y = 2; x + y", Ok(NumVal(3.0)));
      (None, "const x = 1; const y = 2; const z = 3; x + y + z", Ok(NumVal(6.0)));
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

let func_eval_tests =
  test_group "Function Definition Evaluation"
    [
      (* 1) anonymous function literal, no free vars *)
      ( None,
        "function(x){ return x+1; }",
        Ok (ClosureVal ( StringMap.empty,
                         ( None,
                           [("x", None)],
                           ReturnBlock (NoPos,
                                        BopExpr (NoPos,
                                                 VarExpr (NoPos, "x"),
                                                 PlusBop,
                                                 ValExpr (NoPos, NumVal 1.0))),
                           None )))) ;
      (* 2) named function literal, no free vars *)
      ( None,
        "function foo(x){ return x+2; }",
        Ok (ClosureVal ( StringMap.empty,
                         ( Some "foo",
                           [("x", None)],
                           ReturnBlock (NoPos,
                                        BopExpr (NoPos,
                                                 VarExpr (NoPos, "x"),
                                                 PlusBop,
                                                 ValExpr (NoPos, NumVal 2.0))),
                           None )))) ;
      (* 3) function literal that closes over outer const x *)
      ( None,
        "const x = 10; function(y){ return x+y; }",
        Ok (ClosureVal ( StringMap.(add "x" (Immutable, NumVal 10.0) empty),
                         ( None,
                           [("y", None)],
                           ReturnBlock (NoPos,
                                        BopExpr (NoPos,
                                                 VarExpr (NoPos, "x"),
                                                 PlusBop,
                                                 VarExpr (NoPos, "y"))),
                           None ))))
    ]


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

let call_eval_tests =
  test_group "Call Evaluation"
    [
      (* TODO 5 tests *)
      (Some("nested_closure"), "const x = 10; const f = function(y) { return function(z) { return x + y + z; }; }; const g = f(3); g(7)", Ok(NumVal(20.0)));
      (Some("returning_function"), "const f = function() { return function() { return 42; }; }; const g = f(); g()", Ok(NumVal(42.0)));
      (Some("internal_helper"), "const x = 10; const f = function(n) { const helper = function(y) { return x + y; }; return helper(n); }; f(5)", Ok(NumVal(15.0)));
      (Some("delayed_closure"), "const x = 2; const f = function() { return x + 3; }; const x = 100; f()", Ok(NumVal(5.0)));
      (Some("multi_param_closure"), "const base = 10; const f = function(a, b) { return a + b + base; }; f(2,3)", Ok(NumVal(15.0)));
      (Some("fib"), Printf.sprintf "(%s)(6)" fib_js, Ok(NumVal(8.0)));
    ]
