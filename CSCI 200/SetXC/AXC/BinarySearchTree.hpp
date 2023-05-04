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

#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP

#include <iostream>
#include <queue>
#include <stack>
#include "TreeNode.hpp"

template <typename T>
class BinarySearchTree 
{
    public:
        /**
         * @brief Default constructor
        */
        BinarySearchTree();
        /**
         * @brief Copy constructor
         * @param OTHER The BinarySearchTree object to be copied
        */
        BinarySearchTree(const BinarySearchTree& OTHER);
        /**
         * @brief Destructor
        */
        ~BinarySearchTree();
        /**
         * @brief Assignment operator overload
         * @param OTHER The BinarySearchTree object to be assigned
         * @return Reference to the BinarySearchTree object after assignment
        */
        BinarySearchTree& operator=(const BinarySearchTree& OTHER);
        /**
         * @brief Inserts a value into the BinarySearchTree
         * @param VAL  The value to be inserted
        */
        void insert(const T VAL);
        /**
         * @brief Returns the number of nodes in the BinarySearchTree
         * @return The size of the BinarySearchTree
        */
        int size() const;
        /**
         * @brief Prints the values of the tree in in-order traversal
        */
        void printInOrder() const;
        /**
         * @brief Prints the values of the tree in pre-order traversal
        */
        void printPreOrder() const;
        /**
         * @brief Prints the values of the tree in post-order traversal
        */
        void printPostOrder() const;
        /**
         * @brief Prints the values of the tree in breadth-first order
        */
        void printBreadthOrder() const;
        /**
         * @brief Prints the values of the tree in depth-first order
        */
        void printDepthOrder() const;
        /**
         * @brief Prints the values of the tree by levels
        */
        void printByLevels() const;
        /**
         * @brief Returns the height of the tree
        */
        int height() const;

    private:
        /**
         * @brief Helper function for insert method
         * @param VAL The value to be inserted
         * @param node The current node being examined in the recursion
        */
        void recurvInsert(const T VAL, TreeNode<T>* node);
        /**
         * @brief Recursively print the left subtree, then print the current node, then recursively print the right subtree
         * @param NODE The current node being examined in the recursion
        */
        void printInOrder(const TreeNode<T>* NODE) const; 
        /**
         * @brief Print the current node, then recursively print the left subtree, then recursively print the right subtree
         * @param NODE The current node being examined in the recursion
        */
        void printPreOrder(const TreeNode<T>* NODE) const;
        /**
         * @brief Recursively print the left subtree, then recursively print the right subtree, then print the current node
         * @param NODE The current node being examined in the recursion
        */
        void printPostOrder(const TreeNode<T>* NODE) const;
        /**
         * @brief Pointer to the root node of the BinarySearchTree
        */
        TreeNode<T>* mpRoot;
        /**
         * @brief The size of the BinarySearchTree
        */
        int mSize;
};


template <typename T>
BinarySearchTree<T>::BinarySearchTree()
{
    mpRoot = nullptr;
    mSize = 0;
}

template <typename T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree& OTHER)
{
    // If the other BST object is empty, set the root of this BST to null
    if (!OTHER.mpRoot)
        mpRoot = nullptr;
    // Create a new root node for this BST and copy all the nodes from the other BST 
    else
    {
        mpRoot = new TreeNode<T>(OTHER.mpRoot->data);
        std::queue<TreeNode<T>*> qThis, qOther;
        qThis.push(mpRoot);
        qOther.push(OTHER.mpRoot);

        while(!qOther.empty())
        {
            TreeNode<T>* currThis = qThis.front();
            TreeNode<T>* currOther = qOther.front();
            qThis.pop();
            qOther.pop();

            if(currOther->mpLeft)
            {
                currThis->mpLeft = new TreeNode<T>(currOther->mpLeft->data);
                qThis.push(currThis->mpLeft);
                qOther.push(currOther->mpLeft);
            }
            if(currOther->mpRight)
            {
                currThis->mpRight = new TreeNode<T>(currOther->mpRight->data);
                qThis.push(currThis->mpRight);
                qOther.push(currOther->mpRight);
            }
        }
    }
    mSize = OTHER.mSize;
}

template <typename T>
BinarySearchTree<T>::~BinarySearchTree()
{
    // If the BST is not empty, use a stack to traverse all the nodes and delete them one by one
    if(mpRoot) 
    {
        std::stack<TreeNode<T>*> nodes;
        nodes.push(mpRoot);
        while(!nodes.empty()) 
        {
            TreeNode<T>* current = nodes.top();
            nodes.pop();
            if(current->mpLeft) 
                nodes.push(current->mpLeft);
            if(current->mpRight) 
                nodes.push(current->mpRight);
            delete current;
        }
    }
    mpRoot = nullptr;
    mSize = 0;
}

template <typename T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& OTHER)
{
    // Check if the two BST objects are different
    if (this != &OTHER) 
    {
        // If this BST is not empty, use a stack to traverse all the nodes and delete them one by one
        if (mpRoot) 
        {
            std::stack<TreeNode<T>*> to_delete;
            to_delete.push(mpRoot);
            
            while(!to_delete.empty())
            {
                TreeNode<T>* current = to_delete.top();
                to_delete.pop();
                
                if(current->mpRight) 
                    to_delete.push(current->mpRight);
                
                if(current->mpLeft) 
                    to_delete.push(current->mpLeft);
                
                delete current;
            }
            
            mpRoot = nullptr;
            mSize = 0;
        }
        // If the other BST is not empty, create a new root node for this BST and copy all the nodes from the other BST to this BST using a stack
        if(OTHER.mpRoot)
        {
            mpRoot = new TreeNode<T>(OTHER.mpRoot->data);
            std::stack<TreeNode<T>*> to_copy;
            to_copy.push(mpRoot);
            std::stack<TreeNode<T>*> other_to_copy;
            other_to_copy.push(OTHER.mpRoot);

            while(!other_to_copy.empty())
            {
                TreeNode<T>* current = to_copy.top();
                to_copy.pop();
                TreeNode<T>* other_current = other_to_copy.top();
                other_to_copy.pop();

                if(other_current->mpLeft)
                {
                    current->mpLeft = new TreeNode<T>(other_current->mpLeft->data);
                    to_copy.push(current->mpLeft);
                    other_to_copy.push(other_current->mpLeft);
                }

                if(other_current->mpRight)
                {
                    current->mpRight = new TreeNode<T>(other_current->mpRight->data);
                    to_copy.push(current->mpRight);
                    other_to_copy.push(other_current->mpRight);
                }
            }
            // Updates the size of the current object to match that of the other object
            mSize = OTHER.mSize;
        }
    }
    
    return *this;
}

template <typename T>
void BinarySearchTree<T>::insert(const T VAL)
{
    // If the tree is empty, the new node becomes the root node
    if(!mpRoot) 
            mpRoot = new TreeNode<T>(VAL);
    // If the value is less than the value of the root node, the function recursively inserts the value into the left subtree
    else if (VAL < mpRoot->data) 
    {
        if(mpRoot->mpLeft) 
            recurvInsert(VAL, mpRoot->mpLeft);
        else 
            mpRoot->mpLeft = new TreeNode<T>(VAL);
    }
    // Else, the function recursively inserts the value into the right subtree
    else
    {
        if(mpRoot->mpRight)
            recurvInsert(VAL, mpRoot->mpRight);
        else 
            mpRoot->mpRight = new TreeNode<T>(VAL);
    }
    // After the insertion is complete, the size of the tree is updated
    mSize++;
}

template <typename T>
void BinarySearchTree<T>::recurvInsert(const T VAL, TreeNode<T>* node) 
{
    // If the value is less than the value of the current node, the function recursively inserts the value into the left subtree
    if(VAL < node->data) 
    {
        if(node->mpLeft)
            recurvInsert(VAL, node->mpLeft);
        else 
            node->mpLeft = new TreeNode<T>(VAL);
    } 
    // Else, the function recursively inserts the value into the right subtree.
    else 
    {
        if(node->mpRight) 
            recurvInsert(VAL, node->mpRight);
        else 
            node->mpRight = new TreeNode<T>(VAL);
    }
}

template <typename T>
int BinarySearchTree<T>::size() const 
{
    // If tree is empty, return 
    if (mpRoot == nullptr) 
        return 0;
    
    // Initialize count to 0 and create a stack for DFS
    int count = 0;
    std::stack<TreeNode<T>*> nodeStack;
    nodeStack.push(mpRoot);
    
    // Traverse the tree using DFS and count the number of nodes
    while(!nodeStack.empty()) 
    {
        TreeNode<T>* currentNode = nodeStack.top();
        nodeStack.pop();
        count++;
        
        // Push the left children of the current node into the stack
        if(currentNode->mpLeft != nullptr) 
            nodeStack.push(currentNode->mpLeft);

         // Push the right children of the current node into the stack
        if(currentNode->mpRight != nullptr)
            nodeStack.push(currentNode->mpRight);
    }
    // Return the total count of nodes in the tree
    return count;
}

template <typename T>
void BinarySearchTree<T>::printInOrder() const
{
    // If the tree is not empty, traverse and print the nodes in-order starting from the root
    if(mpRoot)
        printInOrder(mpRoot);
}

template <typename T>
void BinarySearchTree<T>::printInOrder(const TreeNode<T>* NODE) const 
{
    // Traverse and print the left child of the current node first
    if(NODE->mpLeft)
        printInOrder(NODE->mpLeft);
    // Print the current node
    std::cout << NODE->data << " ";
    // Traverse and print the right child of the current node last
    if(NODE->mpRight)
        printInOrder(NODE->mpRight);
}

template <typename T>
void BinarySearchTree<T>::printPreOrder() const
{
    // If the tree is not empty, traverse and print the nodes pre-order starting from the root
    if(mpRoot) 
        printPreOrder(mpRoot);
}

template <typename T>
void BinarySearchTree<T>::printPreOrder(const TreeNode<T>* NODE) const 
{
    // Print the current node first
    std::cout << NODE->data << " ";
    // Traverse and print the left child of the current node next
    if(NODE->mpLeft)
        printPreOrder(NODE->mpLeft);
    // Traverse and print the right child of the current node last
    if(NODE->mpRight)
        printPreOrder(NODE->mpRight);
}

template <typename T>
void BinarySearchTree<T>::printPostOrder() const 
{
    // If the tree is not empty, traverse and print the nodes post-order starting from the root
    if(mpRoot) 
        printPostOrder(mpRoot);
}

template <typename T>
void BinarySearchTree<T>::printPostOrder(const TreeNode<T>* NODE) const 
{
    // Traverse and print the left child of the current node first
    if(NODE->mpLeft)
        printPostOrder(NODE->mpLeft);
    // Traverse and print the right child of the current node next
    if(NODE->mpRight)
        printPostOrder(NODE->mpRight);
    // Print the current node last
    std::cout << NODE->data << " ";
}

template <typename T>
void BinarySearchTree<T>::printBreadthOrder() const 
{
    // If the tree is empty, return
    if(!mpRoot) 
        return;

    // Create a queue for BFS and push the root node
    std::queue<TreeNode<T>*> q;
    q.push(mpRoot);
    // Continue traversing the tree while the queue is not empty
    while(!q.empty()) 
    {
        // Get the next node in the queue
        TreeNode<T>* current = q.front();
        q.pop();
        // Print the data in the current node
        std::cout << current->data << " ";
        // If the current node has a left child, add it to the queue
        if(current->mpLeft)
            q.push(current->mpLeft);
        // If the current node has a right child, add it to the queue
        if(current->mpRight)
            q.push(current->mpRight);
    }
    std::cout << std::endl;
}

template <typename T>
void BinarySearchTree<T>::printDepthOrder() const 
{
    // If the tree is empty, there is nothing to print, so return
    if(!mpRoot) 
        return;

    // Create a stack for DFS and push the root node
    std::stack<TreeNode<T>*> s;
    s.push(mpRoot);
    // Continue traversing the tree while the stack is not empty
    while(!s.empty()) 
    {
        // Get the next node in the stack
        TreeNode<T>* current = s.top();
        s.pop();
        // Print the data in the current node
        std::cout << current->data << " ";
        // If the current node has a right child, add it to the stack
        if(current->mpRight)
            s.push(current->mpRight);
         // If the current node has a left child, add it to the stack
        if(current->mpLeft)
            s.push(current->mpLeft);
    }
    std::cout << std::endl;
}

template <typename T>
void BinarySearchTree<T>::printByLevels() const 
{
    // If the tree is empty, there is nothing to print, so return
    if(!mpRoot)
        return;

    // Create a queue for BFS and push the root node
    std::queue<TreeNode<T>*> q;
    q.push(mpRoot);
    // Set the initial level and level size
    int level = 1;
    int level_size = 1;
    // Continue traversing the tree while the queue is not empty
    while(!q.empty()) 
    {
        // Print the current level number
        std::cout << "Level " << level << ": ";
         // Traverse the nodes in the current level and print their data
        for(int i = 0; i < level_size; i++)
        {
            TreeNode<T>* current = q.front();
            q.pop();
            std::cout << current->data << " ";
            // If the current node has a left child, add it to the queue
            if(current->mpLeft)
                q.push(current->mpLeft);
            // If the current node has a right child, add it to the queue
            if(current->mpRight)
                q.push(current->mpRight);
        }
        std::cout << std::endl;
        level++;
        level_size = q.size();
    }
}   

template <typename T>
int BinarySearchTree<T>::height() const
{
    // If the tree is empty, its height is 0
    if(mpRoot == nullptr)
        return 0;

    // Create a queue for BFS and push the root node
    std::queue<TreeNode<T>*> nodeQueue;
    nodeQueue.push(mpRoot);
    int height = 0;
    // Traverse the tree level by level
    while (!nodeQueue.empty()) 
    {
        // Get the number of nodes at the current level
        int levelSize = nodeQueue.size();
        // Traverse all the nodes at the current level
        while(levelSize--) 
        {
            // Get the first node in the queue
            TreeNode<T>* curr = nodeQueue.front();
            nodeQueue.pop();
            // Add the left children of the current node to the queue if they exist
            if(curr->mpLeft != nullptr)
                nodeQueue.push(curr->mpLeft);
            // Add the right children of the current node to the queue if they exist
            if(curr->mpRight != nullptr)
                nodeQueue.push(curr->mpRight);
        }
        // Increase the height by 1 for each level of the tree traversed
        height++;
    }
    return height;
}

#endif