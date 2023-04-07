/* CSCI 200: Assignment 4: A4 - Wild Left-Center-Right Simulation
 *
 * Author: Tristen Erpelding
 * 
 * Simulates a game of the wild version of the dice game Left-Center-Right. Instead of
 * rolling a die, the program uses a random number generator ti create the same effect and
 * simulates the passing of chips around the circle in the same way the game does using a 
 * circularly linked list.
 */

#include "Node.hpp"

template<typename T>
class LinkedList {
public:
    /**
    * @brief creates an empty list that has size equal to zero and sets the head & tail 
    * pointers to be null
    */
    LinkedList(int size);

    /**
    * @brief destroys the existing list by removing all nodes that are present in the list
    */
    ~LinkedList();

    /**
    * @brief returns the value of the element at given position. If position doesn't exist, 
    * throws a std::out_of_range exception
    * @param POS a constant integer
    */
    Node<T>* getHead() const;

    /**
    * @brief inserts a new node with the provided value BEFORE the provided position. 
    * That is, the newly created node will point to the current node at the provided position. 
    * If the provided position is out of range for the current list, then clamp the position to the head or tail of the list as appropriate
    * @param POS constant integer
    * @param VALUE constant integer
    */
    void insert(const int POS, const T VAL);

private:
    /**
    * @brief Pointer to the head node of the list
    */
    Node<T>* mpHead;

    /**
    * @brief Pointer to the tail node of the list
    */
    Node<T>* mpTail;

    /**
    * @brief Stores the current number of nodes in the list
    */
    int mSize;

    /**
    * @brief creates a node with provided value and null previous & next pointers
    * @param VALUE constant integer
    * @return new node with given value
    */
    Node<T>* mMakeNodeForValue(const T VALUE);
};

template<typename T>
LinkedList<T>::LinkedList(int size) 
{
    mpHead = nullptr;
    mpTail = nullptr;
    mSize = size;
}

template<typename T>
LinkedList<T>::~LinkedList() 
{
    int counter = 0;
    while(counter < mSize) 
    {
        mpHead = mpHead->pNext;
        if(mpHead == nullptr) 
        {
            mpTail = nullptr;
            break;
        }
        counter++;
    }
}

template<typename T>
Node<T>* LinkedList<T>::getHead() const
{
    return mpHead;
}

template<typename T>
void LinkedList<T>::insert(const int POS, const T VALUE) 
{
    if(POS <= 0 || mpHead == nullptr) 
    {
        Node<T>* pNewNode = mMakeNodeForValue(VALUE);
        if(mpHead == nullptr) 
        {
            mpHead = pNewNode;
            mpTail = pNewNode;
            // Make the Node circular
            pNewNode->pNext = pNewNode;
            pNewNode->pPrev = pNewNode;
        }                     
        else 
        {
            pNewNode->pNext = mpHead;
            mpHead->pPrev = pNewNode;
            mpHead = pNewNode;
            // Update tail and head's next pointer to make it circular
            mpTail->pNext = mpHead; 
            mpHead->pPrev = mpTail;
        }
    } 
    else 
    {
        Node<T>* pCurrNode = mpHead;
        int counter = 0;
        while(pCurrNode != nullptr && counter < POS) 
        {
            counter++;
            pCurrNode = pCurrNode->pNext;
        }
        if(pCurrNode == nullptr) {
            Node<T>* pNewNode = mMakeNodeForValue(VALUE);
            if(mpHead == nullptr) 
            {
                mpHead = pNewNode;
                mpTail = pNewNode;
                // Make the new node circular
                pNewNode->pNext = pNewNode; 
                pNewNode->pPrev = pNewNode;
            }
            else 
            {
                mpTail->pNext = pNewNode;
                pNewNode->pPrev = mpTail;
                mpTail = pNewNode;
                // Update tail's next pointer to make it circular
                mpTail->pNext = mpHead; 
                mpHead->pPrev = mpTail; 
            }
        } 
        else 
        {
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
Node<T>* LinkedList<T>::mMakeNodeForValue(const T VALUE) 
{
    Node<T>* pNode = new Node<T>(VALUE);
    return pNode;
}