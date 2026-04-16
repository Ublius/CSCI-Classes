open Util (* see util.ml *)

(******************)
(** Starter Code **)
(******************)

(****************************************)
(* PART I: BST and Arithmetic Evaluator *)
(****************************************)

(*
 * REMINDER:
 *  you can nest pattern matching
 *      example:
 *      match x with
 *      | Some (a,b) -> (match a with
 *                   |[] -> []
 *                   |x::xs ->xs)   (*notice the paranthesis*)
 *      | None -> []
 *  you can define your own helper functions
 *  you can define your own local functions
 *  you can use any of the function defined in this file and other files of Lab3
 *  including util.ml and testing.ml
 *  you cannot use the List library, such as List.map and @ for append
 * *)


type bop = Add | Sub | Mul | Div

type maybeint=int option   
(* None is a value of type maybeint; Some 1 is a value of type maybeint*)
(*
 * this type definition is similar to having
type maybeint=None | Some of int 
 *)



(* a recursive expression type that is
 * either a number of maybeint
 * or a binary expression consisting of a binary operator (bop) and its two operands. 
 * the operands are of type expression
 * so that we can represent complex arithmetic expression.
 * *)
type exp = Num of maybeint| BinExp of (bop* exp *exp) 

let not_value=Num None


(* Evaluate the expression 
 * Given expression,evaluates the operands recursively and then applies the binop.
 * returns either None or Some int
 * Division by zero causes None
 * *)
let rec exp_eval (e : exp) : maybeint =
  match e with
  | Num x -> x
  | BinExp (bop, x, y) -> 
        match exp_eval x with
        | None -> None
        | Some x_value -> match exp_eval y with
                | None -> None
                | Some 0 -> None
                | Some y_value -> match bop with
                        | Add -> Some (x_value + y_value)
                        | Sub -> Some (x_value - y_value)
                        | Mul -> Some (x_value * y_value)
                        | Div -> Some (x_value / y_value)

(* Convert the expression to a string.  
 * Use ^ to concat strings. 
 * Given a maybeint, extract the int part and convert the int to string
 * using string_of_int 
 * For example, When Num Some x is given , string_of_int x returns a string
 * For Num None, return the string "NaN"
 * To convert a bop to string, use one of  "+", "-", "*", "/"
 * The string must be fully parenthesized and contain no whitespace. *)
let rec exp_string (e : exp) : string =
  match e with
  | Num x -> (match x with 
        | None -> "NaN"
        | Some value -> string_of_int value)
  | BinExp (bop, x, y) -> 
                  let x_recurse = exp_string x in 
                  let y_recurse = exp_string y in
                  match bop with
                        | Add -> "("^x_recurse^"+"^y_recurse^")"
                        | Sub -> "("^x_recurse^"-"^y_recurse^")"
                        | Mul -> "("^x_recurse^"*"^y_recurse^")"
                        | Div -> "("^x_recurse^"/"^y_recurse^")"


(***************************)
(*** Binary Search Trees ***)
(***************************)

(* Binary Tree Data Type *)
(* 'v is the type of values in the tree *)
type 'v binary_tree =
  | Empty
  | Node of 'v binary_tree * 'v * 'v binary_tree

(* Result of comparing two values *)
(* if a < b, then cmp a b -> Lesser *)
(* if a = b, then cmp a b -> Equal *)
(* if a > b, then cmp a b -> Greater *)
type cmp_result =
  | Lesser | Equal | Greater

(* compare two data elements of type 'v *)
type 'v cmp_fun = 'v -> 'v -> cmp_result

(* Higher Order Functions on Binary Trees *)

(* Apply function f to every data element of tree t and collect the
   results in a list following an inorder traversal of the tree
*)
let rec inorder_helper (f : 'v -> 'a) (tree : 'v binary_tree) (accumulator : 'v list) =
  match tree with
  | Empty -> accumulator
  | Node (left, value, right) ->
        let accumulator_right = inorder_helper f right accumulator in
        let accumulator_value = (f value) :: accumulator_right in 
        inorder_helper f left accumulator_value

let map_inorder (f : 'v -> 'a) (t : 'v binary_tree) : 'a list =
        inorder_helper f t []
   


(* Apply function f to every data element of tree t and 
 * return a new tree with 
 * data elements that are results of the function application.
 * It is ok if the resulting tree is not preserving the BST invariant.
 * *)
let rec map_tree_helper (f : 'v -> 'a) (t : 'v binary_tree) : 'a binary_tree =
   match t with
   | Empty -> Empty
   | Node (left, value, right) ->
        Node ( map_tree_helper f left, f value, map_tree_helper f right)

let map_tree (f : 'v -> 'a) (t : 'v binary_tree) : 'a binary_tree=
   map_tree_helper f t


(* Binary Search Trees *)

(* Test if t is a binary search tree.
 *
 * That is, (recursively) are all elements to the left less the the
 * current element and all elements to the right greater than the
 * current element 
 * see the ADT activity slides for help
 * *)
let is_bst (cmp : 'v cmp_fun) (t : 'v binary_tree) : bool =
  let rec helper tree lower upper =
    match tree with
    | Empty -> true
    | Node (left, value, right) ->
        let ok_lower =
          match lower with
          | None -> true
          | Some low -> cmp low value = Lesser  
        in
        let ok_upper =
          match upper with
          | None -> true
          | Some up -> cmp value up = Lesser   
        in
        ok_lower && ok_upper &&
        helper left lower (Some value) &&   
        helper right (Some value) upper    
  in
  helper t None None



(* Return the maximum element of a binary search tree. *)
let rec bst_max (t : 'v binary_tree) : 'v option =
  match t with
    Empty -> None
  | Node(l, d, Empty) -> Some d
  | Node(_, _, r) -> bst_max r

(* Return the minimum element of a binary search tree. *)
let rec bst_min t : 'v option =
  match t with
    Empty -> None
  | Node(Empty, d, _) -> Some d
  | Node(l, _, _) -> bst_min l

(* Insert element x into binary search tree t.
 *
 * Do not reinsert (duplicate) existing elements 
 * *)
let rec bst_insert (cmp : 'v cmp_fun) (t : 'v binary_tree) (x : 'v) : 'v binary_tree =
  match t with
   | Empty -> Node(Empty, x, Empty)
   | Node(l,d,r) ->
      (match cmp x d with
       | Lesser -> Node(bst_insert cmp l x, d, r)
       | Greater -> Node(l, d, bst_insert cmp r x)
       | Equal -> t)  

(* Search for element x in binary search tree t.
 *
 * Return true if the tree contains x and false if it does not. *)
let rec bst_search (cmp : 'v cmp_fun) (t : 'v binary_tree) (x : 'v) : bool =
  match t with
    Empty -> false
  | Node(l,d,r) ->
     (match cmp x d with
      | Lesser -> bst_search cmp l x
      | Greater -> bst_search cmp r x
      | Equal -> true)


(* Remove the minimum element of binary search tree t.
 *
 * Return a tuple containing the new binary tree and an option of the
 * removed element.  *)
let rec bst_remove_min (t : 'v binary_tree) : 'v binary_tree * 'v option =
  match t with
    Empty -> (Empty, None)
  | Node(Empty, d, r) -> (r, Some d)
  | Node(l, d, r) -> let (l', min) = bst_remove_min l
                     in (Node(l', d, r), min)


(* Remove the element x from binary search tree t
 *
 * Return a tuple containing the new binary tree and a bool that is
 * true if the element was found and false if the element was not
 * found *)
let rec bst_remove (cmp : 'v cmp_fun) (t : 'v binary_tree) (x : 'v)
        : 'v binary_tree * bool =
  match t with
    Empty -> (Empty, false)
  | Node(l,d,r) ->
     (match cmp x d with
      | Lesser -> let (l,f) = bst_remove cmp l x
                  in (Node(l, d, r), f)
      | Greater -> let (r,f) = bst_remove cmp r x
                   in (Node(l, d, r),f)
      | Equal ->
         ((match l with
             Empty -> r
           | Node(_,_,_) ->
              let (rp,oxp) = bst_remove_min r
              in match oxp with
                 | None -> l
                 | Some(xp) -> Node(l,xp,rp)),
          true))


(***********)
(** Tests **)
(***********)

(* See description in testing.ml *)

(* Convert a string tree to string *)
let rec str_int_binary_tree t =
  match t with
  | Empty -> "Empty"
  | Node(l,v,r) ->
     Printf.sprintf "Node(%s,%d,%s)"
       (str_int_binary_tree l)
       v
       (str_int_binary_tree r)

(* Convert tree to string *)
let rec str_str_binary_tree t =
  match t with
  | Empty -> "Empty"
  | Node(l,v,r) ->
     Printf.sprintf "Node(%s,\"%s\",%s)"
       (str_str_binary_tree l)
       v
       (str_str_binary_tree r)

(* Convert tree to string *)
let rec str_char_binary_tree (t: char binary_tree) =
  match t with
  | Empty -> "Empty"
  | Node(l,v,r) ->
     Printf.sprintf "Node(%s,\'%c\',%s)"
       (str_char_binary_tree l)
       v
       (str_char_binary_tree r)

let identity x = x

let map_printer =
  Some((fun (f,t) -> str_int_binary_tree t),
       str_int_list)

let map_printer_tree_int =
  Some((fun (f,t) -> str_int_binary_tree t),
        str_int_binary_tree)

let map_printer_tree_char =
  Some((fun (f,t) -> str_int_binary_tree t),
        str_char_binary_tree)

let tree_arg_printer_int =
  (fun (t,x) -> str_pair str_int_binary_tree string_of_int (t,x))

let tree_arg_printer_str =
  (fun (t,x) -> str_pair str_str_binary_tree identity (t,x))

let int_cmp : int->int->cmp_result =
  fun a b ->
  if a < b then Lesser
  else if a > b then Greater
  else Equal

let str_cmp : string->string->cmp_result =
  fun a b ->
  let c = compare a b in
  if c < 0 then Lesser
  else if c > 0 then Greater
  else Equal

let l0 = Node(Empty,0,Empty)
let l1 = Node(Empty,1,Empty)
let l2 = Node(Empty,2,Empty)
let l3 = Node(Empty,3,Empty)
let l4 = Node(Empty,4,Empty)
let l5 = Node(Empty,5,Empty)
let l6 = Node(Empty,6,Empty)
let l7 = Node(Empty,7,Empty)
let l8 = Node(Empty,8,Empty)

let la = Node(Empty,"a",Empty)
let lb = Node(Empty,"b",Empty)
let lc = Node(Empty,"c",Empty)
let ld = Node(Empty,"d",Empty)
let le = Node(Empty,"e",Empty)
let lf = Node(Empty,"f",Empty)

let lca = Node(Empty,'a',Empty)
let lcb = Node(Empty,'b',Empty)
let lcc = Node(Empty,'c',Empty)
let lcd = Node(Empty,'d',Empty)
let lce = Node(Empty,'e',Empty)
let lcf = Node(Empty,'f',Empty)

(* Custom test Functions *)

let add_one x = x + 1
let double x = x * 2

let map_inorder_tests =
  ("map_inorder",
   (fun (f,t) -> map_inorder f t),
   (=), (=),
   map_printer,
   [
     (Some("simple tree"),
      (identity,
       Node(l1, 2, l3)),
      Ok([1;2;3]));
     (None,
      (identity, Empty),
      Ok([]));

     (Some("Addition Function"),
     (add_one,
       Node(l1,2,Node(l3,4,l5))),
       Ok([2;3;4;5;6]));
       
     (Some("Double Function"),
     (double,
       Node(l1,2,Node(l3,4,l5))),
       Ok([2;4;6;8;10]));

     (Some("Complex Tree Identity"),
     (identity,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok([1;2;3;4;5;6;7;8;1]));

     (Some("Complex Tree Addition"),
     (add_one,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok([2;3;4;5;6;7;8;9;2]));

     (Some("Complex Tree Double"),
     (double,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok([2;4;6;8;10;12;14;16;2]));

     
  ])


let map_tree_tests =
  ("map_tree",
   (fun (f,t) -> map_tree f t),
   (=), (=),
   map_printer_tree_int,

   [
     (Some("simple tree"),
      (identity,
       Node(l1, 2, l3)),
      Ok(Node(l1, 2, l3))
     );

     (Some("Complex Tree"),
     (identity,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok(Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))));

     (Some("Complex Tree Addition"),
     (add_one,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok(Node(Node(l2,3,Node(l4,5,l6)), 7, Node(l8,9, l2))));

     (Some("Complex Tree Double"),
     (double,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok(Node(Node(l2,4,Node(l6,8,Node(Empty,10,Empty))),12,Node(Node(Empty,14,Empty),16, l2))));
  ])

let intToChar (n:int)= Char.chr (n+Char.code 'a')

let map_tree_char_tests =
  ("map_tree_char",
   (fun (f,t) -> map_tree f t),
   (=), (=),
   map_printer_tree_char,

   [
     (Some("int tree to char tree"),
      (intToChar,
       Node(l1, 2, l3)),
       Ok(Node(lcb, 'c', lcd))
     );

     (Some("Complex Tree"),
     (intToChar,
      Node(Node(l1,2,Node(l3,4,l5)), 6, Node(l7,8, l1))),
      Ok(Node(Node(lcb,'c',Node(lcd,'e',lcf)), 'g', Node(Node(Empty,'h',Empty),'i', lcb))));

     (Some("Left-skewed Tree"),
      (intToChar,
       Node(Node(l1, 3, l1), 5, Empty)),
       Ok(Node(Node(lcb, 'd', lcb), 'f', Empty)))

  ])




let is_bst_int = is_bst int_cmp
let is_bst_tests_int =
  ("is_bst_int",
   is_bst_int,
   (=), (=),
   Some(str_int_binary_tree,
        str_bool),
   [
     (Some("simple tree"),
      Node(Empty,
           1,
           Node(l2,
                3,
                Empty)),
      Ok(true));

     (Some("non-BST: left child violation"),
      Node(Node(Empty, 5, Empty), 3, Node(Empty, 7, Empty)),
      Ok(false));

     (Some("int BST full"),
      Node(
        Node( Node(Empty, 1, Empty),
              2,
              Node(Empty, 3, Empty)
            ),
        4,
        Node( Node(Empty, 5, Empty),
              6,
              Node(Empty, 7, Empty)
            )
      ),
      Ok(true));

     (Some("int non-BST"),
      Node(
        Node( Node(Empty, 7, Empty),
              2,
              Node(Empty, 3, Empty)
            ),
        4,
        Node(Empty, 6, Empty)
      ),
      Ok(false));


  ])

let is_bst_str = is_bst str_cmp
let is_bst_tests_str =
  ("is_bst_str",
   is_bst_str,
   (=), (=),
   Some(str_str_binary_tree,
        str_bool),
   [
     (Some("simple tree"),
      Node(Empty,
           "a",
           Node(lb,
                "c",
                Empty)),
      Ok(true));

     (Some("string BST"),
      Node(
        Node(Empty, "apple", Empty),
        "mango",
        Node(Empty, "zebra", Empty)
      ),
      Ok(true));

     (Some("string non-BST"),
      Node(
        Node(Empty, "zebra", Empty),
        "mango",
        Node(Empty, "peach", Empty)
      ),
      Ok(false));


  ])


let bst_max_tests =
  ("bst_max",
   bst_max,
   ((=) : 'v option -> 'v option -> bool),
   (=),
   Some(str_int_binary_tree,
        str_int_option),
   [
     (Some("simple tree"),
      Node(Empty,
           1,
           Node(l2, 3, Empty)),
      Ok(Some(3)));

     (Some "find max in single-node tree",
      (Node(Empty, 42, Empty)),
      Ok(Some 42));
     (Some "find max in right-skewed tree",
      (Node(Empty, 1, Node(Empty, 2, Node(Empty, 3, Node(Empty, 10, Empty))))),
      Ok(Some 10));
     (Some "find max in left-skewed tree",
      (Node(Node(Node(Empty, 5, Empty), 10, Empty), 15, Empty)),
      Ok(Some 15));
     (Some "find max in a balanced tree",
      (Node(Node(Empty, 5, Empty), 10, Node(Node(Empty, 12, Empty), 15, Node(Empty, 20, Empty)))),
      Ok(Some 20));
     (Some "find max in an empty tree",
      (Empty),
      Ok(None));
  ])


let bst_min_tests =
  ("bst_min",
   bst_min,
   ((=) : 'v option -> 'v option -> bool),
   (=),
   Some(str_int_binary_tree,
        str_int_option),
   [
     (Some("simple tree"),
      Node(Empty,
           1,
           Node(l2, 3, Empty)),
      Ok(Some(1)));

     (Some "empty tree",
      Empty,
      Ok None);
     (Some "balanced tree", 
      Node(
        Node(Node(Empty, 2, Empty), 4, Node(Empty, 6, Empty)), 
        10, 
        Node(Node(Empty, 12, Empty), 15, Node(Empty, 18, Empty))
      ), 
      Ok (Some 2));
     (Some "right-skewed tree", 
      Node(Empty, 3, Node(Empty, 6, Node(Empty, 8, Empty))), 
      Ok (Some 3));
     (Some "left-skewed tree", 
      Node(Node(Node(Empty, 2, Empty), 5, Empty), 10, Empty), 
      Ok (Some 2));
     (Some "unbalanced tree",
      Node(
        Node(Node(Node(Empty, 1, Empty), 3, Empty), 5, Node(Empty, 7, Empty)), 
        10, 
        Node(Node(Empty, 12, Empty), 15, Empty)
      ),
      Ok (Some 1));
  ])


let bst_insert_tests_int =
  ("bst_insert_int",
   (fun (t,x) ->  bst_insert int_cmp t x),
   (=), (=),
   Some(tree_arg_printer_int, str_int_binary_tree),
   [
     (Some("simple tree"),
      (Node(l1, 2, l3),
       0),
      Ok (Node(Node(l0,1,Empty),
               2,
               l3)));

     (Some "insert into empty tree",
      (Empty, 5),
       Ok (Node(Empty, 5, Empty)));
     (Some "insert new minimum",
      (Node(Node(l1, 3, l4), 5, l6), 0),
       Ok (Node(Node(Node(l0,1,Empty), 3, l4), 5, l6)));
     (Some "insert new maximum",
      (Node(l1, 2, Node(l3, 4, Empty)), 6),
       Ok (Node(l1, 2, Node(l3, 4, Node(Empty, 6, Empty)))));
     (Some "insert into balanced tree",
       (Node(Node(l1, 3, l4), 5, Node(l6, 7, l8)), 2),
       Ok (Node(Node(Node(Empty,1,Node(Empty,2,Empty)), 3, l4), 5, Node(l6, 7, l8))));
     (Some "insert duplicate value",
      (Node(Node(l1, 3, l4), 5, l6), 3),
        Ok (Node(Node(l1, 3, l4), 5, l6)));
  ])

let bst_insert_tests_str =
  ("bst_insert_str",
   (fun (t,x) ->  bst_insert str_cmp t x),
   (=), (=),
   Some(tree_arg_printer_str, str_str_binary_tree),
   [
     (Some("simple tree"),
      (Node(lb, "c", ld),
       "a"),
      Ok (Node(Node(la,"b",Empty),
               "c",
               ld)));

     (Some "insert at root",
      (Empty, "m"),
      Ok (Node(Empty, "m", Empty)));
     (Some "insert to the left",
      (Node(Empty, "m", Empty), "a"),
      Ok (Node(Node(Empty, "a", Empty), "m", Empty)));
     (Some "insert to the right",
      (Node(Empty, "m", Empty), "z"),
      Ok (Node(Empty, "m", Node(Empty, "z", Empty))));
     (Some "insert into deeper left subtree",
      (Node(Node(Empty, "m", Empty), "t", Empty), "b"),
      Ok (Node(Node(Node(Empty, "b", Empty), "m", Empty), "t", Empty)));
     (Some "insert into deeper right subtree",
      (Node(Empty, "m", Node(Empty, "z", Empty)), "x"),
      Ok (Node(Empty, "m", Node(Node(Empty, "x", Empty), "z", Empty))));
   ])


let bst_search_tests_int =
  ("bst_search_int",
   (fun (t,x) ->  bst_search int_cmp t x),
   (=), (=),
   Some(tree_arg_printer_int, str_bool),
   [
     (Some("simple tree"),
      (Node(l1, 2, l3),
       2),
      Ok true);

     (Some "search for root",
      (Node(Empty, 10, Empty), 10),
      Ok true);
     (Some "search for missing value",
      (Node(Node(Empty, 5, Empty), 10, Node(Empty, 15, Empty)), 20),
      Ok false);
     (Some "search in left subtree",
      (Node(Node(Empty, 5, Empty), 10, Empty), 5),
      Ok true);
     (Some "search in right subtree",
      (Node(Empty, 10, Node(Empty, 15, Empty)), 15),
      Ok true);
     (Some "search deeper in tree",
      (Node(Node(Empty, 5, Node(Empty, 7, Empty)), 10, Node(Empty, 15, Empty)), 7),
      Ok true);
   ]
  )


let bst_search_tests_str =
  ("bst_search_str",
   (fun (t,x) ->  bst_search str_cmp t x),
   (=), (=),
   Some(tree_arg_printer_str, str_bool),
   [
     (Some("simple tree"),
      (Node(la, "b", lc),
       "a"),
      Ok true);

     (Some "search for root",
      (Node(Empty, "m", Empty), "m"),
      Ok true);
     (Some "search for missing string",
      (Node(Node(Empty, "a", Empty), "m", Node(Empty, "z", Empty)), "x"),
      Ok false);
     (Some "search in left subtree",
      (Node(Node(Empty, "a", Empty), "m", Empty), "a"),
      Ok true);
     (Some "search in right subtree",
      (Node(Empty, "m", Node(Empty, "z", Empty)), "z"),
      Ok true);
     (Some "search deeper in tree",
      (Node(Node(Empty, "b", Node(Empty, "d", Empty)), "m", Node(Empty, "z", Empty)), "d"),
      Ok true);
   ])

let bst_remove_min_tests =
  ("bst_remove_min",
   bst_remove_min,
   (=), (=),
   Some(str_int_binary_tree,
        (fun (t,x) -> str_pair str_int_binary_tree str_int_option (t,x))),
   [
     (Some("simple tree"),
      Node(l1, 2, l3),
      Ok (Node(Empty,2,l3),
          Some 1));
     (Some("empty tree"),
      Empty,
      Ok (Empty, None));

     (Some "remove minimum from left-skewed tree",
      (Node(Node(Empty, 1, Empty), 2, Empty)),
      Ok (Node(Empty, 2, Empty), Some 1));
     (Some "remove minimum from root",
      (Node(Empty, 1, Empty)),
      Ok (Empty, Some 1));
     (Some "remove minimum from tree with right children",
      (Node(Node(Empty, 1, Empty), 3, Node(Empty, 5, Empty))),
      Ok (Node(Empty, 3, Node(Empty, 5, Empty)), Some 1));
     (Some "remove minimum from deeper left subtree",
      (Node(Node(Node(Empty, 0, Empty), 2, Empty), 5, Node(Empty, 10, Empty))),
      Ok (Node(Node(Empty, 2, Empty), 5, Node(Empty, 10, Empty)), Some 0));
     (Some "remove minimum from larger tree",
      (Node(Node(Node(Empty, 2, Empty), 5, Node(Empty, 6, Empty)), 10, Node(Empty, 15, Empty))),
      Ok (Node(Node(Empty, 5, Node(Empty, 6, Empty)), 10, Node(Empty, 15, Empty)), Some 2));
   ])


let bst_remove_tests_int =
  ("bst_remove_int",
   (fun(t,x) -> bst_remove int_cmp t x),
   (=), (=),
   Some(tree_arg_printer_int,
        (fun (t,f) -> str_pair str_int_binary_tree str_bool (t,f))),
   (* None, *)
   [
     (Some("simple tree"),
      (Node(l1, 2, l3), 1),
      Ok ((Node (Empty,2,l3)), true));

     (Some "remove root with no children",
      (Node(Empty, 5, Empty), 5),
      Ok (Empty, true));
     (Some "remove leaf node",
      (Node(Node(Empty, 3, Empty), 5, Empty), 3),
      Ok (Node(Empty, 5, Empty), true));
     (Some "remove node with one child",
      (Node(Node(Empty, 3, Empty), 5, Node(Empty, 8, Empty)), 5),
      Ok (Node(Node(Empty, 3, Empty), 8, Empty), true));
     (Some "remove node with two children",
      (Node(Node(Empty, 3, Empty), 5, Node(Empty, 8, Node(Empty, 10, Empty))), 5),
      Ok (Node(Node(Empty, 3, Empty), 8, Node(Empty, 10, Empty)), true));
     (Some "remove deeper node with two children",
      (Node(Node(Empty, 2, Node(Empty, 3, Empty)), 5, Node(Empty, 8, Empty)), 3),
      Ok (Node(Node(Empty, 2, Empty), 5, Node(Empty, 8, Empty)), true));
   ])


let bst_remove_tests_str =
  ("bst_remove_str",
   (fun(t,x) -> bst_remove str_cmp t x),
   (=), (=),
   Some(tree_arg_printer_str,
        (fun (t,f) -> str_pair str_str_binary_tree str_bool (t,f))),
   [
     (Some("simple tree"),
      (Node(la, "b", lc), "a"),
      Ok ((Node (Empty,"b",lc)), true));

     (Some "remove root with no children",
      (Node(Empty, "m", Empty), "m"),
      Ok (Empty, true));
     (Some "remove leaf node",
      (Node(Node(Empty, "a", Empty), "m", Empty), "a"),
      Ok (Node(Empty, "m", Empty), true));
     (Some "remove node with one child",
      (Node(Node(Empty, "a", Empty), "m", Node(Empty, "z", Empty)), "m"),
      Ok (Node(Node(Empty, "a", Empty), "z", Empty), true));
     (Some "remove node with two children",
      (Node(Node(Empty, "a", Empty), "m", Node(Empty, "z", Node(Empty, "zz", Empty))), "m"),
      Ok (Node(Node(Empty, "a", Empty), "z", Node(Empty, "zz", Empty)), true));
     (Some "remove deeper node with two children",
      (Node(Node(Empty, "b", Node(Empty, "c", Empty)), "m", Node(Empty, "z", Empty)), "c"),
      Ok (Node(Node(Empty, "b", Empty), "m", Node(Empty, "z", Empty)), true));
    ])



let zero= Num (Some 0)
let one= Num (Some 1)
let two= Num (Some 2)
let three= Num (Some 3)
let four= Num (Some 4)
let five= Num (Some 5)
let six= Num (Some 6)
                                
let exp_string_tests =
  ("exp_string",
   exp_string, (=), (=),
   Some (exp_string, fun x->x),
   [
     (Some "Simple Num", one , Ok "1");
     (Some "Simple Exp", BinExp(Add, one , two ), Ok "(1+2)");
     (Some "Exp Commutativity",  BinExp(Sub, one , two ), Ok "(1-2)");
     (* TODO *)
     (Some "Simple Multiplication", BinExp(Mul, six , six ), Ok "(6*6)");
     (Some "Simple Division", BinExp(Div, six , two ), Ok "(6/2)");
     (Some "Division by Zero", BinExp(Div, six , zero ), Ok "(6/0)" );
     (Some "Indirect division by zero", BinExp(Div, six, BinExp(Sub, two, two)), Ok "(6/(2-2))" );
     (Some "Complex Expression", BinExp( Mul, BinExp(Sub, six, two), BinExp(Add, one , two )), Ok "((6-2)*(1+2))");
   ])


let exp_eval_tests =
  ("exp_eval",
   exp_eval, (=), (=),
   Some (exp_string, str_int_option),
   [
     (Some "Simple Num", one , Ok (Some 1));
     (Some "Simple Exp", BinExp(Add, one , two ), Ok (Some 3));
     (None,  BinExp(Sub, one , two ), Ok (Some (-1)));

     (Some "Simple Multiplication", BinExp(Mul, six , six ), Ok (Some 36));
     (Some "Simple Division", BinExp(Div, six , two ), Ok (Some 3) );
     (Some "Division by Zero", BinExp(Div, six , zero ), Ok (None) );
     (Some "Indirect division by zero", BinExp(Div, six, BinExp(Sub, two, two)), Ok (None) );
     (Some "Complex Expression", BinExp( Mul, BinExp(Sub, six, two), BinExp(Add, one , two )), Ok (Some 12));

   ])
