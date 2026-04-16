open Util (* see util.ml *)
open Lab4_lazy

(****************)
(* Starter Code *)
(****************)

module Stream = struct
  type 'a cell =
    | Cons of 'a * 'a t
    | Nil
  and 'a t = ('a cell) suspension

  exception IndexError

  (* Return the head element of the list *)
  let head(l : 'a t) : 'a  =
      match force l with
        | Cons (h, _) -> h
        | Nil -> raise IndexError

  (* Return the tail of the list *)
  let tail(l : 'a t) : 'a t  =
      match force l with
        | Cons (_,t) -> t
        | Nil -> raise IndexError       

  let nil()  = delay(fun () -> Nil)

  (* Create stream from the elements of a list *)
  let rec from_list (l : 'a list) : 'a t =
    match l with
    | [] -> nil()
    | h :: t -> delay(fun () -> Cons(h, from_list t))

  (* Fold function f over the stream from the right (end) of the stream. *)
  (* fold_right is a monolithic function *)
  let rec fold_right (f : 'a->'acc->'acc) (s : 'a t) (acc : 'acc) : 'acc =
    match force s with
    | Nil -> acc
    | Cons (h,t) -> f h (fold_right f t acc)

  let rec to_list s =
    fold_right (fun x l -> x :: l) s []

  (* Create a stream by applying function f to produce successive elements. *)
  (* applying f to element i will produce element i+1 *)
  let rec seed (f : 'a -> 'a) (x : 'a) =
    delay (fun () -> Cons (x, seed f (f x)))

  (* Create a stream by applying function f to produce successive elements. *)
  (*Given a function f and a pair (x, y), the seed2 function generates a stream starting with x. 
  Each subsequent element in the stream is derived by applying f to the current pair (x, y) to produce the next pair, and so forth. *)
  let rec seed2 (f : ('a*'b) -> ('a*'b)) ((x,y) : ('a *'b)) =
    delay (fun () -> Cons (x, seed2 f (f (x, y))))

  (* Append two streams *)
  let rec append (s1 : 'a t) (s2 : 'a t) : 'a t =
    delay (fun () ->
      match force s1 with
      | Nil -> force s2
      | Cons (h,t) -> Cons (h, append t s2)
    )

  (* Reverse a stream. *)
  (* reverse is a monolithic function *)
  let reverse (s : 'a t) : 'a t =
    let rec h acc s =
      match force s with
      | Nil -> acc 
      | Cons (a, t) -> h (delay (fun () -> Cons (a, acc))) t 
    in
    h (nil()) s 

  let rec nth(l : 'a t) (n:int) : 'a =
    if 0 = n
    then head l
    else nth (tail l) (n-1)

  (* Return a stream containing the first n elements of the input stream *)
  let rec prefix (l:'a t) (n:int) : 'a t =
    if n <= 0 then nil()
    else delay(fun () -> Cons(head l, prefix (tail l) (n-1)))

  let rec suffix (l:'a t) (n:int) : 'a t =
    if n <= 0 then l
    else suffix (tail l) (n-1)

  let rec sublist (l:'a t) (start:int) (stop:int) : 'a list =
    to_list (prefix (suffix l start) (stop - start))

  (* Map a function over a stream *)
  let rec map (f : 'x -> 'y) (l : 'x t): 'y t =
        match force l with
        | Nil -> delay(fun() -> Nil)
        | _ ->
        let map_helper () =
                let h = head l in
                let t = tail l in
                let w = map f t in
                Cons(f h, w)
        in
        delay map_helper

  (* Map a function over two streams *)
  let rec map2 (f : 'x -> 'y -> 'z) (l1 : 'x t) (l2 : 'y t) : 'z t =
        match (force l1), (force l2) with
                | Nil, _ | _, Nil -> delay(fun() -> Nil)
                | _ ->
        let map2_help () = 
                let h1 = head l1 in
                let h2 = head l2 in
                let t1 = tail l1 in
                let t2 = tail l2 in
                let w = map2 f t1 t2 in
                Cons((f h1 h2), w)
        in
        delay map2_help

  (* Filter a stream *)
  let rec filter  (f : 'x -> bool) (l : 'x t) : 'x t =
          match force l with
          | Nil -> delay(fun() -> Nil)
          | _ ->
          let filter_helper = 
                  match force l with
                  | Nil -> Nil
                  | Cons(a, b) ->
                                  if f a then
                                          Cons(a, filter f b)
                                  else force (filter f b)
          in
          delay (fun() -> filter_helper)
end

;;
(*********)
(* Tests *)
(*********)

(* to/from list *)
let stream_fromto_list_tester x = Stream.to_list (Stream.from_list x)
let stream_fromto_list_printer = Some(str_int_list,str_int_list)
let stream_fromto_list_tests =
  ("stream_fromto_list",
   stream_fromto_list_tester, (=), (=),
   stream_fromto_list_printer,
   [
     (None, [1;2;3], Ok [1;2;3]);
     (None, [1], Ok [1]);
     (None, [], Ok []);
     (None, [0; -1; -2; -3], Ok [0; -1; -2; -3]); (* Handles negative numbers *)
     (None, [5; 10; 15; 20], Ok [5; 10; 15; 20]); (* Handles spaced increments *)
     (None, [1000000; 2000000; 3000000], Ok [1000000; 2000000; 3000000]); (* Large numbers *)
     (None, [1; 2; 3; 2; 1], Ok [1; 2; 3; 2; 1]); (* Handles duplicate values *)
     (None, [42], Ok [42]); (* Single element test case *)
  ])

(* seed tests *)
let stream_seed_tester (f,x,n) = Stream.to_list (Stream.prefix (Stream.seed f x) n)
let stream_seed_printer = Some((fun (f,x,n) -> string_of_int x), str_int_list)
let stream_seed_tests =
  ("seed",
   stream_seed_tester, (=), (=),
   stream_seed_printer,
   [
     (Some "nats", ((fun x -> 1+x), 0, 5), Ok [0;1;2;3;4]);
     (Some "xorshift32",
      ((fun x ->
        let bits32 = 0xffffffff in
        let x = ((x lsl 13) lxor x) land bits32 in
        let x = ((x lsr 17) lxor x) land bits32 in
        let x = ((x lsl  5) lxor x) land bits32 in
        x),
       42, 5),
      Ok [42; 11355432; 2836018348; 476557059; 3648046016]);

     (Some "zeno", ((fun x -> x / 2), 256, 5), Ok [256;128;64;32;16]);
     (Some "evens", ((fun x -> x + 2), 0, 6), Ok [0; 2; 4; 6; 8; 10]); 
     (Some "powers_of_2", ((fun x -> x * 2), 1, 6), Ok [1; 2; 4; 8; 16; 32]); 
     (Some "collatz", ((fun x -> if x mod 2 = 0 then x / 2 else 3 * x + 1), 6, 7), 
       Ok [6; 3; 10; 5; 16; 8; 4]);
  ])

let stream_seed2_tester (f,xi,n) = Stream.to_list (Stream.prefix (Stream.seed2 f xi) n)
let stream_seed2_printer = Some((fun (f,xi,n) -> str_pair string_of_int string_of_int xi),
                                str_int_list)
let stream_seed2_tests =
  ("seed2",
   stream_seed2_tester, (=), (=),
   stream_seed2_printer,
   [
     (Some "fact", ((fun (x,i) -> (x*i,i+1)), (1,1), 5), Ok [1; 1; 2; 6; 24]);

     (Some "fib", ((fun (x,xm) -> (x + xm,x)), (0,1), 8), Ok[0; 1; 1; 2; 3; 5; 8; 13] );
     (Some "lucas_seq", ((fun (x, xm) -> (xm, x + xm)), (2, 1), 7), Ok [2; 1; 3; 4; 7; 11; 18]);
     (Some "arith_geo", ((fun (a, r) -> (a * r, r + 1)), (3, 2), 6), Ok [3; 6; 18; 72; 360; 2160]);

  ])

(* append *)

let stream_append_tester (a,b) =
  Stream.to_list(Stream.append
                    (Stream.from_list a)
                    (Stream.from_list b))
let stream_append_printer = Some(str_pair str_int_list str_int_list, str_int_list)
let stream_append_tests =
  ("stream_append",
   stream_append_tester, (=), (=),
   stream_append_printer,
   [
     (None, ([1;2;3],[4;5;6]), Ok [1;2;3;4;5;6]);

     (Some "Empty with non empty", ([], [4;5;6]), Ok [4;5;6]);
     (Some "Non empty with empty", ([1;2;3], []), Ok [1;2;3]);
     (Some "Single Element", ([1], [2]), Ok [1;2]); 
     (Some "Different lengths", ([1; 2; 3], [4; 5; 6; 7; 8]), Ok [1; 2; 3; 4; 5; 6; 7; 8]);
  ])


(* reverse *)
let stream_reverse_tester x = Stream.to_list (Stream.reverse (Stream.from_list x))
let stream_reverse_printer = Some(str_int_list,str_int_list)
let stream_reverse_tests =
  ("stream_reverse_list",
   stream_reverse_tester, (=), (=),
   stream_reverse_printer,
   [
     (None, [1;2;3], Ok [3;2;1]);

     (Some "Empty", [], Ok []); 
     (Some "Long List", [1; 2; 3; 4; 5], Ok [5; 4; 3; 2; 1]); 
     (Some "Duplicate Elements", [0; 0; 0], Ok [0; 0; 0]); 
     (Some "Single Element", [1], Ok [1]);
  ])


(* nth *)
let stream_nth_tester (x,n) = Stream.nth (Stream.from_list x) n
let stream_nth_printer = Some(str_pair str_int_list string_of_int,string_of_int)
let stream_nth_tests =
  ("stream_nth",
   stream_nth_tester, (=), (=),
   stream_nth_printer,
   [
     (None, ([0;1;2],1), Ok 1);
     (None, ([1;2;3],2), Ok 3);
     (None, ([1;2;3],3), Error Stream.IndexError);
     (None, ([],1), Error Stream.IndexError);
     (None, ([42],0), Ok 42);
  ])

(* prefix *)
let stream_prefix_tester (x,n) = Stream.to_list (Stream.prefix (Stream.from_list x) n)
let stream_prefix_printer = Some(str_pair str_int_list string_of_int,str_int_list)
let stream_prefix_tests =
  ("stream_prefix",
   stream_prefix_tester, (=), (=),
   stream_prefix_printer,
   [
     (None, ([0;1;2],2), Ok [0;1]);

     (Some "Long List", ([1; 2; 3; 4; 5], 3), Ok [1; 2; 3]); 
     (Some "More Elements", ([10; 20; 30], 5), Error Stream.IndexError); 
     (Some "Empty", ([], 2), Error Stream.IndexError); 
     (Some "First", ([5; 10; 15], 1), Ok [5]);
  ])

(* suffix *)
let stream_suffix_tester (x,n) = Stream.to_list (Stream.suffix (Stream.from_list x) n)
let stream_suffix_printer = Some(str_pair str_int_list string_of_int,str_int_list)
let stream_suffix_tests =
  ("stream_suffix",
   stream_suffix_tester, (=), (=),
   stream_suffix_printer,
   [
     (None, ([0;1;2],0), Ok [0;1;2]);

     (None, ([0;1;2],1), Ok [1;2]);  (* Removes the first element *)
     (None, ([0;1;2],2), Ok [2]);  (* Removes the first two elements *)
     (None, ([0;1;2],3), Ok []);  (* Removes all elements, should return an empty list *)
  ])

(* map *)
let stream_map_tester (f,x) = Stream.to_list (Stream.map f (Stream.from_list x))
let stream_map_printer = Some((fun (f,x) -> str_int_list x), str_int_list)
let stream_map_tests =
  ("stream_map",
   stream_map_tester, (=), (=),
   stream_map_printer,
   [
     (None, ((fun x->1+x), [0;1;2]), Ok [1;2;3]);
     (Some("Double"), ((fun x -> 2 * x), [0;1;2;3;4;5]), Ok [0;2;4;6;8;10]);
     (Some("Division"), ((fun x -> 100 / x), [1;2;4;5;20]), Ok [100;50;25;20;5]);
     (Some("Empty"), ((fun x -> x+1), []), Ok []);
  ])

(* map2 *)
let stream_map2_tester (f,x,y) = Stream.to_list (Stream.map2 f (Stream.from_list x) (Stream.from_list y))
let stream_map2_printer = Some((fun (f,x,y) -> str_pair str_int_list str_int_list (x,y)), str_int_list)
let stream_map2_tests =
  ("stream_map2",
   stream_map2_tester, (=), (=),
   stream_map2_printer,
   [
     (None, ((+), [0;1;2], [4;5;6]), Ok [4;6;8]);
     (Some("Multiply"), (( * ), [0;1;2;3;4;5], [0;1;2;3;4;5]), Ok [0;1;4;9;16;25]);
     (Some("Subtraction"), (( - ), [10;9;8;7;6;5;4;3;2;1], [9;8;7;6;5;4;3;2;1;0]), Ok [1;1;1;1;1;1;1;1;1;1]);
     (Some("Empty"), ((+), [], []), Ok []);
  ])

(* filter *)
let stream_filter_tester (f,x) = Stream.to_list (Stream.filter f (Stream.from_list x))
let stream_filter_printer = Some((fun (f,x) -> str_int_list x), str_int_list)
let stream_filter_tests =
  ("stream_filter",
   stream_filter_tester, (=), (=),
   stream_filter_printer,
   [
     (None, ((fun x-> (x mod 2) = 0), [0;1;2;3;4;5;6]), Ok [0;2;4;6]);
     (* TODO *)
     (Some("Odds"), ((fun x -> (x mod 2) = 1), [0;1;2;3;4;5;6]), Ok [1;3;5]);
     (Some("Greater than 10"), ((fun x -> (x - 10) > 0), [10;14;2;20;4;5;77;32;4;0;-12;12]), Ok [14;20;77;32;12]);
     (Some("Empty"), ((fun x -> (x mod 2) = 0), []), Ok []);
  ])
