/* CSCI 200: Assignment 5: L5C - A Needle in the Haystack
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L5B
 * To get started, prompt the user to enter the number of integers to store in a list. 
 * Then generate n random integers. Finally, sort the list into ascending order.
 * Now we'll have the user give us a target value to search for. 
 * Implement the binary search algorithm
*/

#ifndef NODE_H
#define NODE_H

/**
 * @brief A single element of a linked list
 * 
 */
template <typename T>
struct Node {
    /**
     * @brief the value of this Node
     * 
     */
    T value;
    /**
     * @brief pointer to the next element of the linked list
     * 
     */
    Node<T> *pNext;
    /**
     * @brief pointer to the previous element of the linked list
     * 
     */
    Node<T> *pPrev;
};

#endif