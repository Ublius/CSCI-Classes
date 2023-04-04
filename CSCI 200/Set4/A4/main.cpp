#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Game.hpp"

using namespace std;

int main() {
    // seeding random for the die
    srand(time(0));
    rand();
    // Initializing user variables
    int userIn;
    int turnCount = 0;
    // Taking user input for how many players to make
    cout << "How many players are playing: ";
    cin >> userIn;
    // Calling the constructor for our game class
    Game mainGame(userIn);
    // Main loop for each turn and how I keep track of the turns
    while(mainGame.endCheck())
    {
        mainGame.gameTurn();
        turnCount += 1;
    }
    // Printing out the winning player
    mainGame.playerWin();
    // Printing out the turns
    cout << " " << turnCount << " turns" << endl;
    // Deconstructing and cleaning up
    mainGame.~Game();
    return 0;
}