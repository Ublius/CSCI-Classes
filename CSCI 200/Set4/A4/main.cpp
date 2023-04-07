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
#include <vector>
#include <iomanip>

#include "Player.h"
#include "Game.h"

using namespace std;

int main() {
    // Initialize Random
    srand(time(0));

    // Get the number games to be played
    int numGames;
    cout << "How many games would you like to play? ";
    cin >> numGames;

    // Vector to store the number of Turns
    vector<int> turnCountVector;

    // Get the number players
    int numPlayers;
    cout << "How many players are playing: ";
    cin >> numPlayers;
    
    // For the number of games
    for(int i = 0; i < numGames; i++) 
    {
        // Create a Circular Doubly Linked List of Players
        LinkedList<Player> linkedList(numPlayers);
        for(int i = 1; i <= numPlayers; i++) 
        {
            // Give each player 3 chips to start
            Player player(i);
            linkedList.insert(i + 1, player);
        }

        // Player 1 will go first
        Node<Player>* pCurrNode = linkedList.getHead();
        Player currNode = pCurrNode->value;

        // The players with Chips
        int noZeroChips = numPlayers;
        // Total number of turns
        int numTurns = 0;
        // Number of Chips in the center
        int centerChips = 0;
        // Bool value for direction; True - clockwise, false - counter
        bool direction = true;

        // While there are at least two players with chips
        while(noZeroChips > 1) 
        {
            // Vector to store the number for each roll
            vector<int> numbers;
            // Bool value for if the player had zero chips to start
            bool zeroChips = false;
            // Determine how many dice the current player will roll
            int chipCount = currNode.getChipCount();
            // Player have more than three chips
            if (chipCount > 3)
                chipCount = 3;
            // Player now has zero chips
            else if (currNode.getChipCount() == 0)
                zeroChips = true;
            // Roll the dice for the number of chips
            for (int i = 0; i < chipCount; i++) 
            {
                int random = rollDie();
                numbers.push_back(random);
            }
            // Run a turn and check if the player now has zero chips
            bool hasZeroChips = playTurn(numbers, direction, pCurrNode, noZeroChips, centerChips, chipCount);
            // Decreate the number of players who dont have zero chips
            if(hasZeroChips && !zeroChips) 
                noZeroChips--;
            // Increase the number of players who have zero chips
            else if(!hasZeroChips && zeroChips) 
                noZeroChips++;

            currNode = pCurrNode->value;
            // Increase the number of turns
            numTurns++;
        }
        // Add the number of turns for that game to total number of turns
        turnCountVector.push_back(numTurns);
        cout << left << "Player #" << currNode.getPlayer() << " wins with " << currNode.getChipCount() << " chips left after " << numTurns << " turns" << endl;
    }
    int vectorSum = 0;
    for(std::vector<int>::size_type i = 0; i < turnCountVector.size(); i++)
    {
        vectorSum += turnCountVector[i];
    }
    // Get the Average number of turns
    double vectorAvg = (double)vectorSum / turnCountVector.size();
    cout << "The average number of turns for " << numGames << " games with " << numPlayers << " players is " << vectorAvg << " turns." << endl;
    return 0;
}