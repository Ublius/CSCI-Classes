open Util (* see util.ml *)

(****************)
(* Starter Code *)
(****************)

(* Simulating Laziness *)

type 'a thunk = unit -> 'a

type 'a suspval =
  | Unevaluated of 'a thunk
  | Evaluated of 'a

type 'a suspension = 'a suspval ref

let delay(f : unit -> 'a) : 'a suspension =
  ref (Unevaluated f)

let rec force(x : 'a suspension) : 'a =
  match !x with
  | Evaluated y -> y
  | Unevaluated thunk ->
     let y = thunk () in
        x := Evaluated y;
        y

;;
(*********)
(* Tests *)
(*********)

(* Delay / Force *)
let lazy_delay_force_tester x = force (delay (fun () -> x))
let lazy_delay_force_tests =
  ("lazy_delay_force",
   lazy_delay_force_tester, (=), (=),
   Some(string_of_int, string_of_int),
   [
     (None, 1, Ok 1);
     (None, 42, Ok 42);
     (None, 0, Ok 0);
     (None, -1, Ok (-1));
     (None, 1000000, Ok 1000000);
     (None, 123456, Ok 123456);
  ])

(* Memoization *)
let lazy_memoize_tester x =
  let s = (delay (fun () -> x)) in
  let _ = force s in
  !s

let str_suspval f x =
  match x with
  | Evaluated y -> "Evaluated(" ^ f y ^ ")"
  | Unevaluated thunk -> "Unvaluated(thunk)"

let str_suspval_int = str_suspval string_of_int

let lazy_memoize_tests =
  ("lazy_memoize",
   lazy_memoize_tester, (=), (=),
   Some(string_of_int,  str_suspval_int),
   [
     (None, 1, Ok(Evaluated 1) );
     (None, 42, Ok(Evaluated 42) );
     (None, 0, Ok(Evaluated 0));
     (None, 42, Ok (Evaluated 42)); 
     (None, 1000000000, Ok (Evaluated 1000000000)); (* Large number test *)
     (None, 314159, Ok (Evaluated 314159)); (* Another arbitrary large number *)
  ])
