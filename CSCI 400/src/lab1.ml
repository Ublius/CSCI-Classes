open Util (* see util.ml *)

(******************)
(** Starter Code **)
(******************)

exception IndexError

(* Return the i'th element of a list
 * raise IndexError if i is out of bounds
 * *)
let rec nth (i : int)  (l: 'a list) : 'a =
  match l with (* Get the list *)
    [] -> raise IndexError  (* List is null *)
  | a::d -> if i = 0 then a  (* If no more element return ith *)
            else nth (i-1) d  (* Otherwise decrease elements *)

(* Append two lists *)
let rec append (l1 : 'a list) (l2: 'a list) : 'a list =
  match l1 with (* Get the list *)
    [] -> l2 (* If list is null return the second list *)
  | a::d -> a :: append d l2 (* Otherwise append the first element and call the function recursively *)

(* Reverse a list *)
let rec reverse_helper (l : 'a list) (r : 'a list) : 'a list =
        match l with
        |  [] -> r
        |  h::t -> reverse_helper t (h :: r)
        
let reverse (l : 'a list) : 'a list =
        reverse_helper l []

(* Length of a list*) 
(* Do not change 'let lentgth' to 'let rec length'.
 * Instead, define a helper function inside `length` 
 * that uses recursion to calculate the length of the list 
*)
let rec h (l: 'a list) (r : int) : int =
  match l with
    [] -> r
  | a::d -> h d (r+1)

let length (l : 'a list) : int  =
  h l 0

(* Return the part of list l beginning at index 0 and ending at index iend
 * raises IndexError exception when out of bounds*)
let rec list_prefix (iend : int) (l : 'a list) : 'a list =
  if iend < 0 then raise IndexError
  else if iend = 0 then []
  else
    match l with
      [] -> if iend = 0 then []
            else raise IndexError
    | a::d ->
       a :: list_prefix (iend-1) d

(* Return the part of list l beginning at istart and running through
   the end of the list *)
let rec list_suffix (istart : int) (l : 'a list) : 'a list =
  if istart < 0 then raise IndexError (* If index is less than 0 raise error *)
  else 
    match l with (* Get the list *)
      [] -> if istart = 0 then [] (* If list is null and index is 0 return null *)
            else raise IndexError
    | a::d -> 
       if istart = 0 then l (* If index is 0 return the list *)
       else list_suffix (istart-1) d (* Otherwise call the function recursively *)

(* Merge sorted lists l1 and l2 based on cmp.  The result is a sorted
   list containing all elements from both l2 and l2. *)
let rec merge_helper (cmp : 'a->'a->bool) (l1 : 'a list) (l2 : 'a list) (r : 'a list) : 'a list =
        match (l1,l2) with
        | (a::b, c::d) -> 
                        if cmp a c then merge_helper cmp b l2 (a::r)   
                        else merge_helper cmp l1 d (c::r)
        | (a::b, [] ) -> merge_helper cmp b l2 (a::r)
        | ([], c::d ) -> merge_helper cmp l1 d (c::r)
        | ([], [] ) -> reverse (r)

let rec merge (cmp : 'a->'a->bool) (l1 : 'a list) (l2 : 'a list) : 'a list =
        merge_helper cmp l1 l2 []


(* Sort list l via mergesort

   mergesort takes two inputs: a cmp function and a list l to sort.

   cmp is a function that compares two elements of list l.  When cmp
   returns true, its first argument comes first in the sorted lest.
   When cmp returns false, its second argument comes first in the
   sorted list.  *)

let rec mergesort (cmp : 'a->'a->bool) (l:'a list) : 'a list =
  match l with
  [] -> []
  | a::d -> if d = [] then a::[] else let mid = (length l) / 2 in
  let half1 = mergesort cmp (list_prefix mid l) in
  let half2 = mergesort cmp (list_suffix mid l) in
  merge cmp half1 half2

(***********)
(** Tests **)
(***********)

(* See description in testing.ml *)

let nth_tests =
  ("Nth", (*name of the test group*)
   (fun (i,l)->nth i l), (*function to be tested *)
   (=), (*what operator to use for equal *)
   (=), (*instance_destroy(id, true)
instance_destroy(other.id, true)what operator to use for equality of exceptions*)
   Some((fun x -> str_pair string_of_int str_int_list x), string_of_int),
        (*pretty printer for input and output*)
        (*the lambda function makes a string pair from the input types of the tested function
         * e.g.here the input parameters are int list, see "nth i l" *)

        (*below are the test cases as a list of tuples
         * each tuple is: testname, input, expectedOuput
         * where input is a tuple having all parameters
         * and expected output is
         * either Ok expectedResult  or Error expectedException*)
   [
     (Some("simple list"), (0, [1;2;3;4;5]), Ok 1);
     (Some("error"), (-1, [1;2;3;4;5]), Error IndexError);
     
     (Some("long list"), (7, [1;2;3;4;5;6;7;8;9;10;11;12;13;14;15]), Ok 8);
     (Some("single list"), (0, [1]), Ok 1);
     (Some("last element"), (4, [1;2;3;4;5]), Ok 5);
     (Some("error empty list"), (0, []), Error IndexError);
     (Some("error invalid index"), (9, [1;2;3;4;5]), Error IndexError);
  ])

let append_tests =
  ("append", (fun (l1,l2)->append l1 l2), (=), (=),
   Some((fun x -> str_pair str_int_list  str_int_list x),
        str_int_list),
   [
     (Some("simple list"), ([1;2],[3;4]), Ok [1;2;3;4]);

     (Some("long list"), ([1;2;3;4;5],[6;7;8;9;10]), Ok [1;2;3;4;5;6;7;8;9;10]);
     (Some("single element"), ([1],[2]), Ok [1;2]);
     (Some("empty list"), ([],[]), Ok[]);
     (Some("first empty list"), ([],[1;2;3]), Ok [1;2;3]);
     (Some("second empty list"), ([1;2;3],[]), Ok [1;2;3]);
  ])

let reverse_tests =
  ("reverse", reverse, (=), (=), Some(str_int_list,str_int_list),
   [
     (Some("simple list"), [1;2;3;4;5], Ok[5;4;3;2;1]);

     (Some("Medium List"), [5;3;100;2;10;11;19;20;21;22;23], Ok[23;22;21;20;19;11;10;2;100;3;5]);
     (Some("Long List"), [5;3;100;2;10;11;19;20;21;22;23;5;3;100;2;10;11;19;20;21;22;23], Ok[23;22;21;20;19;11;10;2;100;3;5;23;22;21;20;19;11;10;2;100;3;5]);
     (Some("Empty List"), [], Ok[]);
     (Some("Longer List"), [5;3;100;2;10;11;19;20;21;22;23;5;3;100;2;10;11;19;20;21;22;23;5;3;100;2;10;11;19;20;21;22;23;5;3;100;2;10;11;19;20;21;22;23], Ok[23;22;21;20;19;11;10;2;100;3;5;23;22;21;20;19;11;10;2;100;3;5;23;22;21;20;19;11;10;2;100;3;5;23;22;21;20;19;11;10;2;100;3;5]);
     (Some("One Element"), [1], Ok[1]);
     
  ])

let length_tests =
  ("length", length, (=), (=), Some(str_int_list,string_of_int),
   [
     (Some("simple list"), [1;2;3;4;5], Ok 5);
     
     (Some("long list"), [1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24;25], Ok 25);
     (Some("single element"), [1], Ok 1);
     (Some("empty list"), [], Ok 0);
     (Some("10 elements"), [1;2;3;4;5;6;7;8;9;10], Ok 10);
     (Some("11 elements"), [1;1;1;1;1;1;1;1;1;1;1], Ok 11);
  ])

let list_prefix_tests =
  ("list_prefix", (fun (iend,l) -> list_prefix iend l), (=), (=),
   Some((fun x -> str_pair string_of_int  str_int_list x),
        str_int_list),
   [
     (Some("simple list"), (2,[1;2;3;4;5]), Ok [1;2]);

     (Some("error less than boundary"), (-1,[1;2;3;4;5]), Error IndexError);
     (Some("error greater than length"), (6,[1;2;3;4;5]), Error IndexError);
     (Some("empty list"), (0,[]), Ok []);
     (Some("Duplicate values"), (2,[1;2;2;3;4]), Ok [1;2]);
     (Some("None with list"), (0,[1;2;3;4;5]), Ok []);
  ])

let list_suffix_tests =
  ("list_suffix", (fun (istart,l) -> list_suffix istart l), (=), (=),
   Some((fun x -> str_pair string_of_int  str_int_list x),
        str_int_list),
   [
     (Some("simple list"), (2,[1;2;3;4;5]), Ok [3;4;5]);
     (Some("error less than boundary"), (-1,[1;2;3;4;5]), Error IndexError);

     (Some("error greater than length"), (6,[1;2;3;4;5]), Error IndexError);
     (Some("empty list"), (0,[]), Ok []);
     (Some("Duplicate values"), (2,[1;2;2;3;4]), Ok [2;3;4]);
     (Some("None with list"), (0,[1;2;3;4;5]), Ok [1;2;3;4;5]);
     (Some("Long List"), (6,[1;2;3;4;5;6;7;8;9;10;11]), Ok [7;8;9;10;11]);
  ])

let merge_tests =
  ("merge", (fun (cmp,l1,l2) -> merge cmp l1 l2), (=), (=),
   Some((fun (cmp,l1,l2) -> str_pair str_int_list str_int_list (l1, l2)),
        str_int_list),
   [
     (Some("simple list"), ((<),[1;3],[2;4;5]), Ok [1;2;3;4;5]);
     (Some("simple list reverse"), ((>),[3;1],[5;4;2]), Ok [5;4;3;2;1]);

     (Some("empty lists"), ((>),[],[]), Ok[]);
     (Some("first empty list"), ((<),[],[1;2;3;4;5]), Ok[1;2;3;4;5]);
     (Some("first list empty reverse"), ((>),[],[5;4;3;2;1]), Ok[5;4;3;2;1]);
     (Some("second list empty"), ((<),[1;2;3;4;5],[]), Ok[1;2;3;4;5]);
     (Some("second list empty reverse"), ((>),[5;4;3;2;1],[]), Ok[5;4;3;2;1]);
  ])


let mergesort_tests =
  ("mergesort", (fun (cmp,l) -> mergesort cmp l), (=), (=),
   Some((fun (cmp,l) -> str_int_list l),
        str_int_list),
   [
     (Some("simple list"), ((<),[1;3;4;2;5]), Ok [1;2;3;4;5]);
     
     (Some("long list"), ((<), [1;9;51;6248;2454;14;23;6;78;124;46;3;36;7;34634;462]), Ok [1;3;6;7;9;14;23;36;46;51;78;124;462;2454;6248;34634]);
     (Some("reversed list"), ((<), [10;9;8;7;6;5;4;3;2;1]), Ok [1;2;3;4;5;6;7;8;9;10]);
     (Some("single element list"), ((<), [1]), Ok [1]);
     (Some("empty list"), ((<), []), Ok []);
     (Some("10 element shuffle"), ((<), [7;4;3;8;1;5;10;6;2;9]), Ok [1;2;3;4;5;6;7;8;9;10]);
   ])
