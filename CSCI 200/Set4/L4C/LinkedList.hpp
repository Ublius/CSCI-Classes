/* CSCI 200: Assignment 4: L4B - A Linked List Class Test Suite
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L4A
 * Creating the LinkedList class declaration. 
 * This class will represent a LinkedList data structure. 
 * Its contained state will be the actual linked list and it will contain the methods to manipulate that state - the linked list.
 */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include"Node.h"

class LinkedList
{
    public:
        /** 
         * @brief The constructor of the class object; creates an empty list that has 
         * size equal to zero and sets the head & tail pointers to be null
        */
        LinkedList();
        /**
         * @brief The destructor of the class object; destroys the existing list by 
         * removing all nodes that are present in the list
        */
        ~LinkedList();
        /**
         * @brief If position doesn't exist, throws a std::out_of_range exception
         * @param POS The position in the list
         * @return Returns the value of the element at given position
        */
        int get(const int POS);
        /**
         * @brief If target doesn't exist, returns -1
         * @param TARGET The target to look for in the list
         * @return Returns the position of the first occurrence of the target
        */
        int find(const int TARGET);
        /**
         * @brief inserts a new node with the provided value BEFORE the provided position. 
         * That is, the newly created node will point to the current node at the provided position. 
         * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate
         * @param POS The postion to add the value in the list
         * @param VALUE The value to add at the position
        */
        void insert(const int POS, const int VALUE);
        /**
         * @brief If the list is empty, throws a std::out_of_range exception
         * @return Returns the max value within the list
        */
        int max();
        /**
         * @brief If the list is empty, throws a std::out_of_range exception
         * @return Returns the min value within the list
        */
        int min();
        /**
         * @brief if the provided position exists in the list, removes and deallocates the associated node.
         * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate. 
         * If the list is empty, then do nothing
         * @param POS The postion to remove the value in the list
        */
        void remove(const int POS);
        /**
         * @brief If the provided position exists in the list, sets the vale of the associated node. 
         * If the provided position is out of range for the current list, do nothing
         * @param POS The postion to change the value in the list
         * @param VALUE The value to change at the position
        */
        void set(const int POS, const int VALUE);
        /**
         * @brief A method to get the size of the linked list
         * @return Returns the current number of nodes within the list
        */
        unsigned int size();

    private:
        /**
         * @brief Pointer to the head node of the list
        */
        Node* mpHead;
        /**
         * @brief Pointer to the tail node of the list
        */
        Node* mpTail;
        /**
         * @brief Stores the current number of nodes in the list
        */
        unsigned int mSize;
        /**
         * @brief Creates a node with provided value and null previous & next pointers
         * @param VALUE What to store in the node you make
        */
        Node* mMakeNodeForValue(const int VALUE);

};

#endif