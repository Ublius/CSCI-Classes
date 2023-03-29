/* CSCI 200: Assignment 4: L4A - Linked List Test Suite
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     Used the class sides    
 *
 * Your task is to implement the functions declared in linked_list_functions.h 
 * such that all of the tests within the test suite pass.
 */

#include "linked_list_functions.h"

#include <exception>
#include <iostream>
#include <string>

Node* linked_list_make_node(const int VALUE) {
    Node* pNode = new Node;
    pNode->value = VALUE;
    pNode->pNext = nullptr;
    pNode->pPrev = nullptr;
    return pNode;
}

void linked_list_add_value_to_front(Node*& pHead, Node*& pTail, const int VALUE) {
    // make a new node
    Node* pNewNode = linked_list_make_node(VALUE);      
    // if list is initially empty
    if(!pHead)
    {
        // head and tail are the new node        
        pHead = pNewNode;
        pTail = pNewNode;
    }
    // otherwise, chain into existing node
    else
    {
        // link new node and head, update head
        pNewNode->pNext = pHead;
        pNewNode->pPrev = nullptr;
        pHead->pPrev = pNewNode;
        pHead = pNewNode;
    }
}

int linked_list_get_size(const Node* const P_HEAD, const Node* const P_TAIL) {
    const Node* pCURR_NODE = P_HEAD;
    int counter = 0;
    // advance through list until end and count number of jumps
    while(pCURR_NODE != nullptr)
    {
        counter++;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return counter;
}

int linked_list_get_value_at_position(const Node* const P_HEAD, const Node* const P_TAIL, const int POS) {
    const Node* pCURR_NODE = P_HEAD;
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

void linked_list_set_value_at_position(Node* const P_head, Node* const P_tail, const int POS, const int VALUE) {
    // if trying to set an invalid spot, or list is empty - do nothing
    if(P_head == nullptr || POS < 0) 
        return;
    Node* pCurrNode = P_head;
    int counter = 0;
    // advance through list counting number of jumps and continue until position found or at end
    while(pCurrNode != nullptr && counter < POS)
    {
        pCurrNode = pCurrNode->pNext;
        counter++;
    }
    // if valid position, change value
    if(pCurrNode != nullptr && counter == POS)
        pCurrNode->value = VALUE;
    
}

void linked_list_remove_node_at_front(Node*& pHead, Node*& pTail) {
    // if list is empty - do nothing
    if(pHead == nullptr) return;                
    // if we're now pointing at nothing, list only had one element
    if(pHead == pTail)
    {
        delete pHead;
        pHead = nullptr;
        //  set tail to also be null so it points to nothing
        pTail = nullptr;
        return;
    }
    // otherwise, list still has elements
    // store pointer to current head
    Node* pOldHead = pHead;
    // advance head
    pHead = pHead->pNext;
    // set back pointer to be null
    pHead->pPrev = nullptr;
    // delete former head
    delete pOldHead;
}

void linked_list_add_value_to_back(Node*& pHead, Node*& pTail, const int VALUE) {
    // make new node
    Node* pNewNode = linked_list_make_node(VALUE);
    // if list is initially empty
    if(!pHead)
    {
        // head and tail are the new node        
        pHead = pNewNode;
        pTail = pNewNode;
    }                 
    // otherwise, chain off existing node
    else
    {
        // link new node and tail, update tail
        pNewNode->pNext = nullptr;
        pNewNode->pPrev = pTail;
        pTail->pNext = pNewNode;
        pTail = pNewNode;
    }
}

void linked_list_remove_node_at_back(Node*& pHead, Node*& pTail) {
    // if list is empty, do nothing
    if(pTail == nullptr) return;
    // if we're now pointing at nothing, list only had one element
    if(pHead == pTail)
    {
        delete pHead;
        //  set head to also be null so it points to nothing
        pHead = nullptr;
        //  set tail to also be null so it points to nothing
        pTail = nullptr;
        return;
    }
    // otherwise, list still has elements
    // store pointer to current tail
    Node* pOldHead = pTail;
    // move tail backwards
    pTail = pTail->pPrev;
    // set next pointer to be null
    pTail->pNext = nullptr;
    // delete former tail
    delete pOldHead;
}

void linked_list_add_value_before_position(Node*& pHead, Node*& pTail, const int POS, const int VALUE) {
    // if position is before the beginning, add to front
    if(POS <= 0 || pHead == nullptr)
        linked_list_add_value_to_front(pHead, pTail, VALUE);
    else 
    {
        Node* pCurrNode = pHead;
        int counter = 0;
        // advance through list counting number of jumps and continue until position found or at end
        while(pCurrNode != nullptr && counter < POS) 
        {
            pCurrNode = pCurrNode->pNext;
            counter++;
        }
        // if we have gone past the end
        if(pCurrNode == nullptr) 
            // add to back
            linked_list_add_value_to_back(pHead, pTail, VALUE);            
        else 
        {
            // otherwise add before target position
            Node* pNewNode = linked_list_make_node(VALUE);
            Node* prevNode = pCurrNode->pPrev;
            pNewNode->pPrev = prevNode;
            pNewNode->pNext = pCurrNode;
            pCurrNode->pPrev = pNewNode;
            prevNode->pNext = pNewNode;
        }
    }
}

int linked_list_min(const Node* const P_HEAD, const Node* const P_TAIL) {
    const Node* pCURR_NODE = P_HEAD;
    
    // if list is empty
    if(pCURR_NODE == nullptr) 
        // throw exception
        throw std::out_of_range("list is empty");

    // traverse list tracking min value present
    int minVal = pCURR_NODE->value;
    pCURR_NODE = pCURR_NODE->pNext;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value < minVal)
            minVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return minVal;
}

int linked_list_max(const Node* const P_HEAD, const Node* const P_TAIL) {
    const Node* pCURR_NODE = P_HEAD;
    
    // if list is empty
    if(pCURR_NODE == nullptr) 
        // throw exception
        throw std::out_of_range("list is empty");
    // traverse list tracking max value present
    int maxVal = pCURR_NODE->value;
    pCURR_NODE = pCURR_NODE->pNext;
    while(pCURR_NODE != nullptr)
    {
        if(pCURR_NODE->value > maxVal)
            maxVal = pCURR_NODE->value;
        pCURR_NODE = pCURR_NODE->pNext;
    }
    return maxVal;
}

int linked_list_find(const Node* const P_HEAD, const Node* const P_TAIL, const int TARGET) {
    const Node* pCURR_NODE = P_HEAD;
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

void linked_list_remove_node_at_position(Node*& pHead, Node*& pTail, const int POS) {
    // if list is empty
    if(pHead == nullptr)
        // do nothing
        return;
    // if position is before start of list
    else if(POS <= 0)
        // remove from front
        linked_list_remove_node_at_front(pHead, pTail);
    else 
    {
        Node* pCurrNode = pHead;
        int counter = 0;
        // advance through list counting number of jumps and continue until position found or at end
        while(counter < POS && pCurrNode != nullptr)
        {
            pCurrNode = pCurrNode->pNext;
            counter++;
        }
        // if we have gone to tail or further
        if(pCurrNode == nullptr || pCurrNode == pTail)
            // remove from back
            linked_list_remove_node_at_back(pHead, pTail);
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