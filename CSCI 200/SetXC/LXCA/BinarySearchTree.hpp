/* CSCI 200: Lab XC - Binary Search Trees
 *
 * Author: Tristen Erpelding
 *
 * We are going to now make a new templated class called BinarySearchTree. 
 * This will necessitate creating a new struct for a TreeNode to store the left and right pointers as appropriate
*/

#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP

#include <stack>
#include <queue>

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

    private:
        /**
         * @brief Helper function for insert method
         * @param VAL The value to be inserted
         * @param node The current node being examined in the recursion
        */
        void recurvInsert(const T VAL, TreeNode<T>* node);
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
        while (!nodes.empty()) 
        {
            TreeNode<T>* current = nodes.top();
            nodes.pop();
            if (current->mpLeft) nodes.push(current->mpLeft);
            if (current->mpRight) nodes.push(current->mpRight);
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
            
            while (!to_delete.empty())
            {
                TreeNode<T>* current = to_delete.top();
                to_delete.pop();
                
                if (current->mpRight) 
                    to_delete.push(current->mpRight);
                
                if (current->mpLeft) 
                    to_delete.push(current->mpLeft);
                
                delete current;
            }
            
            mpRoot = nullptr;
            mSize = 0;
        }
        // If the other BST is not empty, create a new root node for this BST and copy all the nodes from the other BST to this BST using a stack
        if (OTHER.mpRoot)
        {
            mpRoot = new TreeNode<T>(OTHER.mpRoot->data);
            std::stack<TreeNode<T>*> to_copy;
            to_copy.push(mpRoot);
            std::stack<TreeNode<T>*> other_to_copy;
            other_to_copy.push(OTHER.mpRoot);

            while (!other_to_copy.empty())
            {
                TreeNode<T>* current = to_copy.top();
                to_copy.pop();
                TreeNode<T>* other_current = other_to_copy.top();
                other_to_copy.pop();

                if (other_current->mpLeft)
                {
                    current->mpLeft = new TreeNode<T>(other_current->mpLeft->data);
                    to_copy.push(current->mpLeft);
                    other_to_copy.push(other_current->mpLeft);
                }

                if (other_current->mpRight)
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
    if (!mpRoot) 
            mpRoot = new TreeNode<T>(VAL);
    // If the value is less than the value of the root node, the function recursively inserts the value into the left subtree
    else if (VAL < mpRoot->data) 
    {
        if (mpRoot->mpLeft) 
            recurvInsert(VAL, mpRoot->mpLeft);
        else 
            mpRoot->mpLeft = new TreeNode<T>(VAL);
    }
    // Else, the function recursively inserts the value into the right subtree
    else
    {
        if (mpRoot->mpRight)
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
    if (VAL < node->data) 
    {
        if (node->mpLeft)
            recurvInsert(VAL, node->mpLeft);
        else 
            node->mpLeft = new TreeNode<T>(VAL);
    } 
    // Else, the function recursively inserts the value into the right subtree.
    else 
    {
        if (node->mpRight) 
            recurvInsert(VAL, node->mpRight);
        else 
            node->mpRight = new TreeNode<T>(VAL);
    }
}

template <typename T>
int BinarySearchTree<T>::size() const 
{
    if (mpRoot == nullptr) 
        return 0;
    
    int count = 0;
    std::stack<TreeNode<T>*> nodeStack;
    nodeStack.push(mpRoot);
    
    while(!nodeStack.empty()) 
    {
        TreeNode<T>* currentNode = nodeStack.top();
        nodeStack.pop();
        count++;
        
        if(currentNode->mpLeft != nullptr) 
            nodeStack.push(currentNode->mpLeft);

        
        if(currentNode->mpRight != nullptr)
            nodeStack.push(currentNode->mpRight);
    }
    
    return count;
}

#endif