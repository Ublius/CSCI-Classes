open Testing
open Lab3_btree
open Lab3_rbtree

let main () =
  print_string "Running Lab 3\n";
  print_string "=============\n";

  (*Binary Search Tree*)
  Testing.print_tests Lab3_btree.map_inorder_tests;
  Testing.print_tests Lab3_btree.map_tree_tests;
  Testing.print_tests Lab3_btree.map_tree_char_tests;
  Testing.print_tests Lab3_btree.is_bst_tests_int;
  Testing.print_tests Lab3_btree.is_bst_tests_str;
  Testing.print_tests Lab3_btree.bst_max_tests;
  Testing.print_tests Lab3_btree.bst_min_tests;
  Testing.print_tests Lab3_btree.bst_insert_tests_int;
  Testing.print_tests Lab3_btree.bst_insert_tests_str;
  Testing.print_tests Lab3_btree.bst_search_tests_int;
  Testing.print_tests Lab3_btree.bst_search_tests_str;
  Testing.print_tests Lab3_btree.bst_remove_min_tests;
  Testing.print_tests Lab3_btree.bst_remove_tests_int;
  Testing.print_tests Lab3_btree.bst_remove_tests_str;
  (* Expressions *)
  Testing.print_tests Lab3_btree.exp_string_tests;
  Testing.print_tests Lab3_btree.exp_eval_tests;

  (* Red-Black Trees *)
  Testing.print_tests Lab3_rbtree.rbt_is_invariant_int_tests;
  Testing.print_tests Lab3_rbtree.rbt_is_invariant_str_tests;
  Testing.print_tests Lab3_rbtree.rbt_search_int_tests;
  Testing.print_tests Lab3_rbtree.rbt_search_str_tests;
  Testing.print_tests Lab3_rbtree.rbt_balance_int_tests;
  Testing.print_tests Lab3_rbtree.rbt_balance_str_tests;
  Testing.print_tests Lab3_rbtree.rbt_insert_int_tests;
  Testing.print_tests Lab3_rbtree.rbt_insert_str_tests;
  Testing.print_tests Lab3_rbtree.rbt_map_inorder_tests;
  Testing.print_tests Lab3_rbtree.rbt_map_revorder_tests;

;;

main ()

