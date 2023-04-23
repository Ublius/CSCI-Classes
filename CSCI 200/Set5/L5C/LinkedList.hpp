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

#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <exception>
#include <iostream>

#include"Node.hpp"

template <typename T>
class LinkedList
{
    public:
        /** 
         * @brief The constructor of the class object; creates an empty list that has 
         * size equal to zero and sets the head & tail pointers to be null
        */
        LinkedList();
        /** 
         * @brief Copy Constructor that performs a deep copy of the other list
         * @param OTHER The other list to be copied
        */
        LinkedList(const LinkedList<T>& OTHER);
        /**
         * @brief The destructor of the class object; destroys the existing list by 
         * removing all nodes that are present in the list
        */
        ~LinkedList();
        /** 
         * @brief Copy Assignment Operator that deallocates the existent list (if any) then performs a deep copy of the other list
         * @param OTHER The other list to be copied
        */
        LinkedList& operator=(const LinkedList<T>& OTHER);
        /**
         * @brief If position doesn't exist, throws a std::out_of_range exception
         * @param POS The position in the list
         * @return Returns the value of the element at given position
        */
        T get(const int POS);
        /**
         * @brief If target doesn't exist, returns -1
         * @param TARGET The target to look for in the list
         * @return Returns the position of the first occurrence of the target
        */
        T find(const T TARGET);
        /**
         * @brief inserts a new node with the provided value BEFORE the provided position. 
         * That is, the newly created node will point to the current node at the provided position. 
         * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate
         * @param POS The postion to add the value in the list
         * @param VALUE The value to add at the position
        */
        void insert(const int POS, const T VALUE);
        /**
         * @brief If the list is empty, throws a std::out_of_range exception
         * @return Returns the max value within the list
        */
        T max();
        /**
         * @brief If the list is empty, throws a std::out_of_range exception
         * @return Returns the min value within the list
        */
        T min();
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
        void set(const int POS, const T VALUE);
        /**
         * @brief A method to get the size of the linked list
         * @return Returns the current number of nodes within the list
        */
        unsigned int size();
        /**
         * @brief Merges two linked lists in ascending order
         * @param left Pointer to the head of the left linked list
         * @param right Pointer to the head of the right linked list
         * @return Pointer to the head of the merged linked list
        */
        Node<T>* merge(Node<T>* left, Node<T>* right);
        /**
         * @brief Splits a linked list into two halves.
         * @param source Pointer to the head of the linked list to be split
         * @param left Pointer to the head of the left half of the linked list after split
         * @param right Pointer to the head of the right half of the linked list after split
        */
        void split(Node<T>* source, Node<T>** left, Node<T>** right);
        /**
         * @brief Sorts the given linked list using the Merge Sort algorithm.
         * @param headRef A pointer to the head of the linked list to be sorted.
        */
        void mergeSort(Node<T>** headRef);
        int binarySearch(LinkedList<T>& list, const T& target);
        /**
         * @brief Pointer to the head node of the list
        */
        Node<T>* mpHead;
    private:
        /**
         * @brief Pointer to the tail node of the list
        */
        Node<T>* mpTail;
        /**
         * @brief Stores the current number of nodes in the list
        */
        unsigned int mSize;
        /**
         * @brief Creates a node with provided value and null previous & next pointers
         * @param VALUE What to store in the node you make
        */
        Node<T>* mMakeNodeForValue(const T VALUE);

};

template <typename T>
LinkedList<T>::LinkedList()
{
    mpHead = nullptr;
    mpTail = nullptr;
    mSize = 0;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& OTHER) 
{
    mpHead = nullptr;
    mSize = 0;
    Node<T>* pCurrent = OTHER.mpHead;
    while (pCurrent != nullptr) 
    {
        insert(1, pCurrent->value);
        pCurrent = pCurrent->pNext;
    }
}

template <typename T>
LinkedList<T>::~LinkedList(){
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

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& OTHER)
{
    if (this != &OTHER) 
    {
        Node<T>* pCurrent = OTHER.mpHead;
        while (pCurrent != nullptr) {
            insert(1, pCurrent->value);
            pCurrent = pCurrent->pNext;
        }
    }
    return *this;
}

template <typename T>
T LinkedList<T>::get(const int POS)
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

template <typename T>
T LinkedList<T>::find(const T TARGET)
{
    const Node<T>* pCURR_NODE = mpHead;
    int counter = 0;
    // advance through list until end and target is found or end reached
    while(pCURR_NODE != nullptr && pCURR_NODE->value != TARGET)
    {
        pCURR_NODE = pCURR_NODE->pNext;
        counter++;
    }
    // return found position or -1 if end of list reached
    return (pCURR_NODE != nullptr) ? counter : -1;
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

template <typename T>
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

template <typename T>
T LinkedList<T>::max()
{
    const Node<T>* pCURR_NODE = mpHead;
    // if list is empty
    if(pCURR_NODE == nullptr) 
        throw std::out_of_range("list is empty"); // throw exception
    // traverse list tracking max value present
    T maxVal = pCURR_NODE->value;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value > maxVal)
            maxVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return maxVal;
}

template <typename T>
T LinkedList<T>::min()
{
    const Node<T>* pCURR_NODE = mpHead;
    // if list is empty
    if(pCURR_NODE == nullptr) 
        throw std::out_of_range("list is empty"); // throw exception
    // traverse list tracking min value present
    T minVal = pCURR_NODE->value;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value < minVal)
            minVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return minVal;
}

template <typename T>
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
        Node<T>* pNodeDelete = mpHead;
        mpHead = mpHead->pNext;
        if(mpHead == nullptr)
            mpTail = nullptr;
        else
        {
            mpHead->pPrev = nullptr;
            pNodeDelete = nullptr;
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
            Node<T>* pNodeDelete = mpTail;
            mpTail = mpTail->pPrev;
            if(mpTail == nullptr)
                mpHead = nullptr;
            else
            {
                mpTail->pNext = nullptr;
                pNodeDelete = nullptr;
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
        pCurrNode->value = VALUE;
}

template <typename T>
unsigned int LinkedList<T>::size()
{
    const Node<T>* pCURR_NODE = mpHead;
    int counter = 0;
    // advance through list until end and count number of jumps
    while(pCURR_NODE != nullptr)
    {
        counter++;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return counter;
}

template <typename T>
Node<T>* merge(Node<T>* left, Node<T>* right) 
{
    Node<T>* result = nullptr;

    // If either list is empty, return the other list
    if (left == nullptr) 
        return right;
   
    if (right == nullptr) 
        return left;

    // Compare the values of the first elements of each list, and set the result to the smaller one
    if (left->value <= right->value) 
    {
        result = left;
        result->pNext = merge(left->pNext, right);
    }
    else 
    {
        result = right;
        result->pNext = merge(left, right->pNext);
    }
    // Return the merged list
    return result;
}

template <typename T>
void split(Node<T>* source, Node<T>** left, Node<T>** right) 
{
    Node<T>* slow = source;
    Node<T>* fast = source->pNext;
    
    while (fast != nullptr) 
    {
        fast = fast->pNext;
        if (fast != nullptr) 
        {
            slow = slow->pNext;
            fast = fast->pNext;
        }
    }

    // Set the left half to the original list, and the right half to the second half of the list
    *left = source;
    *right = slow->pNext;
    slow->pNext = nullptr;
}

template <typename T>
void mergeSort(Node<T>** headRef) 
{
    Node<T>* head = *headRef;
    Node<T>* left;
    Node<T>* right;

    // If the list has zero or one element, it is already sorted
    if (head == nullptr || head->pNext == nullptr) 
        return;

    // Split the list into two halves
    split(head, &left, &right);

    // Recursively sort the two halves
    mergeSort(&left);
    mergeSort(&right);

    // Merge the sorted halves into a single sorted list
    *headRef = merge(left, right);
}

template <typename T>
int binarySearch(LinkedList<T>& list, const T& target) {
    int left = 0;
    int right = list.size() - 1;

    while (left <= right) 
    {
        int mid = (left + right) / 2;
        if (list.get(mid) == target) 
            return mid;
        else if (list.get(mid) < target) 
            left = mid + 1;
        else 
            right = mid - 1;
    }

    return -1;
}

#endif