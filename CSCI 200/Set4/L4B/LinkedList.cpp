/* CSCI 200: Assignment 4: L4B - A Linked List Class Test Suite
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L4A
 * Creating the LinkedList class declaration. 
 * This class will represent a LinkedList data structure. 
 * Its contained state will be the actual linked list and it will contain the methods to manipulate that state - the linked list.
 */

#include "Node.h"
#include "LinkedList.h"

#include <exception>
#include <iostream>

LinkedList::LinkedList(){
    mpHead = nullptr;
    mpTail = nullptr;
    mSize = 0;
}

LinkedList::~LinkedList(){
    while(mpHead)
    {
        Node* pNodeDelete = mpHead;
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

int LinkedList::get(const int POS)
{
    const Node* pCURR_NODE = mpHead;
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

int LinkedList::find(const int TARGET)
{
    const Node* pCURR_NODE = mpHead;
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

Node* LinkedList::mMakeNodeForValue(const int VALUE)
{
    Node* pNode = new Node;
    pNode->value = VALUE;
    pNode->pNext = nullptr;
    pNode->pPrev = nullptr;
    return pNode;
}

void LinkedList::insert(const int POS, const int VALUE)
{
    // if position is before the beginning, add to front
    if(POS <= 0 || mpHead == nullptr)
    {
        Node* pNewNode = mMakeNodeForValue(VALUE);
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
        Node* pCurrNode = mpHead;
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
            Node* pNewNode = mMakeNodeForValue(VALUE);
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
            Node* pNewNode = mMakeNodeForValue(VALUE);
            Node* prevNode = pCurrNode->pPrev;
            pNewNode->pPrev = prevNode;
            pNewNode->pNext = pCurrNode;
            pCurrNode->pPrev = pNewNode;
            prevNode->pNext = pNewNode;
        }
    }
}

int LinkedList::max()
{
    const Node* pCURR_NODE = mpHead;
    // if list is empty
    if(pCURR_NODE == nullptr) 
        throw std::out_of_range("list is empty"); // throw exception
    // traverse list tracking max value present
    int maxVal = pCURR_NODE->value;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value > maxVal)
            maxVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return maxVal;
}

int LinkedList::min()
{
    const Node* pCURR_NODE = mpHead;
    // if list is empty
    if(pCURR_NODE == nullptr) 
        throw std::out_of_range("list is empty"); // throw exception
    // traverse list tracking min value present
    int minVal = pCURR_NODE->value;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value < minVal)
            minVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return minVal;
}

void LinkedList::remove(const int POS)
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
        Node* pNodeDelete = mpHead;
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
        Node* pCurrNode = mpHead;
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
            Node* pNodeDelete = mpTail;
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
            Node* prevNode = pCurrNode->pPrev;
            Node* nextNode = pCurrNode->pNext;
            prevNode->pNext = nextNode;
            nextNode->pPrev = prevNode;
            // delete current node
            pCurrNode = nullptr;
        }
    }
}
 
void LinkedList::set(const int POS, const int VALUE)
{
    Node* pCurrNode = mpHead;
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

unsigned int LinkedList::size()
{
    const Node* pCURR_NODE = mpHead;
    int counter = 0;
    // advance through list until end and count number of jumps
    while(pCURR_NODE != nullptr)
    {
        counter++;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return counter;
}
