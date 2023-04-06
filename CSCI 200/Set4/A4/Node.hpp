/* CSCI 200: Assignment 4: A4 - Wild Left-Center-Right Simulation
 *
 * Author: Tristen Erpelding
 * 
 * Simulates a game of the wild version of the dice game Left-Center-Right. Instead of
 * rolling a die, the program uses a random number generator ti create the same effect and
 * simulates the passing of chips around the circle in the same way the game does using a 
 * circularly linked list.
 */

template<typename T>
class Node {
public:
    Node(const T VALUE);

    T value;
    Node *pNext;
    Node *pPrev;
};

template<typename T>
Node<T>::Node(const T VALUE) 
{
    value = VALUE;
    pNext = nullptr;
    pPrev = nullptr;
}