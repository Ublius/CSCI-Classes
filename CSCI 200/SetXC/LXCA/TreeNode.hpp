/* CSCI 200: Lab XC - Binary Search Trees
 *
 * Author: Tristen Erpelding
 *
 * We are going to now make a new templated class called BinarySearchTree. 
 * This will necessitate creating a new struct for a TreeNode to store the left and right pointers as appropriate
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