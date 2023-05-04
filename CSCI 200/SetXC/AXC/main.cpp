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

#include <iostream>
#include "BinarySearchTree.hpp"

int main() 
{
    std::cout << "Tree 1:" << std::endl;
    BinarySearchTree<int> tree1;

    // Add values to the tree
    tree1.insert(6);
    tree1.insert(5);
    tree1.insert(7);
    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(9);
    tree1.insert(3);

    // Print the tree using the three traversal methods
    std::cout << "In Order: ";
    tree1.printInOrder();
    std::cout << std::endl;

    std::cout << "Pre Order: ";
    tree1.printPreOrder();
    std::cout << std::endl;

    std::cout << "Post Order: ";
    tree1.printPostOrder();
    std::cout << std::endl;

    std::cout << "Breadth Order: ";
    tree1.printBreadthOrder();

    std::cout << "Depth Order: ";
    tree1.printDepthOrder();

    std::cout << "By Levels: " << std::endl;
    tree1.printByLevels();

    std::cout << "Height: " << tree1.height() << std::endl;

    std::cout << std::endl;
    std::cout << "Tree 2:" << std::endl;
    BinarySearchTree<int> tree2;

    // Add values to the tree
    tree2.insert(5);
    tree2.insert(1);
    tree2.insert(9);
    tree2.insert(7);
    tree2.insert(6);
    tree2.insert(3);
    tree2.insert(2);

    // Print the tree using the three traversal methods
    std::cout << "In Order: ";
    tree2.printInOrder();
    std::cout << std::endl;

    std::cout << "Pre Order: ";
    tree2.printPreOrder();
    std::cout << std::endl;

    std::cout << "Post Order: ";
    tree2.printPostOrder();
    std::cout << std::endl;

    std::cout << "Breadth Order: ";
    tree2.printBreadthOrder();

    std::cout << "Depth Order: ";
    tree2.printDepthOrder();

    std::cout << "By Levels: " << std::endl;
    tree2.printByLevels();

    std::cout << "Height: " << tree2.height() << std::endl;

    std::cout << std::endl;
    
    std::cout << "Tree 3:" << std::endl;
    BinarySearchTree<int> tree3;

    // Add values to the tree
    tree3.insert(5);
    tree3.insert(2);
    tree3.insert(1);
    tree3.insert(7);
    tree3.insert(9);
    tree3.insert(6);
    tree3.insert(3);

    // Print the tree using the three traversal methods
    std::cout << "In Order: ";
    tree3.printInOrder();
    std::cout << std::endl;

    std::cout << "Pre Order: ";
    tree3.printPreOrder();
    std::cout << std::endl;

    std::cout << "Post Order: ";
    tree3.printPostOrder();
    std::cout << std::endl;

    std::cout << "Breadth Order: ";
    tree3.printBreadthOrder();

    std::cout << "Depth Order: ";
    tree3.printDepthOrder();

    std::cout << "By Levels: " << std::endl;
    tree3.printByLevels();

    std::cout << "Height: " << tree3.height() << std::endl;

    return 0;
}