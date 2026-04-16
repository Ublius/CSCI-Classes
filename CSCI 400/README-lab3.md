The purpose of this assignment is to help you understand algebraic
data types and persistent data structures.

Instructions
============

1. Do not re-clone this repository.  Merge the updated starter code
   into your team's repository as described in the main `README` file
   and Lab 0.

2. Part I: Implement the following functions in `lab3_btree.ml`:
   - `exp_eval`: O(n), ~10 lines
   - `exp_string`: O(n), ~10 lines
   - `map_inorder`: O(n), ~8 lines
   - `map_tree`: O(n), ~8 lines
   - `is_bst`: O(n), ~12 lines
   - `bst_min`: O(height), ~5 lines
   - `bst_insert`: O(height), ~8 lines
   - `bst_search`: O(height), ~8 lines
   - `bst_remove_min`: O(height), ~10 lines

3. Part II: Implement the following functions in `lab3_rbtree.ml`:
   - `is_invariant`: O(n), ~15 lines
   - `search`: O(h) = O(ln n), ~8 lines
   - `balance`: O(1), ~10 lines
   - `insert_helper`: O(h) = O(ln n), ~12 lines
   - `map_inorder`: O(n), ~6 lines
   - `map_revorder`: O(n), ~6 lines

5. Add at least 5 new *non-trivial* unit tests per function, at the
   locations marked by `TODO` comments.  Note that some functions are
   tested with multiple value types.

6. Test your code!

7. Push all above changes to your team repo in the github.com course
   organization.  Ensure that the code you want graded is in the
   master branch before the deadline.

Building and Testing
====================

- Type `make lab3` to build the lab
- Type `make lab3_test` to run the test cases defined in `src/lab3.ml`
- Each unit test is a tuple of the form `(optional_name, input,
  expected_output)`, where `optional_name` can either be `None` or
  `Some(x)`, where `x` is a human-readable name for the unit test. The
  `expected_output` is a `result` type, which allows you to use
  `Ok(out)` for regular output `out`, or `Error(ex)` if the unit test
  is expected to generate an exception `ex`.

Allowable OCaml Subset
======================

To help you learn to think in the functional programming style, this
project is restricted to a subset of OCaml.

You are permitted to use:
- binding (let)
- function definition (let, fun, let rec)
- function application (f x)
- pattern matching (match e with ...)
- conditionals (if e1 then e2 else e3)
- cons and list construction (a::d and [...])
- tuples ((...))
- arithmetic, Boolean, and equality operations (+,-,/,*,=, etc.)
- options (Some X, None)

You are NOT permitted to use:
- Loops (for, while)
- Mutable variables (ref)
- OCaml libraries, operators, or functions that directly implement the
  functions you must implement.  For example, you may not use any
  existing tree or set libraries.