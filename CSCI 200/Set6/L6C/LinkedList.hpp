/* CSCI 200: Lab 6C - The Abstract List
 *
 * Author: Tristen Erpelding
 *
 * We will create an abstract templated List class that supports a subset of the list operations we have been working with.
 * Create the Array class which extends List using public inheritance.
 * Create the LinkedList class which extends List using public inheritance.
 * 
*/

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include "List.hpp"
#include <iostream>

/**
 * @brief A single element of a linked list
 * 
*/
template<typename T>
struct Node 
{
    /**
     * @brief the value of this Node
    */
    T value;
    /**
     * @brief pointer to the next element of the linked list
    */
    Node<T> *pNext;
    /**
     * @brief pointer to the previous element of the linked list
    */
    Node<T> *pPrev;
};

template<typename T>
class LinkedList : public List<T> 
{
public:
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
    T get(const int POS) const override;
    /**
     * @brief If the provided position exists in the list, sets the vale of the associated node. 
     * If the provided position is out of range for the current list, do nothing
     * @param POS The postion to change the value in the list
     * @param VALUE The value to change at the position
    */
    void set(const int POS, const T VALUE) override;
    /**
     * @brief inserts a new node with the provided value BEFORE the provided position. 
     * That is, the newly created node will point to the current node at the provided position. 
     * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate
     * @param POS The postion to add the value in the list
     * @param VALUE The value to add at the position
    */
    void insert(const int POS, const T VALUE) override;
    /**
     * @brief if the provided position exists in the list, removes and deallocates the associated node.
     * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate. 
     * If the list is empty, then do nothing
     * @param POS The postion to remove the value in the list
    */
    void remove(const int POS) override;

private:
    /**
     * @brief Creates a node with provided value and null previous & next pointers
     * @param VALUE What to store in the node you make
    */
    Node<T>* mMakeNodeForValue(const T VALUE);
    /**
     * @brief Pointer to the head node of the list
    */
    Node<T>* mpHead;
    /**
     * @brief Pointer to the tail node of the list
    */
    Node<T>* mpTail;
};

template<typename T>
LinkedList<T>::~LinkedList() 
{
    while(mpHead)
    {
        Node<T>* pNodeDelete = mpHead;
        mpHead = mpHead->pNext;
        if(mpHead == nullptr)
        {
            pNodeDelete = nullptr;
            mpTail = nullptr;
        }
        else
        {
            delete pNodeDelete;
            pNodeDelete = nullptr;
        }
    }
}

template<typename T>
T LinkedList<T>::get(const int POS) const 
{
    const Node<T>* pCURR_NODE = mpHead;
    int counter = 0;
    // advance through list counting number of jumps and continue until position found or at end
    if(POS < 0)
        throw std::out_of_range("Position is out of range");
    // if valid position, return value
    while(pCURR_NODE && counter < POS)
    {
        pCURR_NODE = pCURR_NODE->pNext;
        counter++;
    }
    // otherwise throw excepetion
    if(!pCURR_NODE || counter != POS)
        throw std::out_of_range("Position is out of range");
    
    return pCURR_NODE->value;
}

template<typename T>
void LinkedList<T>::set(const int POS, const T VALUE) 
{
    Node<T>* pCurrNode = mpHead;
    int counter = 0;
    // advance through list counting number of jumps and continue until position found or at end
    while(pCurrNode != nullptr && counter < POS)
    {
        counter++;
        pCurrNode = pCurrNode->pNext;
    }
    // if valid position, change value
    if(pCurrNode != nullptr && counter == POS)
        pCurrNode->value = VALUE;;
}

template<typename T>
void LinkedList<T>::insert(const int POS, const T VALUE) 
{
    // if position is before the beginning, add to front
    if(POS <= 0 || mpHead == nullptr)
    {
        Node<T>* pNewNode = mMakeNodeForValue(VALUE);
        // if list is initially empty
        if(!mpHead)
        {
            // head and tail are the new node
            mpHead = pNewNode;
            mpTail = pNewNode;
        }
        // otherwise, chain into existing node
        else
        {
            // link new node and head, update head
            pNewNode->pNext = mpHead;
            mpHead->pPrev = pNewNode;
            mpHead = pNewNode;
        }
    }
    else 
    {
        Node<T>* pCurrNode = mpHead;
        int counter = 0;
        // advance through list counting number of jumps and continue until position found or at end
        while(pCurrNode != nullptr && counter < POS) 
        {
            counter++;
            pCurrNode = pCurrNode->pNext;
        }
        // if we have gone past the end
        if(!pCurrNode) 
        {
            Node<T>* pNewNode = mMakeNodeForValue(VALUE);
            if(!mpHead)
            {
                // head and tail are the new node       
                mpHead = pNewNode;
                mpTail = pNewNode;
            }
            // otherwise, chain off existing node
            else
            {
                // link new node and tail, update tail
                mpTail->pNext = pNewNode;
                pNewNode->pPrev = mpTail;
                mpTail = pNewNode;
            }
        }       
        else 
        {
            // otherwise add before target position
            Node<T>* pNewNode = mMakeNodeForValue(VALUE);
            Node<T>* prevNode = pCurrNode->pPrev;
            pNewNode->pPrev = prevNode;
            pNewNode->pNext = pCurrNode;
            pCurrNode->pPrev = pNewNode;
            prevNode->pNext = pNewNode;
        }
    }
}

template<typename T>
void LinkedList<T>::remove(const int POS) 
{
    // if list is empty
    if(mpHead == nullptr)
        return; // do nothing
    // if position is before start of list
    else if(POS <= 0)
    {
        // if list is empty, do nothing
        if(mpHead == nullptr)
            return;
        // Node<T>* pNodeDelete = mpHead;
        mpHead = mpHead->pNext;
        if(mpHead == nullptr)
            mpTail = nullptr;
        else
        {
            mpHead->pPrev = nullptr;
            // pNodeDelete = nullptr;
        }
    }
    else 
    {
        Node<T>* pCurrNode = mpHead;
        int counter = 0;
        // advance through list counting number of jumps and continue until position found or at end
        while(counter < POS && pCurrNode != nullptr)
        {
            counter++;
            pCurrNode = pCurrNode->pNext;
        }
        // if we have gone to tail or further
        if(pCurrNode == nullptr || pCurrNode == mpTail)
        {
            if(mpTail == nullptr)
                return;
            // Node<T>* pNodeDelete = mpTail;
            mpTail = mpTail->pPrev;
            if(mpTail == nullptr)
                mpHead = nullptr;
            else
            {
                mpTail->pNext = nullptr;
                // pNodeDelete = nullptr;
            }
        }
        else 
        {
            Node<T>* prevNode = pCurrNode->pPrev;
            Node<T>* nextNode = pCurrNode->pNext;
            prevNode->pNext = nextNode;
            nextNode->pPrev = prevNode;
            // delete current node
            pCurrNode = nullptr;
        }
    }
}

template <typename T>
Node<T>* LinkedList<T>::mMakeNodeForValue(const T VALUE)
{
    Node<T>* pNode = new Node<T>;
    pNode->value = VALUE;
    pNode->pNext = nullptr;
    pNode->pPrev = nullptr;
    return pNode;
}

#endif