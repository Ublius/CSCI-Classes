open Javascript_ast
open Javascript_main
open Util

(*
 * Check javascript_ast.ml for the following useful functionality:
 * - str_float               -- convert a float to a string
 * - to_num, to_bool, to_str -- do the JavaScript automatic type conversion
 *)

(* basic tests to show how the value conversion functions work (do not modify) *)
let simple_to_num_tests =
  ("Simple ToNum Conversions", to_num, (fun n1 n2 -> eq_float (n1,n2)), eq_exn, Some(str_value,str_float),
   [
     (None, NumVal(123.0), Ok(123.0));
     (None, BoolVal(true), Ok(1.0));
     (None, StrVal(""),    Ok(0.0));
  ])
let simple_to_bool_tests =
  ("Simple ToBool Conversions", to_bool, (=), eq_exn, Some(str_value,string_of_bool),
   [
     (None, BoolVal(true),  Ok(true));
     (None, NumVal(1.0),    Ok(true));
     (None, StrVal("true"), Ok(true));
  ])
let simple_to_str_tests =
  ("Simple ToStr Conversions", to_str, (=), eq_exn, Some(str_value,(fun x -> x)),
   [
     (None, StrVal("hello"), Ok("hello"));
     (None, BoolVal(true),   Ok("true"));
     (None, NumVal(1.234),   Ok("1.234"));
     (None, NumVal(1.000),   Ok("1"));
     (None, NumVal(0.00),    Ok("0"));
     (None, NumVal(100.01),  Ok("100.01"));
  ])

(* (eval p) should reduce a program to a *value* (if Node.js produces
 * a *value* for an example JavaScript program, your evaluator should
 * produce that same value).  *)

(* evaluate a program *)
let rec eval (p : program_t) : value_t = match p with
  | ExprProgram(_,e) -> eval_expr e
  | _ -> raise (UnimplementedProgram(p))


(* evaluate a value *)
and eval_expr (e:expr_t) : value_t =  match e with
  | ValExpr(p,v) -> v
  (* MinusBop provided as an example *)
  | BopExpr(_,e1,MinusBop,e2) ->
     NumVal(to_num (eval_expr e1) -. to_num (eval_expr e2))
  | BopExpr(_,e1,PlusBop,e2) ->
     begin match (eval_expr e1, eval_expr e2) with
     | (StrVal n1, StrVal n2) -> StrVal(n1 ^ n2)
     | (StrVal n1, NumVal n2) -> StrVal(n1 ^ to_str(eval_expr(e2)))
     | (NumVal n1, StrVal n2) -> StrVal(to_str(eval_expr(e1)) ^ n2)
     | (StrVal n1, BoolVal n2) -> StrVal(n1 ^ to_str(eval_expr e2))
     | (BoolVal n1, StrVal n2) -> StrVal(to_str(eval_expr e1) ^ n2)
     | _ -> NumVal(to_num(eval_expr(e1)) +. to_num(eval_expr(e2)))
     end
  | BopExpr(_,e1,TimesBop,e2) ->
     NumVal(to_num (eval_expr e1) *. to_num (eval_expr e2))
  | BopExpr(_,e1,DivBop,e2) ->
     NumVal(to_num (eval_expr e1) /. to_num (eval_expr e2))
  | UopExpr(_,PosUop,e1) ->
     NumVal(+. (to_num (eval_expr e1)))
  | UopExpr(_,NegUop,e1) ->
     NumVal(-. to_num (eval_expr e1))
  | UopExpr(_, NotUop, e1) ->
     BoolVal (not(to_bool(eval_expr e1)))
  | BopExpr(_,e1,EqBop,e2) ->
      (let eval_e1 = eval_expr e1 in
       let eval_e2 = eval_expr e2 in
       match (eval_e1, eval_e2) with
       | (StrVal(_), StrVal(_))
       | (NumVal(_), NumVal(_))
       | (BoolVal(_), BoolVal(_))
       | (UndefVal, UndefVal) -> BoolVal(eval_e1 = eval_e2)
       | _ -> BoolVal(false))
  | BopExpr(_,e1,NeqBop,e2) ->
     (let eval_e1 = eval_expr e1 in
      let eval_e2 = eval_expr e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_))
      | (NumVal(_), NumVal(_))
      | (BoolVal(_), BoolVal(_))
      | (UndefVal, UndefVal)-> BoolVal(eval_e1 <> eval_e2)
      | _ -> BoolVal(true))
  | BopExpr(_,e1,LtBop,e2) ->
    (let eval_e1 = eval_expr e1 in
    let eval_e2 = eval_expr e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_),StrVal(_)) -> BoolVal(eval_e1 < eval_e2)
      | _ -> BoolVal(to_num eval_e1 < to_num eval_e2))
  | BopExpr(_,e1,LteBop,e2) ->
    (let eval_e1 = eval_expr e1 in
      let eval_e2 = eval_expr e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 <= eval_e2)
      | _ -> BoolVal(to_num eval_e1 <= to_num eval_e2))
  | BopExpr(_,e1,GtBop,e2) ->
    (let eval_e1 = eval_expr e1 in
      let eval_e2 = eval_expr e2 in
      match (eval_e1, eval_e2) with
      | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 > eval_e2)
      | _ -> BoolVal(to_num eval_e1 > to_num eval_e2))
  | BopExpr(_,e1,GteBop,e2) ->
        (let eval_e1 = eval_expr e1 in
         let eval_e2 = eval_expr e2 in
         match (eval_e1, eval_e2) with
         | (StrVal(_), StrVal(_)) -> BoolVal(eval_e1 >= eval_e2)
         | _ -> BoolVal(to_num eval_e1 >= to_num eval_e2))
  | BopExpr(_,e1,AndBop,e2) ->
      (match (to_bool (eval_expr e1), eval_expr e2) with
        | (true, v2) ->  eval_expr e2
        | (false, v2) -> eval_expr e1)
  | BopExpr(_, e1, OrBop, e2) ->
     (let v1 = eval_expr e1 in if to_bool v1 then v1 else (eval_expr e2))
  | IfExpr(_, cond, e_then, e_else) ->
      if to_bool (eval_expr cond) then eval_expr e_then else eval_expr e_else
  | PrintExpr(_, e) -> 
      print_endline (to_str (eval_expr e));
      UndefVal
  | _ -> raise (UnimplementedExpr(e))

(*********)
(* Tests *)
(*********)

let test_group name tests =
  (name, compose eval parse_string, eq_value, eq_exn,
   Some((fun (x:string) -> x),str_value), tests)


(* basic tests for the evaluator (do not modify) *)
let simple_expr_eval_tests =
  test_group "Simple Expression Evaluation"
    [
      (None, "1 + true",                    Ok(NumVal(2.0)));
      (None, "false + true",                Ok(NumVal(1.0)));
      (None, "100 || 200",                  Ok(NumVal(100.0)));
      (None, "-false",                      Ok(NumVal(0.0)));
      (None, "1 + 1",                       Ok(NumVal(2.0)));
      (None, "3 + (4 + 5)",                 Ok(NumVal(12.0)));
      (None, "3 * (4 + 5)",                 Ok(NumVal(27.0)));
      (None, "-6 * 90 - 8",                 Ok(NumVal(-548.0)));
      (None, "-100 + 50",                   Ok(NumVal(-50.0)));
      (None, "true && (false || true)",     Ok(BoolVal(true)));
      (None, "true && (false || !true)",    Ok(BoolVal(false)));
      (None, "1 < 2",                       Ok(BoolVal(true)));
      (None, "100 === 100",                 Ok(BoolVal(true)));
      (None, "100 === 101",                 Ok(BoolVal(false)));
      (None, "100 !== 200",                 Ok(BoolVal(true)));
      (None, "true === true",               Ok(BoolVal(true)));
      (None, "0 / 0",                       Ok(NumVal(nan)));
    ]


let simple_print_eval_tests =
  test_group "Simple Print Evaluation"
    [
      (None, "console.log(\"Hello World\")",           Ok(UndefVal));
    ]

let simple_cond_eval_tests =
  test_group "Simple Conditional Evaluation"
    [
      (None, "(1 < 2) ? 123 : 124",         Ok(NumVal(123.0)));
    ]

let simple_str_eval_tests =
  test_group "Simple String Evaluation"
    [
      (None, "\"aaa\" < \"aaaa\"",          Ok(BoolVal(true)));
      (None, "\"bbb\" < \"aaa\"",           Ok(BoolVal(false)));
      (None, "\"hello\"+\" \"+\"world\"",   Ok(StrVal("hello world")));
    ]

let eval_tests =
  test_group "Evaluator"
    [
      (* TODO *)
      (None, "55 + 45", Ok(NumVal(100.0)));
      (None, "-55", Ok(NumVal(-55.0)));
      (None, "(2 === 6) || (2 < 0) && (3 === 3)", Ok(BoolVal(false)));
      (None, "(true || false) && false", Ok(BoolVal(false)));
      (None, "(true || false) && true", Ok(BoolVal(true)));
      (None, "+55", Ok(NumVal(55.0)));

      (* Instructor Tests *)
      (None, "5 - 3", Ok (NumVal 2.0)); (None, "3 - 5", Ok (NumVal (-2.0)));
      (None, "2 * 2", Ok (NumVal 4.0)); (None, "-2 * 2", Ok (NumVal (-4.0)));
      (None, "Infinity * 0", Ok (NumVal nan));
      (None, "Infinity * Infinity", Ok (NumVal infinity));
      (None, "1 / 2", Ok (NumVal 0.5));
      (None, "2.0 / 0", Ok (NumVal infinity));
      (None, "2.0 / 0.0", Ok (NumVal infinity));
      (None, "2.0 / -0.0", Ok (NumVal (-.infinity)));
      (None, "true && true", Ok (BoolVal true));
      (None, "true && false", Ok (BoolVal false));
      (None, "false && true", Ok (BoolVal false));
      (None, "false && 3 === 4", Ok (BoolVal false));
      (None, "console.log(1 + \"2\")", Ok UndefVal);
      (None, "undefined + 1", Ok (NumVal nan));
      (None, "3 === 3", Ok (BoolVal true));
      (None, "3 === 4", Ok (BoolVal false));
      (None, "true === true", Ok (BoolVal true));
      (None, "false === false", Ok (BoolVal true));
      (None, "true === 1", Ok (BoolVal false));
      (None, "undefined === undefined", Ok (BoolVal true));
      (None, "true === 1", Ok (BoolVal false));
      (None, "undefined === undefined", Ok (BoolVal true));
      (None, "3 !== 3", Ok (BoolVal false));
      (None, "3 !== 4", Ok (BoolVal true));
      (None, "true !== true", Ok (BoolVal false));
      (None, "true !== false", Ok (BoolVal true));
      (None, "false !== false", Ok (BoolVal false));
      (None, "\"3\" !== 3", Ok (BoolVal true));
      (None, "true !== 1", Ok (BoolVal true));
      (None, "undefined !== undefined", Ok (BoolVal false));
      (None, "true + true", Ok (NumVal 2.0));
      (None, "true + false", Ok (NumVal 1.0));
      (None, "-undefined", Ok (NumVal nan));
      (None, "true > false", Ok (BoolVal true));
      (None, "false > true", Ok (BoolVal false));
      (None, "true > 0", Ok (BoolVal true));
      (None, "true > 1", Ok (BoolVal false));
      (None, "undefined > 3", Ok (BoolVal false));
      (None, "3 > undefined", Ok (BoolVal false));
      (None, "3 > NaN", Ok (BoolVal false));
      (None, "NaN > 3", Ok (BoolVal false));
      (None, "(2.0 * 5.0) + (1.0 / 5.0)", Ok (NumVal 10.2));
      (None, "1 && 2 + 3", Ok (NumVal 5.0));
      (None, "1 || 2 && 10 / 1.023 * 1231 + (1 || 7)", Ok (NumVal 1.0));
      ( None,
        "(\"aaa\" <= \"abcdef\") ? 1.0 * -17826391872 / 9999.9 : 3.0",
        Ok (NumVal (-1782657.0137701377)) );
    ]

let cond_eval_tests =
  test_group "Conditional Evaluation"
    [
      (None, "\"a\" ? true : 400", Ok(BoolVal(true)));
      (None, "\"a\"-\"a\" ? true : 400", Ok(NumVal(400.0)));
      (None, "1 ? 100 : 200", Ok(NumVal(100.0)));
      (None, "0 ? 100 : 200", Ok(NumVal(200.0)));
      (None, "false ? 100 : 200", Ok(NumVal(200.0))); 
      (None, "(true === true) ? 123 : 321", Ok(NumVal(123.0)));
      (None, "false ? 123 : (2 > 1)", Ok(BoolVal(true)));

      (* Instructor Tests *)
      (None, "(1 < 2) ? 123 : 124", Ok (NumVal 123.0));
      (None, "(1 > 2) ? 123 : 124", Ok (NumVal 124.0));
      (None, "(1 < 2) ? ((1 < 3) ? 123 : 124) : 125", Ok (NumVal 123.0));
    ]

let str_eval_tests =
  test_group "String Evaluation"
    [
      (Some("Test adding string int and bool"), "\"Tester\" + \" \" + 123 + \" \" + false", Ok(StrVal("Tester 123 false")));

      (* Instructor Tests *)
      (None, "true || true", Ok (BoolVal true));
      (None, "false || true", Ok (BoolVal true));
      (None, "true || false", Ok (BoolVal true));
      (None, "false || 3 === 4", Ok (BoolVal false));
      (None, "\"Cat\" || \"Dog\"", Ok (StrVal "Cat"));
      (None, "false || \"Cat\"", Ok (StrVal "Cat"));
      (None, "\"Cat\" || false", Ok (StrVal "Cat"));
      (None, "\"\" || false", Ok (BoolVal false));
      (None, "false || \"\"", Ok (StrVal ""));
      (None, "\"Cat\" && \"Dog\"", Ok (StrVal "Dog"));
      (None, "false && \"Cat\"", Ok (BoolVal false));
      (None, "\"Cat\" && false", Ok (BoolVal false));
      (None, "\"\" && false", Ok (StrVal ""));
      (None, "false && \"\"", Ok (BoolVal false));
      (None, "5 < \"hello\"", Ok (BoolVal false));
      (None, "5 < \"6\"", Ok (BoolVal true));
      (None, "\"a\" < \"b\"", Ok (BoolVal true));
      (None, "\"a\" < 2", Ok (BoolVal false));
      (None, "true < \"abc\"", Ok (BoolVal false));
      (None, "true <= \"true\"", Ok (BoolVal false));
      (None, "\"hello\" <= 5", Ok (BoolVal false));
      (None, "\"a\" <= \"abc\"", Ok (BoolVal true));
      (None, "\"5\" <= 6", Ok (BoolVal true));
      (None, "\"a\" <= \"3\"", Ok (BoolVal false));
      (None, "\"3\" <= 3", Ok (BoolVal true));
      (None, "\"a\" > \"a\"", Ok (BoolVal false));
      (None, "\"b\" > \"a\"", Ok (BoolVal true));
      (None, "\"5\" > 3", Ok (BoolVal true));
      (None, "\"3\" >= 5", Ok (BoolVal false));
      (None, "\"aaaa\" >= \"abc\"", Ok (BoolVal false));
      (None, "\"123\" >= 123", Ok (BoolVal true));
      (None, "\"1\" === \"1\" + \"\"", Ok (BoolVal true));
      (None, "\"1\" === 1", Ok (BoolVal false));
      (None, "\"hello\" === \"hello\"", Ok (BoolVal true));
      (None, "\"hello\" === \"hola\"", Ok (BoolVal false));
      (None, "(\"dog\" || false) ? 1.0 : 2.0", Ok (NumVal 1.0));
      (None, "(\"dog\" && false) ? 1.0 : 2.0", Ok (NumVal 2.0));
      (None, "(false || \"\") ? 1.0 : 2.0", Ok (NumVal 2.0));
      (None, "(false || \"dog\") ? 1.0 : 2.0", Ok (NumVal 1.0));
      (None, "\"hello\" !== \"hello\"", Ok (BoolVal false));
      (None, "\"hello\" !== \"hola\"", Ok (BoolVal true));
      (None, "\"aaa\" + \"aaa\"", Ok (StrVal "aaaaaa"));
      (None, "\"abc\" + 123", Ok (StrVal "abc123"));
      (None, "123 + \"abc\"", Ok (StrVal "123abc"));
      (None, "\"12\" + 0.123", Ok (StrVal "120.123"));
      (None, "true + \"1\"", Ok (StrVal "true1"));
      (None, "\"foo\" - 3.0", Ok (NumVal nan));
      (None, "5 - \"3\"", Ok (NumVal 2.0));
      (None, "\"foo\" * 2", Ok (NumVal nan));
      (None, "\"2\" * 2", Ok (NumVal 4.0));
    ]
