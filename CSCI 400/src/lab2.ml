open Util (* see util.ml *)

(******************)
(** Starter Code **)
(******************)

exception IndexError

(* Implementing higher-order functions *)

let rec map (f : 'a->'b) (l : 'a list) : 'b list =
        match l with 
        | [] -> []
        | h::t -> f h :: map f t 

let rec filter (f : 'a->bool) (l : 'a list) : 'a list =
        match l with
        | [] -> []
        | h::t -> if f h then h :: filter f t else filter f t

let rec fold_left (f: 'y ->'x->'y) (y:'y) (l:'x list) : 'y =
        match l with
        | [] -> y
        | h::t -> fold_left f (f y h) t

let rec fold_right (f : 'x->'y->'y) (y:'y) (l:'x list) : 'y =
        match l with 
        | [] -> y
        | h::t -> f h (fold_right f y t)


(* Using higher-order functions *)
let cons a b = a::b
(* Concatenate two lists. *)
let append (l1 : 'a list) (l2 : 'a list) : 'a list =
        fold_right cons l2 l1

(* rev_append l1 l2 reverses l1 and concatenates it with l2 *)
let rev r x = x::r
let rev_append (l1 : 'a list) (l2 : 'a list) : 'a list =
        let l3 = fold_left rev [] l1 in
        append l3 l2

(* Concatenate a list of lists. *)
let flatten (l : 'a list list) : 'a list =
  match l with
  | [] -> []
  | a::d -> fold_left append a d

(* Given a list, return a pair of list where each pair consists of
 * an element of the list and its repetition count in the list.
 * example: given ['a';'b';'b';'b';'a';'c';'b';'a';'c'] 
 * frequncy returns [('a',3);('b',4);('c',2)] 
 *
 * Note: The order in which pairs appear in the output list must 
 * match the order in which characters first appear in the input list.
 * example: given ['a';'b';'a'] frequency returns [('a',2);('b',1)]
 *          given ['b';'a';'a'] frequency returns [('b',1);('a',2)] *)
(*hint: fold_left*)
let frequency (l:'a list) : ('a*int) list=
  let rec increment acc x=
     match acc with
       | []->[(x,1)]
       | (a,c)::t -> if x=a then (x,c+1)::t
         else (a,c)::(increment t x)
  in 
  fold_left increment [] l


(*given a list l and an element e, returns the number of times
 * e appear in the list l. 
 * example1: count 1 [1;2;1;1;] returns 3
 * example2: count 4 [1;2;1;1;] returns 0*)
let count (e:'a) (l:'a list) : int=
  let h y x =
    if e = x then 1+y 
    else y
  in fold_left h 0 l

(* Insertion Sort *)

(* Insert an element elt into sorted list l in sorted order *)
let rec insert (cmp : 'a->'a->bool) (elt :'a) (l:'a list) : 'a list =
  match l with
  | [] -> elt::[]
  | hd::tl when cmp elt hd -> elt::l
  | hd::tl -> hd::insert cmp elt tl

let insertionsort (cmp : 'a->'a->bool) (l:'a list) : 'a list =
  fold_left (fun acc x -> insert cmp x acc) [] l

(* Selection Sort *)

(* Select the initial element from l based on cmp.  Return a tuple of
   the initial element and the list with the initial element
   removed. *)
let rec select_helper (cmp : 'a->'a->bool) (init: 'a) (l:'a list) (x: 'a list) : 'a * 'a list =
        match l with
        | [] -> (init, x)
        | a::d -> if cmp a init then select_helper cmp a d (append x (init::[]))
        else select_helper cmp init d (append x (a::[]))
let select (cmp : 'a->'a->bool) (l:'a list) : 'a * 'a list =
        match l with
        | [] -> raise IndexError
        | a::d -> (select_helper cmp a d [])

let rec selectionsort (cmp : 'a->'a->bool) (l:'a list) : 'a list =
        match l with
        [] -> []
       | a::d -> begin match select cmp l with
           (b, e) -> b::selectionsort cmp e
       end

(* Quicksort *)

(* Partion list l around elt.  Return a tuple consisting of all
   elements before elt and all elements after elt. *)
let pivot (cmp : 'a->'a->bool) (elt :'a) (l:'a list) : 'a list * 'a list =
  let rec helper left right l = 
      match l with
      | [] -> (left, right)
      | h::t -> 
           if cmp h elt then helper (h::left) right t
           else helper left (h::right) t
  in
  helper [] [] l



(* The simple implementation of quicksort recurses on the two sublists
   and appends the sorted results. *)
let rec quicksort_simple (cmp : 'a->'a->bool) (l : 'a list) : 'a list =
  (*TODO*)
  match l with
  | [] -> []
  | h::t -> 
      let left, right = pivot cmp h t in
      (quicksort_simple cmp left) @ [h] @ (quicksort_simple cmp right)




(* The better implementation of quicksort elides the append by passing
   a "tail" list to recursive calls.  Sorted results are directly
   cons'ed onto the tail, avoiding the need for an extra append. *)
let quicksort_better (cmp : 'a->'a->bool) (l : 'a list) : 'a list =
  let rec f (cmp : 'a->'a->bool) (l : 'a list) (r : 'a list) : 'a list =
    (* r is the tail: everything that must come after l in the sorted
       list. Passing r to f saves us from having to append sorted
       lists. *)
    match l with
    | [] -> r
    | h::t ->
         let left, right = pivot cmp h t in
         f cmp left (h :: f cmp right r)
  in f cmp l []

(***********)
(** Tests **)
(***********)

(* See description in testing.ml *)

let list_cmp cmp l1 l2 =
  (List.sort cmp l1) = (List.sort cmp l2)

let int_list_cmp l1 l2 =
  list_cmp (-) l1 l2


let map_tests =
  ("map", (fun (f,l)->map f l), (=), (=),
   Some((fun (f,l) -> str_int_list l),
        str_int_list),
   [
     (Some("simple list"), ((fun x -> 1+x), [1;2;3;4;5]), Ok [2;3;4;5;6]);

     (Some("Double long list"), ((fun x -> 2*x), [1;2;3;4;5;6;7;8;9;10;11;12;13;14;15]), Ok [2;4;6;8;10;12;14;16;18;20;22;24;26;28;30]);
     (Some("Empty list"), ((fun x -> 1+x), []), Ok []);
     (Some("Division test"), ((fun x -> x / 5), [5;10;15;20;25;30;35;40;45;50]), Ok [1;2;3;4;5;6;7;8;9;10]);
     (Some("Square test"), ((fun x -> x*x), [2;4;5;7;9]), Ok [4;16;25;49;81]);
     (Some("Multiple operations"), ((fun x -> x*5 + 9 * 2 - 3), [1;2;3;4]), Ok [20;25;30;35]);
  ])

let filter_tests =
  ("filter", (fun (f,l)->filter f l), (=), (=),
   Some((fun (f,l) -> str_int_list l),
        str_int_list),
   [
     (Some("simple list"), ((fun x -> (x mod 2)=0), [1;2;3;4;5]), Ok [2;4]);

     (Some("Long list"), ((fun x-> (x+1) > 10), [1;10;9;5;21;53;4;36;7;4;2;5;6;74;2;25;6]), Ok [10;21;53;36;74;25]);
     (Some("Empty list"), ((fun x -> (x mod 2) = 0), []), Ok []);
     (Some("No elements pass"), ((fun x -> ((x*2) mod 2) = 1),[1;2;3;4;5;6;7;8;9]), Ok []);
     (Some("Every element passes"), ((fun x -> ((x*2) mod 2) = 0), [1;2;3;4;5;6;7;8;9]), Ok [1;2;3;4;5;6;7;8;9]);
     (Some("remove 0"), ((fun x -> x <> 0), [0;2;0;3;5;0;0;0;5;0;1;0;6;7;5;4;0;9]), Ok [2;3;5;5;1;6;7;5;4;9]);
  ])

let fold_left_tests =
  ("fold_left", (fun (f,y,l)->fold_left f y l), (=), (=),
   Some((fun (f,y,l) -> str_pair string_of_int str_int_list (y,l)),
        string_of_int),
   [
     (Some("+"), ((+), 0, [1;2;3]), Ok 6);
     (Some("-"), ((-), 0, [1;2;3]), Ok (-6));

     (Some("*"), (( * ), 1, [1;2;3;4;5]), Ok 120);
     (Some("/"), ((/), 200, [2;5;4]), Ok 5);
     (Some("Empty list"), ((+), 0, []), Ok 0);
     (Some("Count >0"), ((fun a x -> if x > 0 then a+1 else a), 0, [(-3);(-4);1;7;(-4);2;(-314);6;4;(-3);5]), Ok 6);
     (Some("Long list addition"), ((+), 0, [1;1;3;-9;4;6;2;7;6;5;-3;0;0;4;2;6;5;3]), Ok 43);
  ])

let fold_right_tests =
  ("fold_right", (fun (f,y,l)->fold_right f y l), (=), (=),
   Some((fun (f,y,l) -> str_pair string_of_int str_int_list (y,l)),
        string_of_int),
   [
     (Some("+"), ((+), 0, [1;2;3]), Ok 6);
     (Some("-"), ((-), 0, [1;2;3]), Ok 2);

     (Some("*"), (( * ), 1, [1;2;3;4;5]), Ok 120);
     (Some("/"), ((/), 2, [200;600;24;8]), Ok 2);
     (Some("Empty list"), ((+), 0, []), Ok 0);
     (Some("Count > 0"), ((fun x a -> if x > 0 then a+1 else a), 0, [(-3);(-4);1;7;(-4);2;(-314);6;4;(-3);5]), Ok 6);
     (Some("Long list addition"), ((+), 0, [1;1;3;-9;4;6;2;7;6;5;-3;0;0;4;2;6;5;3]), Ok 43);
  ])


let append_tests =
  ("append", (fun (l1,l2)->append l1 l2), (=), (=),
   Some((fun x -> str_pair str_int_list  str_int_list x),
        str_int_list),
   [
     (Some("simple list"), ([1;2],[3;4]), Ok [1;2;3;4]);

     (Some("long lists"), ([1;2;3;4;5;6;7;8;9;10],[1;2;3;4;5;6;7;8;9;10]), Ok [1;2;3;4;5;6;7;8;9;10;1;2;3;4;5;6;7;8;9;10]);
     (Some("First empty"), ([],[1;2;3;4;5]), Ok [1;2;3;4;5]);
     (Some("Second empty"), ([1;2;3;4;5],[]), Ok [1;2;3;4;5]);
     (Some("Both single elements"), ([1],[2]), Ok [1;2]);
     (Some("Both empty"), ([],[]), Ok []);
  ])

let rev_append_tests =
  ("rev_append", (fun (l1,l2)->rev_append l1 l2), (=), (=),
   Some((fun x -> str_pair str_int_list  str_int_list x),
        str_int_list),
   [
     (Some("simple list"), ([1;2],[3;4]), Ok [2;1;3;4]);

     (Some("First empty"), ([],[1;2;3]), Ok [1;2;3]);
     (Some("Second empty"), ([1;2;3],[]), Ok [3;2;1]);
     (Some("Both empty"), ([],[]), Ok []);
     (Some("Both single elements"), ([1],[2]), Ok [1;2]);
     (Some("Long lists"), ([1;2;3;4;5;6;7;8;9;10],[1;2;3;4;5;6;7;8;9;10]), Ok [10;9;8;7;6;5;4;3;2;1;1;2;3;4;5;6;7;8;9;10]);
   ]
  )

let flatten_tests =
  ("flatten", (fun l -> flatten l), (=), (=),
   Some((fun l -> "[" ^ str_x_list (str_int_list) l ";" ^ "]" ),
        str_int_list),
   [
     (Some("simple list"), [[1;2];[3;4]], Ok [1;2;3;4]);
     (Some("simple list 2"), [[3;4]; [1;2]], Ok [3;4;1;2]);

     (Some("Long list"), [[1;2;3;4];[5;6;7;8];[9;10;11;12];[1;2;3;4;5;6;7;8]], Ok [1;2;3;4;5;6;7;8;9;10;11;12;1;2;3;4;5;6;7;8]);
     (Some("Empty list"), [], Ok []);
     (Some("Single element list"), [[1;2;3;4]], Ok [1;2;3;4]);
     (Some("Second empty"), [[1;2;3;4;5];[];[];[]], Ok [1;2;3;4;5]);
     (Some("First empty"), [[];[];[];[1;2;3;4;5]], Ok [1;2;3;4;5]);
   ]
  )

let str_char c = Char.escaped c
let str_char_list (l:char list) : string =
  "["^(str_x_list str_char l "; ")^"]"

let frequency_tests=
 ("frequency", (fun l-> frequency l), (=),(=),
  Some((fun l -> str_char_list l), 
      (fun a -> str_x_list (fun (s,l) -> str_pair str_char string_of_int (s,l)) a ";")
     ),
[
(Some("frequency list 3 elem"), ['a';'b';'b';'b';'a';'c';'b';'a';'c'], Ok [('a',3);('b',4);('c',2)]);

(Some("frequency list 1 elem"), ['a';'a';'a';'a';'a';'a';'a';'a';'a';'a'], Ok [('a',10)]);
(Some("frequency 0 elem"), [], Ok []);
(Some("frequency elements in reverse order"), ['d'; 'c'; 'b'; 'a'; 'b'; 'c'; 'd'; 'a'], Ok [('d',2); ('c',2); ('b',2); ('a',2)]);
(Some "frequency many unique elements", ['a'; 'b'; 'c'; 'd'; 'e'; 'f'; 'g'; 'h'; 'i'; 'j'], Ok [('a', 1); ('b', 1); ('c', 1); ('d', 1); ('e', 1); ('f', 1); ('g', 1); ('h', 1); ('i', 1); ('j', 1)]);
(Some "frequency single character once", ['a'], Ok [('a', 1)]);
]
)

let count_tests =
  ("count", (fun (i,l)->count i l), (=), (=), 
   Some((fun x -> str_pair string_of_int str_int_list x), string_of_int),
   [
     (Some("simple list"), (1, [1;2;1;1;3]), Ok 3);

     (Some("long list"), (1, [1;2;1;1;3;1;1;3;1;1;3;8;2;3;4;1;1;4;3;67;10;453;1;1;2;1;3;4;1]), Ok 13);
     (Some("no elements"), (0, [1;2;3;4;5]), Ok 0);
     (Some("negative elements"), (-1, [-1;-1;1;2;3;4]), Ok 2);
     (Some("empty list"), (1, []), Ok 0);
     (Some("count 0"), (0, [0;0;0;0;0;0;0;0]), Ok 8);
  ])



let sort_test_cases = [
    (Some("simple list"), ((<),[1;3;4;2;5]), Ok [1;2;3;4;5]);

    (Some("long list"), ((>), [7;14;3;19;10;1;17;12;6;8;15;2;20;9;5;11;16;4;13;18]), Ok [20;19;18;17;16;15;14;13;12;11;10;9;8;7;6;5;4;3;2;1]);
    (Some("reverse list"), ((<), [10;9;8;7;6;5;4;3;2;1]), Ok [1;2;3;4;5;6;7;8;9;10]);
    (Some("empty list"), ((>), []), Ok []);
    (Some("negative and positive"), ((<),[-1;1;-1;1;-1;1;-1;1;-1;1]), Ok [-1;-1;-1;-1;-1;1;1;1;1;1]);
    (Some("single element list"), ((>), [10]), Ok [10]);
  ]

let insert_tests =
  ("insert", (fun (cmp,elt,l)->insert cmp elt l), (=), (=),
   Some(((fun (cmp,elt,l) -> str_pair string_of_int str_int_list (elt,l)),
         (fun y -> str_int_list y)
     )),
   [
     (Some("simple <"), ((<), 0, [-1;1;2]), Ok ([-1; 0; 1; 2]));
     (Some("simple >"), ((>), 0, [2;1;-1]), Ok ([2; 1; 0; -1]));

     (Some("Insert at end"), ((<), 10, [1;2;3;4;5;6;7;8;9]), Ok ([1;2;3;4;5;6;7;8;9;10]));
     (Some("Insert at beginning"), ((>), 10, [1;2;3;4;5;6;7;8;9]), Ok ([10;1;2;3;4;5;6;7;8;9]));
     (Some("Insert duplicate"), ((<), 3, [1; 2; 3; 3; 4]), Ok ([1; 2; 3; 3; 3; 4]));
     (Some("Insert into empty"), ((<), 3, []), Ok ([3]));
     Some("Insert negative value"), ((<), -1, [0; 1; 2; 3]), Ok ([-1; 0; 1; 2; 3]);
   ])

let insertionsort_tests =
  ("insertionsort", (fun (cmp,l) -> insertionsort cmp l), (=), (=),
   Some((fun (cmp,l) -> str_int_list l),
        str_int_list),
   sort_test_cases)


let select_test_eq (s1,l1) (s2,l2) =
  (s1 = s2) && (int_list_cmp l1 l2)

let select_tests =
  ("select", (fun (cmp,l)->select cmp l), select_test_eq, (=),
   Some(((fun (cmp,l) -> str_int_list l),
         (fun (s,l) -> str_pair string_of_int str_int_list (s,l))
     )),
   [
     (Some("simple <"), ((<), [1;-1;2]), Ok (-1,[2;1]));
     (Some("simple >"), ((>), [1;-1;2]), Ok (2,[1;-1]));

     (Some("Get first occurance"), ((<), [1;2;1;1;1;1;1;1;1;1]), Ok (1,[2;1;1;1;1;1;1;1;1;]));
     (Some("Long list"), ((>), [1;4371;71;5715;7;5;415;5;653;224;54;16;78;6344;362;234;5]), Ok (6344, [1;71;4371;7;5;415;5;653;224;54;16;78;5715;362;234;5]));
     (Some("Empty list"), ((<), []), Error IndexError);
     (Some("Reversed list"), ((<), [8;7;6;5;4;3;2;1]), Ok (1, [8;7;6;5;4;3;2]));
     (Some("Element carry"), ((>), [1;6;3;4;7;2;5;8;12;9;8;7;14;0;3;4;15]), Ok (15, [1;3;4;6;2;5;7;8;9;8;7;12;0;3;4;14]));
   ])


let selectionsort_tests =
  ("selectionsort", (fun (cmp,l) -> selectionsort cmp l), (=), (=),
   Some((fun (cmp,l) -> str_int_list l),
        str_int_list),
   sort_test_cases)

let pivot_test_eq (a1,b1) (a2,b2) =
  (int_list_cmp a1 a2) && (int_list_cmp b1 b2)

let pivot_tests =
  ("pivot", (fun (cmp,elt,l)->pivot cmp elt l), pivot_test_eq, (=),
   Some(((fun (cmp,elt,l) -> str_pair string_of_int str_int_list (elt,l)),
         (fun y -> str_pair str_int_list  str_int_list y)
     )),
   [
     (Some("simple <"), ((<), 0, [-1;1;0;-2; 2]), Ok ([-2; -1],[2; 0; 1]));
     (Some("simple >"), ((>), 0, [-1;1;0;-2; 2]), Ok ([2; 1], [-2; 0; -1]));

     (Some("All Zeros >"), ((>), 0, [0;0;0;0;0]), Ok ([], [0; 0; 0; 0; 0]));
     (Some("Repeated Pivot <"), ((<), 500, [500;500;501;501; 502;499;]), Ok ([499], [502;501;501;500;500]));
     (Some("Empty list with Selected pivot"), ((<), 3, []), Ok ([], []));
     (Some("Single Element List Greater"), ((>), 0, [3]), Ok ([3], []));
     (Some("Single Element List Lesser"), ((<), 0, [3]), Ok ([], [3]));
  ])

let quicksort_simple_tests =
  ("quicksort_simple", (fun (cmp,l) -> quicksort_simple cmp l), (=), (=),
   Some((fun (cmp,l) -> str_int_list l),
        str_int_list),
   sort_test_cases)

let quicksort_better_tests =
  ("quicksort_better", (fun (cmp,l) -> quicksort_better cmp l), (=), (=),
   Some((fun (cmp,l) -> str_int_list l),
        str_int_list),
   sort_test_cases)
