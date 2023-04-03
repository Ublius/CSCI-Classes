/* CSCI 200: Assignment 4: L4C - A Templated Linked List Class
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L4A, L4B
 * Generalize the Linked List created in Lab4B. 
 * Change your Node and LinkedList classes to be templated
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
    int value;
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