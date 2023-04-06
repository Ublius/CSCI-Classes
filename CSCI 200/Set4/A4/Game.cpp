/* CSCI 200: Assignment 4: A4 - Wild Left-Center-Right Simulation
 *
 * Author: Tristen Erpelding
 * 
 * Simulates a game of the wild version of the dice game Left-Center-Right. Instead of
 * rolling a die, the program uses a random number generator ti create the same effect and
 * simulates the passing of chips around the circle in the same way the game does using a 
 * circularly linked list.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "Game.h"

using namespace std;

int rollDie() {
    int random = rand() % 8;
    return random;
}

void passLeft(Node<Player>*& pCurrNode, bool& zeroChips) {
    Player currNode = pCurrNode->value;
    Node<Player>* pPrevNode = pCurrNode->pPrev;
    Player prevNode = pPrevNode->value;
    if(prevNode.getChipCount() == 0)
        zeroChips = true;
    if(currNode.getChipCount() > 0) 
    {
        currNode.chipToPlayer(prevNode);
        pCurrNode->value = currNode;
        pPrevNode->value = prevNode;
    }
    cout << "Player #" << prevNode.getPlayer() << " has " << prevNode.getChipCount() << " chips - Player #" << currNode.getPlayer() << " has " << currNode.getChipCount() << " chips" << endl;
}

void passRight(Node<Player>*& pCurrNode, bool& zeroChips) {
    Player currNode = pCurrNode->value;
    Node<Player>* pNextNode = pCurrNode->pNext;
    Player nextNode = pNextNode->value;
    if(nextNode.getChipCount() == 0) 
        zeroChips = true;
    if(currNode.getChipCount() > 0) 
    {
        currNode.chipToPlayer(nextNode);
        pCurrNode->value = currNode;
        pNextNode->value = nextNode;
    }
    cout << "Player #" << nextNode.getPlayer() << " has " << nextNode.getChipCount() << " chips - Player #" << currNode.getPlayer() << " has " << currNode.getChipCount() << " chips" << endl;
}

void reverseDirection(bool& direction) 
{
    direction = !direction;
}

int skipPlayer(int count) {
    return count + 1;
}

void movePlayer(bool direction, Node<Player>*& pCurrNode) {
    if(direction == true)
        pCurrNode = pCurrNode->pNext;
    else 
        pCurrNode = pCurrNode->pPrev;
}

bool runTurn(const vector<int> diceActions, bool& direction, Node<Player>*& pCurrNode, int& noZeroChips, int &centerChips, int chipCount) {
    int count = 0;
    bool isZeroChips = false;
    bool zeroChips = false;
    Player currNode = pCurrNode->value;
    if (chipCount > 0) 
    {
        cout << left << "Player #" << currNode.getPlayer() << " has " << currNode.getChipCount() << " chips left" << endl;
        cout << right << setw(17) << "Rolling " << chipCount << " dice" << endl;
    }
    
    for (std::vector<int>::size_type i = 0; i < diceActions.size(); i++) 
    {
        if (diceActions[i] == 3) 
        {
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - give left - ";
            passLeft(pCurrNode, zeroChips);
        }
        else if (diceActions[i] == 4) 
        {
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - give right - ";
            passRight(pCurrNode, zeroChips);
        }
        else if (diceActions[i] == 5) 
        {
            Player currNode = pCurrNode->value;
            if (currNode.getChipCount() > 0) 
            {
                currNode.chipToCenter();
                pCurrNode->value = currNode;
                centerChips++;
            }
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - give center - Center has " << centerChips << " chips - Player #" << currNode.getPlayer() << " has " << currNode.getChipCount() << " chips" << endl;
        }
        else if (diceActions[i] == 7) 
        {
            count = skipPlayer(count);
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - skip! skipping " << count << " players" << endl;
        }
        else if (diceActions[i] == 6) 
        {
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - reverse!" << endl;
            reverseDirection(direction);
        }
        else 
        {
            cout << right << setw(18) << "Rolled a " << diceActions[i] << " - keep!" << endl;
        }
    }
    currNode = pCurrNode->value;
    chipCount = currNode.getChipCount();
    if (chipCount == 0) {
        isZeroChips = true;
    }
    for (int i = 0; i <= count; i++) {
        movePlayer(direction, pCurrNode);
    }
    if (zeroChips == true) {
        noZeroChips++;
    }
    return isZeroChips;
}