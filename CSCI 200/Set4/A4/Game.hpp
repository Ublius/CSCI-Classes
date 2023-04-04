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
    * @brief private data member for the current player index
    */
    int mCurrPlayer;
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
    mCurrPlayer = 1;
    mCenterChips = 0;
    for(int i = 1; i < userIn + 1; i++)
        currPlayers.insert(userIn, Player(i));
}

Game::~Game()
{
    // Deconstructs the list because that is the only memory on the free store
    currPlayers.~LinkedList();
}

int Game::getCurrPlayer()
{
    // Return the current player index
    return mCurrPlayer;
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
  Player Player1 = currPlayers.get(mCurrPlayer-1);
  Player Player2 = currPlayers.get(mGameSize-1);
  Player Player3 = currPlayers.get(mCurrPlayer-2);
  Player Player4 = currPlayers.get(mCurrPlayer);
  Player Player5 = currPlayers.get(0);
    int userDie = 0;
    int playerSkip = 0;
    Die dice;
    // Determining how many dice to roll base on chip amount
    if(Player1.getChips() > 3)
        userDie = 3;
    else
        userDie = Player1.getChips();
    cout << "Player #" << mCurrPlayer << " has " << Player1.getChips() << " left" << endl;
    cout << "Rolling " << userDie << " dice" << endl;
    // Main logic for each roll
    for(int i = 0; i < userDie; i++){
        int x = dice.roll();
        // Keep roll logic and output
        if(x == 0){
            cout << "Rolled a " << x << " - keep!" << endl;
        } else if(x == 1){
            cout << "Rolled a " << x << " - keep!" << endl;
        } else if(x == 2){
            cout << "Rolled a " << x << " - keep!" << endl;
        } else if(x == 3){
        // Give left logic 
        if(mCurrPlayer == 1){
            Player1.giveChipTo(Player2);
            currPlayers.set(mCurrPlayer-1, Player1);
            currPlayers.set(mGameSize-1, Player2);
            cout << "Rolled a " << x << " - give left - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " chips - Player #"<< mGameSize << " has "<< currPlayers.get(mGameSize-1).getChips() << " chips" << endl;
        }else{
            Player1.giveChipTo(Player3);
            currPlayers.set(mCurrPlayer-1, Player1);
            currPlayers.set(mCurrPlayer-2, Player3);
            cout << "Rolled a " << x << " - give left - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " chips - Player #"<< mCurrPlayer-1 << " has "<< currPlayers.get(mCurrPlayer-2).getChips() << " chips" << endl;
        }
        } else if(x == 4){
        // Give right logic
        if(mCurrPlayer == mGameSize){
            Player1.giveChipTo(Player5);
            currPlayers.set(mCurrPlayer-1, Player1);
            currPlayers.set(0, Player5);
            cout << "Rolled a " << x << " - give right - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " chips - Player #1" << " has "<< currPlayers.get(0).getChips() << " chips" << endl;
        }else{
            Player1.giveChipTo(Player4);
            currPlayers.set(mCurrPlayer-1, Player1);
            currPlayers.set(mCurrPlayer, Player4);
            cout << "Rolled a " << x << " - give right - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " chips - Player #"<< mCurrPlayer+1 << " has "<< currPlayers.get(mCurrPlayer).getChips() << " chips" << endl;
        }
        } else if(x == 5){
        // Give center logic
        Player1.giveChipToCenter(mCenterChips);
        currPlayers.set(mCurrPlayer-1, Player1);
        cout << "Rolled a " << x << " - give center - Player #"<< mCurrPlayer << " has " << currPlayers.get(mCurrPlayer-1).getChips() << " - Center has " << mCenterChips << " chips" << endl;
        } else if(x == 6){
        // Reverse logic
        cout << "Rolled a " << x << " - reverse!" << endl;
        if(mDirection == 1){
            mDirection = 0;
        } else{
            mDirection = 1;
        }
        } else{
        // Skip logic
            playerSkip += 1;
            cout << "Rolled a " << x << " - skip! skipping " << playerSkip << " players" << endl;
        }
    }
    // End of turn logic to determine which player to move to next based on indice of current player
    if(mDirection == 1){
        if(playerSkip == 0 && mCurrPlayer < mGameSize){
        mCurrPlayer += 1;
        } else if( playerSkip != 0 && mCurrPlayer + playerSkip <= mGameSize){
        mCurrPlayer += playerSkip;
        } else if ( playerSkip == 0 && mCurrPlayer == mGameSize){
        mCurrPlayer = 1;
        } else{
        mCurrPlayer += playerSkip;
        mCurrPlayer -= mGameSize;
        mCurrPlayer += 1;
        }
    } else{
        if(playerSkip == 0 && mCurrPlayer > 1 ){
        mCurrPlayer -= 1;
        } else if( playerSkip != 0 && mCurrPlayer - playerSkip >  0){
        mCurrPlayer -= playerSkip;
        } else if ( playerSkip == 0 && mCurrPlayer == 1){
        mCurrPlayer = mGameSize;
        } else{
        mCurrPlayer -= playerSkip;
        mCurrPlayer += mGameSize;
        mCurrPlayer -= 1;
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