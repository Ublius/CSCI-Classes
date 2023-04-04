#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "LinkedList.hpp"
#include "Player.hpp"
#include "Die.hpp"


using namespace std;

class Game {
  public:
    /**
    * @brief The constructor for the Game class
    */
    Game(int userIn);
    /**
    * @brief The destructor for the Game class
    */
    ~Game();
    /**
    * @brief Getter for the current player index
    */
    int getCurrPlayer();
    /**
    * @brief Getter for the direction boolean
    */
    bool getDirection();
    /**
    * @brief end check member function to check if more then one player still has chips
    */
    bool endCheck();
    /**
    * @brief game turn member function where each turn happens and all of the logic is controlled
    */
    void gameTurn();
    /**
    * @brief player win function to determine the winning player and print it out
    */
    void playerWin();
  private:
    /**
    * @brief private data member for the currPlayers list
    */
    LinkedList<Player> currPlayers;
    /**
    * @brief private data member for the centerchips 
    */
    int mCenterChips;
    /**
    * @brief private data member for the gamesize
    */
    int mGameSize;
    /**
    * @brief private data member for the direction boolean
    */
    bool mDirection;
};

Game::Game(int userIn)
{
    // Constructs the game for us
    mDirection = 1;
    mGameSize = userIn;
    mCenterChips = 0;
    for(int i = 1; i < userIn + 1; i++)
        currPlayers.insert(i, Player(i));
}

Game::~Game()
{
    // Deconstructs the list because that is the only memory on the free store
    currPlayers.~LinkedList();
}


bool Game::getDirection()
{
    // returns the current direction
    return mDirection;
}

bool Game::endCheck(){
    // Checks to see if we have more then one player with chips
    int playerCheck = 0;
    for(int i = 0; i < mGameSize; i++)
    {
        if (currPlayers.get(i).getChips() > 0)
            playerCheck += 1;
    }
    if(playerCheck > 1)
        return 1;
    else
        return 0;
}

void Game::gameTurn(){
  // All of the logic for each game turn
  // Temporary vairables for each player needed possibly during the turn because I was having issues directly chaning chip values
    int userDie = 0;
    int playerSkip = 0;
    Die dice;
    // Determining how many dice to roll base on chip amount
    for(int i = 0; i < mGameSize; i++)
    {
        if(currPlayers.get(i).getChips() > 3)
            userDie = 3;
        else
            userDie = currPlayers.get(i).getChips();
        cout << "Player #" << currPlayers.get(i) << " has " << currPlayers.get(i).getChips() << " left" << endl;
        cout << "Rolling " << userDie << " dice" << endl;
        // Main logic for each roll
        for(int i = 0; i < userDie; i++)
        {
            int x = dice.roll();
            // Keep roll logic and output
            if(x <= 2)
                cout << "Rolled a " << x << " - keep!" << endl;
            // Give left logic 
            else if(x == 3)
            {
                currPlayers.get(i).giveChipTo(currPlayers.get(i+1));
                cout << "Rolled a " << x << " - give left - Player #"<<  currPlayers.get(i) << " has " <<  currPlayers.get(i).getChips() << " chips - Player #"<<  currPlayers.get(i+1) << " has "<<  currPlayers.get(i+1).getChips() << " chips" << endl;
            }
            else if(x == 4)
            {
                currPlayers.get(i).giveChipTo(currPlayers.get(i-1));
                cout << "Rolled a " << x << " - give left - Player #"<<  currPlayers.get(i) << " has " <<  currPlayers.get(i).getChips() << " chips - Player #"<<  currPlayers.get(i-1) << " has "<<  currPlayers.get(i+1).getChips() << " chips" << endl;
            }
        //     } else if(x == 5){
        //     // Give center logic
        //     Player1.giveChipToCenter(mCenterChips);
        //     currPlayers.set(mCurrPlayer-1, Player1);
        //     cout << "Rolled a " << x << " - give center - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " - Center has " << mCenterChips << " chips" << endl;
        //     } else if(x == 6){
        //     // Reverse logic
        //     cout << "Rolled a " << x << " - reverse!" << endl;
        //     if(mDirection == 1){
        //         mDirection = 0;
        //     } else{
        //         mDirection = 1;
        //     }
        //     } else{
        //     // Skip logic
        //         playerSkip += 1;
        //         cout << "Rolled a " << x << " - skip! skipping " << playerSkip << " players" << endl;
        //     }
        // }
        // End of turn logic to determine which player to move to next based on indice of current player
        }
    }
}


void Game::playerWin(){
    // player win function print out the winner of the game at the end
    for(int i = 0; i < mGameSize; i++){
        if ( currPlayers.get(i).getChips()>0){
        cout << "Player #" << currPlayers.get(i).getPlayer() << " wins with " << currPlayers.get(i).getChips() << " chips left after";
        break;
        }
    }
}

#endif