open Util (* see util.ml *)

(*****************************)
(* PART II: Red-Black Trees *)
(*****************************)

(*****************************
   This 7 minute video explains Red-Black Trees
https://youtu.be/igUOhpGICgA?si=PxZM6Rz7rcLfmCPzhttps://youtu.be/JzhG0jDxGqg?si=I4VNzcYA5ihVQB_O

   Osaki;s Algorithm --see References in the home page, there is a link to the book.--
   https://youtu.be/dCBAhbIEoYM?si=GUdCcVPoaGfy3M0Z

   useful videos:
   https://youtu.be/igUOhpGICgA?si=PxZM6Rz7rcLfmCPz
   https://www.youtube.com/watch?v=D4FJMJUIMSw
*****************************)


module RBTree = struct
  type color = Red | Black
  type 'v tree =
    | Empty
    | Rnode of 'v tree * 'v * 'v tree (* Red node *)
    | Bnode of 'v tree * 'v * 'v tree (* Black node *)

  (* Return the color of an rbtree node *)
  let color t =
    match t with
      Rnode(_,_,_) -> Red
    | Bnode(_,_,_) | Empty -> Black

  (* Result of comparing two values *)
  (* if a < b, then cmp a b -> Lesser *)
  (* if a = b, then cmp a b -> Equal *)
  (* if a > b, then cmp a b -> Greater *)
  type cmp_result =
    | Lesser | Equal | Greater

  (* compare two data elements of type 'v *)
  type 'v cmp_fun = 'v -> 'v -> cmp_result

  (* Test if t satisfies the red-black tree invariants.
   *
   *  1. Does every red node have only black children?
   *  2. Does every path from root to leaf have the same number of black nodes?
   *)
  let is_invariant (t : 'v tree) : bool =
    let rec h t : bool * int =
      match t with
      | Empty -> (true, 0)
      | Rnode(l,_,r) -> 
        let (valid_l, black_l) = h l in
        let (valid_r, black_r) = h r in
        let no_red_violation =
          match l, r with
          | (Rnode _, _) | (_, Rnode _) -> false
          | _ -> true
        in
        valid_l && valid_r && no_red_violation && black_l = black_r, black_l
      | Bnode(l,_,r) ->
        let (valid_l, black_l) = h l in
        let (valid_r, black_r) = h r in
        (valid_l && valid_r && black_l = black_r, black_l + 1)
    in (let (v,_) = h t in v)  (*do NOT replace this code*)

  (* Test if red-black tree t is sorted. *)
  let rec is_sorted (cmp : 'v cmp_fun) (t : 'v tree) : bool =
    let rec h t x0 =
      match t with
        Empty -> (x0,true)
      | Rnode(l,x,r) | Bnode(l,x,r)
        -> let (x0,is) = h l x0
           in if is && (match x0 with
                          None -> true
                        | Some y -> (match cmp x y with
                                     | Lesser -> false
                                     | Equal  | Greater -> true))
              then (h r (Some x))
              else (Some x, false)
    in let (_,is) = h t None
       in is

  (* Search for element x in red-black tree t.
   *
   * Return true if the tree contains x and false if it does not. *)
  let rec search (cmp : 'v cmp_fun) (t : 'v tree) (x:'v) : bool =
    match t with
    Empty -> false
    | Rnode(l,v,r) | Bnode(l,v,r) -> begin match cmp x v with
        Lesser -> search cmp l x
        | Greater -> search cmp r x
        | Equal -> true end

  (* Balance constructor for a red-black tree *)
  let balance (c:color) (l : 'v tree) (v : 'v ) (r : 'v tree) : 'v tree =
        match c, v, l, r with
        Black, z, Rnode(Rnode(a, x, b), y, c), d
        | Black, z, Rnode(a, x, Rnode(b, y, c)), d
        | Black, x, a, Rnode(Rnode(b, y, c), z, d)
        | Black, x, a, Rnode(b, y, Rnode(c, z, d)) -> Rnode(Bnode(a, x, b), y, Bnode(c, z, d))
        | Black, a, b, c -> Bnode(b, a, c)
        | Red, a, b, c -> Rnode(b, a, c)

  (*helper function insert into rbtree*)
  let rec insert_helper (cmp : 'v cmp_fun) (x:'v) (t : 'v tree) : 'v tree =
        match t with
        | Empty -> Rnode(Empty, x, Empty)
        | Rnode(l, a, r) as n ->
          if x < a then balance Red (insert_helper cmp x l) a r
          else if x > a then balance Red l a (insert_helper cmp x r)
          else n
        | Bnode(l, a, r) as n ->
          if x < a then balance Black (insert_helper cmp x l) a r
          else if x > a then balance Black l a (insert_helper cmp x r)
          else n

  (*insert into rbtree*)
  let insert (cmp : 'v cmp_fun) (t : 'v tree) (x:'v) : 'v tree =
    let result=insert_helper cmp x t in
    match result with
    |Rnode (l,v,r) -> Bnode(l,v,r) (*color root to Black*)
    |Bnode (_,_,_) -> result (*root is already black*)
    |Empty -> (*impossible*)
       failwith "RBT insert failed with insert_helper returning leaf"

  (* Apply function f to every data element of tree t and collect the
   results in a list following an inorder traversal of the tree *)
  let rec map_rbtree (f : 'v -> 'a) (t: 'v tree) (a: 'a list) : 'a list = 
      match t with
      | Empty -> a
      | Rnode(l, v, r) | Bnode(l, v, r) ->
              let x = (map_rbtree f r a) in
              let y = f v in
              let z = y::x in
              map_rbtree f l z

  let map_inorder (f : 'v -> 'a) (t : 'v tree) : 'a list =
          map_rbtree f t []

  (* Apply function f to every data element of tree t and collect the
   results in a list following the reverse of an inorder traversal of the tree *)
 let rec reverse (l : 'a list) (r : 'a list) : 'a list =
        match l with
        |  [] -> r
        |  h::t -> reverse t (h :: r)
 
  let map_revorder (f : 'v -> 'a) (t : 'v tree) : 'a list =
          reverse (map_rbtree f t []) []
end


(*********)
(* Tests *)
(*********)

(* See description in testing.ml *)

let int_cmp : int->int->RBTree.cmp_result =
  fun a b ->
  if a < b then RBTree.Lesser
  else if a > b then RBTree.Greater
  else RBTree.Equal

let str_cmp : string->string->RBTree.cmp_result =
  fun a b ->
  let c = compare a b in
  if c < 0 then RBTree.Lesser
  else if c > 0 then RBTree.Greater
  else RBTree.Equal

(* Convert tree to string *)
let rec str_x_rbtree f t =
  let h s l d r =
    Printf.sprintf "%s(%s,%s,%s)"
      s (str_x_rbtree f l) (f d) (str_x_rbtree f r)
  in
  match t with
  | RBTree.Empty -> "Empty"
  | RBTree.Rnode(l,d,r) ->
     h "Rnode" l d r
  | RBTree.Bnode(l,d,r) ->
     h "Bnode" l d r

let str_int_rbtree t = str_x_rbtree string_of_int t
let str_str_rbtree t = str_x_rbtree str_str t

let tree_arg_printer f =
  (fun (t,x) -> str_pair (str_x_rbtree f) f (t,x))

let int_tree_arg_printer = tree_arg_printer string_of_int
let str_tree_arg_printer = tree_arg_printer str_str

exception InvalidRBTreeError

(* To check that test case inputs are valid, sorted red-black trees *)
let check_rbtree (cmp : 'v RBTree.cmp_fun) (t: 'v RBTree.tree) =
  if (RBTree.is_invariant t) && (RBTree.is_sorted cmp t) then t
  else raise InvalidRBTreeError

(* To check that test case expected outputs are valid, sorted red-black trees *)
let eq_rbtree cmp_fun t1 t_expected =
  t1 = (check_rbtree cmp_fun t_expected)

(* Leaf node constants to make test inputs more readable *)
let e = RBTree.Empty
let rnode ((l,v,r) : ('v RBTree.tree * 'v * 'v RBTree.tree)) : 'v RBTree.tree =
  RBTree.Rnode(l,v,r)
let bnode ((l,v,r) : ('v RBTree.tree * 'v * 'v RBTree.tree)) : 'v RBTree.tree =
  RBTree.Bnode(l,v,r)
let r0 =  rnode(e, 0,e)
let r1 =  rnode(e, 1,e)
let r2 =  rnode(e, 2,e)
let r3 =  rnode(e, 3,e)
let r4 =  rnode(e, 4,e)
let r5 =  rnode(e, 5,e)
let r6 =  rnode(e, 6,e)
let r7 =  rnode(e, 7,e)
let r8 =  rnode(e, 8,e)
let r9 =  rnode(e, 9,e)
let r10 = rnode(e,10,e)
let r11 = rnode(e,11,e)
let r12 = rnode(e,12,e)
let r13 = rnode(e,13,e)
let r14 = rnode(e,14,e)
let r15 = rnode(e,15,e)
let r16 = rnode(e,16,e)
let r17 = rnode(e,17,e)
let r18 = rnode(e,18,e)
let r19 = rnode(e,19,e)
let r20 = rnode(e,20,e)
let r21 = rnode(e,21,e)
let r22 = rnode(e,22,e)
let r23 = rnode(e,23,e)
let r24 = rnode(e,24,e)
let r25 = rnode(e,25,e)
let r26 = rnode(e,26,e)
let r27 = rnode(e,27,e)
let r28 = rnode(e,28,e)
let r29 = rnode(e,29,e)
let r30 = rnode(e,30,e)
let r31 = rnode(e,31,e)
let r32 = rnode(e,32,e)
let r33 = rnode(e,33,e)
let r34 = rnode(e,34,e)

let b0 =  bnode(e, 0,e)
let b1 =  bnode(e, 1,e)
let b2 =  bnode(e, 2,e)
let b3 =  bnode(e, 3,e)
let b4 =  bnode(e, 4,e)
let b5 =  bnode(e, 5,e)
let b6 =  bnode(e, 6,e)
let b7 =  bnode(e, 7,e)
let b8 =  bnode(e, 8,e)
let b9 =  bnode(e, 9,e)
let b10 = bnode(e,10,e)
let b11 = bnode(e,11,e)
let b12 = bnode(e,12,e)
let b13 = bnode(e,13,e)
let b14 = bnode(e,14,e)
let b15 = bnode(e,15,e)
let b16 = bnode(e,16,e)
let b17 = bnode(e,17,e)
let b18 = bnode(e,18,e)
let b19 = bnode(e,19,e)
let b20 = bnode(e,20,e)

let ra =  rnode(e,"a",e)
let rb =  rnode(e,"b",e)
let rc =  rnode(e,"c",e)
let rd =  rnode(e,"d",e)

let ba =  bnode(e,"a",e)
let bb =  bnode(e,"b",e)
let bc =  bnode(e,"c",e)
let bd =  bnode(e,"d",e)


let rbt_is_invariant_int_list =
  [
    (Some("simple tree"),
     bnode(r1, 2, r3),
     Ok(true));
    (None,
     bnode(b1, 2, rnode(r3,4,b5)),
     Ok(false));

    (Some("balanced black-height"),
     bnode(bnode(r1,1,r2), 3, bnode(r3,4,r4)),
     Ok(true));
     (Some("deep valid tree"),
     bnode(bnode(rnode(bnode(b1, 1, b2), 2, bnode(b3, 3, b4)),4,bnode(b5, 5, b6)),6,bnode(rnode(bnode(b7, 7, b8), 8, bnode(b9, 9, b10)),10,bnode(b11, 11, b12))),
     Ok(true));
    (Some("valid alternating red-black"),
     bnode(rnode(bnode(r1,1,r2), 2, bnode(r3,3,r4)), 4, bnode(r5,5,r6)),
     Ok(true));
    (Some("invalid Red node having two red children"),
     rnode(rnode(b1,1,b2), 2, rnode(b3,3,b4)),
     Ok(false));
    (Some("invalid: Different black heights on left and right"),
     bnode(bnode(bnode(r1,1,r2), 2, r3), 4, bnode(r4,5,b5)),
     Ok(false));
  ]

let rbt_is_invariant_int_tests =
  ("rbt_is_invariant_int",
   RBTree.is_invariant,
   (=), (=),
   Some(str_int_rbtree,
        str_bool),
   rbt_is_invariant_int_list
   )

let rbt_is_invariant_str_tests =
  ("rbt_is_invariant_str",
   RBTree.is_invariant,
   (=), (=),
   Some(str_str_rbtree,
        str_bool),
   [
     (Some("simple tree"),
      RBTree.Bnode(ra, "b", rc),
      Ok(true));

     (Some("valid balanced tree"),
      RBTree.Bnode(
        RBTree.Bnode(RBTree.Rnode(RBTree.Empty, "a", RBTree.Empty), "b", RBTree.Rnode(RBTree.Empty, "c", RBTree.Empty)),
        "d",
        RBTree.Bnode(RBTree.Rnode(RBTree.Empty, "e", RBTree.Empty), "f", RBTree.Rnode(RBTree.Empty, "g", RBTree.Empty))
      ),
      Ok(true));
     (Some("red violation"),
      RBTree.Bnode(
        RBTree.Bnode(RBTree.Rnode(RBTree.Empty, "a", RBTree.Empty), "b", RBTree.Rnode(RBTree.Rnode(RBTree.Empty, "c", RBTree.Empty), "d", RBTree.Empty)),
        "e",
        RBTree.Bnode(RBTree.Empty, "f", RBTree.Empty)
      ),
      Ok(false));
     (Some("black height imbalance"),
      RBTree.Bnode(
        RBTree.Bnode(RBTree.Rnode(RBTree.Empty, "a", RBTree.Empty), "b", RBTree.Rnode(RBTree.Empty, "c", RBTree.Empty)),
        "d",
        RBTree.Bnode(RBTree.Empty, "e", RBTree.Bnode(RBTree.Empty, "f", RBTree.Empty))
      ),
      Ok(false));
     (Some("deep valid tree"), 
      RBTree.Bnode(
        RBTree.Bnode(
          RBTree.Bnode(
            RBTree.Rnode(RBTree.Empty, "a", RBTree.Empty), "b", 
            RBTree.Rnode(RBTree.Empty, "c", RBTree.Empty)
          ), "d",
          RBTree.Bnode(
            RBTree.Rnode(RBTree.Empty, "e", RBTree.Empty), "f",
            RBTree.Rnode(RBTree.Empty, "g", RBTree.Empty)
          )
        ), "h",
        RBTree.Bnode(
          RBTree.Bnode(
            RBTree.Rnode(RBTree.Empty, "i", RBTree.Empty), "j",
            RBTree.Rnode(RBTree.Empty, "k", RBTree.Empty)
          ), "l",
          RBTree.Bnode(
            RBTree.Rnode(RBTree.Empty, "m", RBTree.Empty), "n",
            RBTree.Rnode(RBTree.Empty, "o", RBTree.Empty)
          )
        )
      ),
      Ok(true));
     (Some("single node"),
      RBTree.Bnode(RBTree.Empty, "z", RBTree.Empty),
      Ok(true));
   ])

let rbt_search_int_tests =
  ("rbt_search_int",
   (fun (t,x) -> RBTree.search int_cmp (check_rbtree int_cmp t) x),
   (=), (=),
   Some(int_tree_arg_printer, str_bool),
   [
     (Some("simple tree"),
      (RBTree.Bnode(r1, 2, r3), 2),
      Ok(true));
     (None,
      (r1, 1),
      Ok(true));
     (None,
      (r1, 2),
      Ok(false));
     (* TODO *)

     (Some("int not found"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, 1, Empty), 2, RBTree.Rnode(Empty, 3, Empty)), 4,
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,5,Empty), 6, Empty), 7,
        RBTree.Bnode(RBTree.Rnode(Empty, 8, Empty), 9, RBTree.Rnode(Empty, 10, Empty)))), 11),
     Ok(false));

     (Some("Find on left"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, 1, Empty), 2, RBTree.Rnode(Empty, 3, Empty)), 4,
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,5,Empty), 6, Empty), 7,
        RBTree.Bnode(RBTree.Rnode(Empty, 8, Empty), 9, RBTree.Rnode(Empty, 10, Empty)))), 3),
     Ok(true));

     (Some("Find on right"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, 1, Empty), 2, RBTree.Rnode(Empty, 3, Empty)), 4,
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,5,Empty), 6, Empty), 7,
        RBTree.Bnode(RBTree.Rnode(Empty, 8, Empty), 9, RBTree.Rnode(Empty, 10, Empty)))), 8),
     Ok(true));

     (Some("Empty tree"),
      (Empty, 2), Ok(false));
   ])

let rbt_search_str_tests =
  ("rbt_search_str",
   (fun (t,x) -> RBTree.search str_cmp (check_rbtree str_cmp t) x),
   (=), (=),
   Some(str_tree_arg_printer, str_bool),
   [
     (Some("simple tree"),
      (RBTree.Bnode(ra, "b", rc), "b"),
      Ok(true));
     (* TODO *)
     (Some("str not found"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, "a", Empty), "b", RBTree.Rnode(Empty, "c", Empty)), "d",
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,"e",Empty), "f", Empty), "g",
        RBTree.Bnode(RBTree.Rnode(Empty, "h", Empty), "i", RBTree.Rnode(Empty, "j", Empty)))), "k"),
     Ok(false));

     (Some("Found on left"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, "a", Empty), "b", RBTree.Rnode(Empty, "c", Empty)), "d",
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,"e",Empty), "f", Empty), "g",
        RBTree.Bnode(RBTree.Rnode(Empty, "h", Empty), "i", RBTree.Rnode(Empty, "j", Empty)))), "a"),
     Ok(true));
     
     (Some("Found on right"),
      (RBTree.Bnode(RBTree.Bnode(RBTree.Rnode(Empty, "a", Empty), "b", RBTree.Rnode(Empty, "c", Empty)), "d",
        RBTree.Rnode(RBTree.Bnode(RBTree.Rnode(Empty,"e",Empty), "f", Empty), "g",
        RBTree.Bnode(RBTree.Rnode(Empty, "h", Empty), "i", RBTree.Rnode(Empty, "j", Empty)))), "h"),
     Ok(true));

     (Some("Empty tree"),
      (Empty, "z"), Ok(false));
   ])
 

let rbt_balance_tester t =
  (* Note: RBTree.balance does not always return a balanced tree!  We
     only enforce balance when we reach the black grandparent in a
     red-red invariant violation. *)
  match t with
    RBTree.Empty -> raise InvalidRBTreeError (* we don't ever balance empty trees *)
  | RBTree.Rnode(l,v,r) | RBTree.Bnode(l,v,r)
    -> RBTree.balance (RBTree.color t) l v r

let rbt_balance_int_tests =
  ("rbt_balance_int",
   rbt_balance_tester,
   (=), (=),
   Some(str_int_rbtree, str_int_rbtree),
   [
     (Some("Case A"),
      RBTree.Bnode(RBTree.Rnode(r1,2,RBTree.Empty),
            3,
            RBTree.Empty),
      Ok(RBTree.Rnode(b1,2,b3)));
     (Some("Except"),
      RBTree.Empty,
      Error InvalidRBTreeError);
     (* TODO *)

      (Some ("Case B"),
       RBTree.Bnode(RBTree.Rnode(Empty, 2, RBTree.Rnode(Empty, 3, Empty)), 4, Empty),
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, 2, Empty), 3, RBTree.Bnode(Empty, 4, Empty))));

      (Some ("Case C"),
       RBTree.Bnode(Empty, 2, RBTree.Rnode(Empty, 3, RBTree.Rnode(Empty, 4, Empty))),
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, 2, Empty), 3, RBTree.Bnode(Empty, 4, Empty))));

      (Some ("Case D"),
       RBTree.Bnode(Empty, 2, RBTree.Rnode(RBTree.Rnode(Empty, 3, Empty), 4, Empty)), 
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, 2, Empty), 3, RBTree.Bnode(Empty, 4, Empty))));
   ])

let rbt_balance_str_tests =
  ("rbt_balance_str",
   rbt_balance_tester,
   (=), (=),
   Some(str_str_rbtree, str_str_rbtree),
   [
     (Some("Case A"),
      RBTree.Bnode(RBTree.Rnode(ra,"b",RBTree.Empty),
            "c",
            RBTree.Empty),
      Ok(RBTree.Rnode(ba,"b",bc)));
       (* TODO *)
      (Some ("Case B"),
       RBTree.Bnode(RBTree.Rnode(Empty, "x", RBTree.Rnode(Empty, "y", Empty)), "z", Empty),
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, "x", Empty), "y", RBTree.Bnode(Empty, "z", Empty))));

      (Some ("Case C"),
       RBTree.Bnode(Empty, "x", RBTree.Rnode(Empty, "y", RBTree.Rnode(Empty, "z", Empty))),
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, "x", Empty), "y", RBTree.Bnode(Empty, "z", Empty))));

      (Some ("Case D"),
       RBTree.Bnode(Empty, "x", RBTree.Rnode(RBTree.Rnode(Empty, "y", Empty), "z", Empty)), 
       Ok(RBTree.Rnode(RBTree.Bnode(Empty, "x", Empty), "y", RBTree.Bnode(Empty, "z", Empty))));

   ])


let rbt_insert_tester f =
  fun (t,x) ->
  check_rbtree f
    (RBTree.insert f
       (check_rbtree f t)
       x)
let int_rbt_insert_tester = rbt_insert_tester int_cmp
let int_rbt_insert_tests_eq = eq_rbtree int_cmp
let int_rbt_insert_list =
  [
    (Some("simple tree"),
     (bnode(r1, 2, e), 3),
     Ok(bnode(r1, 2, r3)));
    (* TODO *)
    (Some("insert left"),
    (bnode(bnode(e,2,rnode(e,3,e)), 4, rnode(bnode(e, 5, rnode(e, 6, e)), 7,bnode(rnode(e,8,e), 9, rnode(e,10,e)))), 1),
    Ok(bnode(bnode(rnode(e,1,e),2,rnode(e,3,e)),4,rnode(bnode(e,5,rnode(e,6,e)), 7, bnode(rnode(e,8,e),9,rnode(e,10,e))))));

    (Some("insert right"),
    (bnode(bnode(rnode(e,1,e),2,rnode(e,3,e)), 4, rnode(bnode(e, 5, rnode(e, 6, e)), 7,bnode(e, 9, rnode(e,10,e)))), 8),
    Ok(bnode(bnode(rnode(e,1,e),2,rnode(e,3,e)),4,rnode(bnode(e,5,rnode(e,6,e)),7,bnode(rnode(e,8,e),9,rnode(e,10,e))))));

    (Some("Empty list"), (Empty, 129), Ok(bnode(e,129,e)));
  ]

let rbt_insert_int_tests =
  ("rbt_insert_int",
   int_rbt_insert_tester,
   int_rbt_insert_tests_eq,  (=),
   Some(int_tree_arg_printer,
        str_int_rbtree),
   int_rbt_insert_list )

let str_rbt_insert_tester = rbt_insert_tester str_cmp
let str_rbt_insert_tests_eq = eq_rbtree str_cmp
let rbt_insert_str_tests =
  ("rbt_insert_str",
   str_rbt_insert_tester,
   str_rbt_insert_tests_eq,  (=),
   Some(str_tree_arg_printer,
        str_str_rbtree),
   [
     (Some("simple tree"),
      (RBTree.Bnode(ra, "b", RBTree.Empty), "c"),
      Ok(RBTree.Bnode(ra, "b", rc)));
     (* TODO *)
    (Some("insert left"),
    (Bnode(Bnode(Empty,"b",Rnode(Empty,"c",Empty)),
    "d",
    Rnode(Bnode(Empty, "e", Rnode(Empty, "f", Empty)),
        "g",
        Bnode(Rnode(Empty,"h",Empty), "i", Rnode(Empty,"j",Empty)))), "a"),
    Ok(Bnode(Bnode(Rnode(Empty,"a",Empty),"b",Rnode(Empty,"c",Empty)),
    "d",
    Rnode(Bnode(Empty,"e",Rnode(Empty,"f",Empty)),
        "g",
        Bnode(Rnode(Empty,"h",Empty),"i",Rnode(Empty,"j",Empty))))));

    (Some("insert right"),
    (Bnode(Bnode(Rnode(Empty,"a",Empty),"b",Rnode(Empty,"c",Empty)),
    "d",
    Rnode(Bnode(Empty, "e", Rnode(Empty, "f",Empty)),
        "g",
        Bnode(Empty, "i", Rnode(Empty,"j",Empty)))), "h"),
    Ok(Bnode(Bnode(Rnode(Empty,"a",Empty),"b",Rnode(Empty,"c",Empty)),
    "d",
    Rnode(Bnode(Empty,"e",Rnode(Empty,"f",Empty)),
        "g",
        Bnode(Rnode(Empty,"h",Empty),"i",Rnode(Empty,"j",Empty))))));

    (Some("Empty list"), (Empty, "wawa zooie"), Ok(Bnode(Empty,"wawa zooie",Empty)));

   ])


let map_printer =
  Some((fun (f,t) -> str_int_rbtree t),
       str_int_list)


let identity x = x

let rbt_map_inorder_tests =
  ("rbt_map_inorder",
   (fun (f,t) -> RBTree.map_inorder f t),
   (=), (=),
   map_printer,
   [
     (Some("simple tree"),
      (identity,RBTree.Bnode(r1, 2, r3)),
      Ok([1; 2; 3]));
     (None,
      (identity,RBTree.Empty),
      Ok([]));
     (* TODO *)
     (Some("Doubled tree"),
     ((fun x -> x * 2), Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty))),
     Ok([2;4;6]));
     (Some("Big Tree"), (identity, 
     Bnode(Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty)),
     4,
     Rnode(Bnode(Empty,5,Rnode(Empty,6,Empty)),7,Bnode(Rnode(Empty,8,Empty),9,Rnode(Empty,10,Empty))))
     ), Ok([1;2;3;4;5;6;7;8;9;10]));

     (Some ("Big doubled"), ((fun x -> x * 2),
     Bnode(Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty)),
     4,
     Rnode(Bnode(Empty,5,Rnode(Empty,6,Empty)),7,Bnode(Rnode(Empty,8,Empty),9,Rnode(Empty,10,Empty))))),
     Ok([2;4;6;8;10;12;14;16;18;20]));

     (Some ("Left side empty"), (identity,
     Bnode(Empty, 1, Rnode(Empty, 2, Bnode(Empty, 3, Rnode(Empty, 4, Bnode(Empty, 5, Rnode(Empty,6,Empty))))))),
     Ok ([1;2;3;4;5;6]));

     (Some ("Right side empty"), (identity,
     Bnode(Rnode(Bnode(Rnode(Bnode(Rnode(Empty,1,Empty),2,Empty),3,Empty),4,Empty),5,Empty),6,Empty)),
     Ok ([1;2;3;4;5;6]));

     (Some ("Empty tree"), (identity, Empty), Ok([]));
  ])

let rbt_map_revorder_tests =
  ("rbt_map_revorder",
   (fun (f,t) -> RBTree.map_revorder f t),
   (=), (=),
   map_printer,
   [
     (Some("simple tree"),
      (identity,RBTree.Bnode(r1, 2, r3)),
      Ok([3; 2; 1]));
     (None,
      (identity,RBTree.Empty),
      Ok([]));
     (* TODO *)
     (Some("Doubled tree"),
     ((fun x -> x * 2), Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty))),
     Ok([6;4;2]));
     (Some("Big Tree"), (identity, 
     Bnode(Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty)),
     4,
     Rnode(Bnode(Empty,5,Rnode(Empty,6,Empty)),7,Bnode(Rnode(Empty,8,Empty),9,Rnode(Empty,10,Empty))))
     ), Ok([10;9;8;7;6;5;4;3;2;1]));

     (Some ("Big doubled"), ((fun x -> x * 2),
     Bnode(Bnode(Rnode(Empty,1,Empty),2,Rnode(Empty,3,Empty)),
     4,
     Rnode(Bnode(Empty,5,Rnode(Empty,6,Empty)),7,Bnode(Rnode(Empty,8,Empty),9,Rnode(Empty,10,Empty))))),
     Ok([20;18;16;14;12;10;8;6;4;2]));

     (Some ("Left side empty"), (identity,
     Bnode(Empty, 1, Rnode(Empty, 2, Bnode(Empty, 3, Rnode(Empty, 4, Bnode(Empty, 5, Rnode(Empty,6,Empty))))))),
     Ok ([6;5;4;3;2;1]));

     (Some ("Right side empty"), (identity,
     Bnode(Rnode(Bnode(Rnode(Bnode(Rnode(Empty,1,Empty),2,Empty),3,Empty),4,Empty),5,Empty),6,Empty)),
     Ok ([6;5;4;3;2;1]));

     (Some ("Empty tree"), (identity, Empty), Ok([]));

  ])
