/* CSCI 200: Lab XC - Binary Search Trees
 *
 * Author: Tristen Erpelding
 *
 * We are going to now make a new templated class called BinarySearchTree. 
 * This will necessitate creating a new struct for a TreeNode to store the left and right pointers as appropriate
*/

#include <iostream>
#include "BinarySearchTree.hpp"

int main() 
{
    BinarySearchTree<int> tree;
    // Add values to the tree
    tree.insert(6);
    tree.insert(5);
    tree.insert(7);
    tree.insert(1);
    tree.insert(2);
    tree.insert(9);
    tree.insert(3);
    // print the size of the tree
    std::cout << "Binary Tree Size: " << tree.size() << std::endl;
    return 0;
}