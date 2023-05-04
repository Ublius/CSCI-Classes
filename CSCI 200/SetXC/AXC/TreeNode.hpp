/* CSCI 200: Assessment AXC - Forest Walk: Traversing BSTs
 *
 * Author: Tristen Erpelding
 *
 * For this assignment, we will verify our tree is created correctly by printing out the tree in different ways and some additional stats.
 * We'll expand our Tree class by adding the following public methods:
    printInOrder(): Recursively print the left subtree, then print the current node, then recursively print the right subtree.
    printPreOrder(): Print the current node, then recursively print the left subtree, then recursively print the right subtree.
    printPostOrder(): Recursively print the left subtree, then recursively print the right subtree, then print the current node.
 * We'll now add two more ways to traverse the tree:
    printBreadthOrder(): Print the tree level by level using a breadth first search.
    printDepthOrder(): Print the tree branch by branch using a depth first search.
 * We'll now add a way to pretty print by level:
    printByLevels(): Print the tree level by level, each on their own line.
 * Finally, add a method to print the height, or number of levels, of the tree.
    height(): Print the number of levels in the tree.
*/

#ifndef TREENODE_HPP
#define TREENODE_HPP

/**
 * @brief A single element of a Binary Tree
*/
template <typename T>
struct TreeNode 
{
    /**
     * @brief the value of this Node
    */
    T data;
    /**
     * @brief pointer to the left element of the binary tree
    */
    TreeNode<T> *mpLeft;
    /**
     * @brief pointer to the right element of the binary list
    */
    TreeNode<T> *mpRight;

    TreeNode(T val) : data(val), mpLeft(nullptr), mpRight(nullptr) {}
};
#endif