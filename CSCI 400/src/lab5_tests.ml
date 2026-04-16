open Javascript_parser;;
open Javascript_lexer;;
open Javascript_main;;
open Javascript_ast;;
open Util;;

let lexer_test_fun s =
  tokenize (Lexing.from_string s)


let lexer_tests =
  ("lexer_tests",
   lexer_test_fun,
   (=), (=),
   Some((fun (x : string) -> x), str_token_list),
   [
     (Some("simple js"),
      "1 + 2",
      Ok([NUMBER(1.0); ADD_OP; NUMBER(2.0); EOF]));
     (Some("named function"),
      "function foo() { return; }",
      Ok([
        FUNC_KW; IDENT("foo"); LP_KW; RP_KW; 
        LCB_KW; RET_KW; SEMICOLON_OP; RCB_KW; EOF
      ]));

           (Some("let array assignment"),
      "let arr = [true, false, 123]",
      Ok([
        LET_KW; IDENT("arr"); ASSIGN_OP;
        LSB_KW; TRUE_KW; COMMA_OP; FALSE_KW; COMMA_OP; NUMBER(123.0); RSB_KW;
        EOF
      ]));

     (Some("strict equality"),
      "x===y",
      Ok([
        IDENT("x"); STREQ_OP; IDENT("y"); EOF
      ]));

     (Some("not strict equality with spaces"),
      "x    !==    y",
      Ok([
        IDENT("x"); NSTREQ_OP; IDENT("y"); EOF
      ]));

     (Some("decimal numbers"),
      "123.456 + 0.789",
      Ok([
        NUMBER(123.456); ADD_OP; NUMBER(0.789); EOF
      ]));

     (Some("console.log multiple"),
      "console.log(x, y, z)",
      Ok([
        CONSOLE_KW; DOT_OP; LOG_KW; 
        LP_KW; IDENT("x"); COMMA_OP; IDENT("y"); COMMA_OP; IDENT("z"); RP_KW;
        EOF
      ]));

     (Some("Infinity times (NaN - 2)"),
      "Infinity * (NaN - 2)",
      Ok([
        INFINITY_KW; MUL_OP; 
        LP_KW; NAN_KW; SUB_OP; NUMBER(2.0); RP_KW;
        EOF
      ]));

     (Some("mixed logical operators"),
      "x || y && z",
      Ok([
        IDENT("x"); LOG_OR_OP; IDENT("y"); LOG_AND_OP; IDENT("z"); EOF
      ]));

     (Some("undefined keyword"),
      "undefined",
      Ok([
        UNDEF_KW; EOF
      ]));

     (Some("chained dot access"),
      "x.y.z",
      Ok([
        IDENT("x"); DOT_OP; IDENT("y"); DOT_OP; IDENT("z"); EOF
      ]));
   ])

let parser_tests =
  ("parser_tests",
   parse_string,
   eq_program, (=),
   Some((fun (x : string) -> x), str_program),
   [
      (Some("simple expression"),
        "1 + 2",
        Ok(ExprProgram(NoPos, BopExpr( NoPos,
                                       ValExpr(NoPos, NumVal(1.0)),
                                       PlusBop,
                                       ValExpr(NoPos, NumVal(2.0)) ))));

      (Some("Identity lambda"),
        "function (x) {return x;}",
        Ok(ExprProgram(NoPos,
                       FuncExpr(NoPos,
                                (None, (* ident_t option *)
                                 [("x", None)], (* typed_ident_t list *)
                                 ReturnBlock(NoPos, VarExpr(NoPos, "x")), (* block_t *)
                                 None (* typ_t option *)
                                )))));
       (* TODO *)
     (Some("string literal"),
      "\"Hello\"",
      Ok(
        ExprProgram(NoPos,
          ValExpr(NoPos, StrVal("Hello"))
        )
      ));


     (Some("call with multiple args"),
      "foo(bar, 123)",
      Ok(
        ExprProgram(NoPos,
          CallExpr(
            NoPos,
            VarExpr(NoPos, "foo"),
            [
              VarExpr(NoPos, "bar");
              ValExpr(NoPos, NumVal(123.0))
            ]
          )
        )
      ));


     (Some("function returning string"),
      "function() { return \"abc\"; }",
      Ok(
        ExprProgram(NoPos,
          FuncExpr(NoPos,
            ( None,
              [],  (* no parameters *)
              ReturnBlock(NoPos, ValExpr(NoPos, StrVal("abc"))),
              None
            )
          )
        )
      ));

     (Some("short-circuited expression"),
      "(true || false) && NaN",
      Ok(
        ExprProgram(NoPos,
          BopExpr(
            NoPos,
            BopExpr(
              NoPos,
              ValExpr(NoPos, BoolVal(true)),
              OrBop,
              ValExpr(NoPos, BoolVal(false))
            ),
            AndBop,
            ValExpr(NoPos, NumVal(Pervasives.nan))
          )
        )
      ));

      (Some("Complex expression"),
      "3 + 4 * (2 - 1) / 5",
      Ok(ExprProgram(NoPos,
                      BopExpr(NoPos,
                              ValExpr(NoPos, NumVal(3.0)),
                              PlusBop,
                              BopExpr(NoPos,
                                      BopExpr(NoPos, 
                                              ValExpr(NoPos, NumVal(4.0)), 
                                              TimesBop, 
                                              BopExpr(NoPos, 
                                                      ValExpr(NoPos, NumVal(2.0)), 
                                                      MinusBop, 
                                                      ValExpr(NoPos, NumVal(1.0)))),
                                      DivBop,
                                      ValExpr(NoPos, NumVal(5.0)))))));

 (Some("Precedence Test"),
         "!((3 === 3)===true)",
         Ok(ExprProgram(NoPos,
                        UopExpr(NoPos,
                                 NotUop,
                                 BopExpr(NoPos,
                                           BopExpr(NoPos,
                                                   ValExpr(NoPos, NumVal(3.0)),
                                                   EqBop,
                                                   ValExpr(NoPos, NumVal(3.0))),
                                           EqBop,
                                           ValExpr(NoPos, BoolVal(true)))))));


(Some("Assignment Expression"),
        "x = y;",
        Ok(StmtProgram(NoPos,
                      AssignStmt(NoPos,
                                VarExpr(NoPos, "x"),
                                VarExpr(NoPos, "y")),
                      ExprProgram(NoPos, ValExpr(NoPos, UndefVal)))));
(Some("Variable declaration with let"),
       "let a = 5;",
       Ok(StmtProgram(NoPos, 
                      LetStmt(NoPos, "a", 
                              ValExpr(NoPos, NumVal(5.0))), 
                      ExprProgram(NoPos,
                                  ValExpr(NoPos, UndefVal)))));


(Some("Return expression"),
        "{ return hello ; }",
        Ok(ExprProgram(NoPos,
                      BlockExpr(NoPos,
                               ReturnBlock(NoPos, 
                                           VarExpr(NoPos, "hello"))))));

 (Some("Console Log expression"),
        "console.log(HelloWorld)",
        Ok(ExprProgram(NoPos,
                      PrintExpr(NoPos, 
                               VarExpr(NoPos, 
                                      "HelloWorld")))));

  ])
