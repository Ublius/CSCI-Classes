/* CSCI 200: Assignment 4: A4 - Wild Left-Center-Right Simulation
 *
 * Author: Tristen Erpelding
 * 
 * Simulates a game of the wild version of the dice game Left-Center-Right. Instead of
 * rolling a die, the program uses a random number generator ti create the same effect and
 * simulates the passing of chips around the circle in the same way the game does using a 
 * circularly linked list.
 */

#ifndef PLAYER_H
#define PLAYER_H

class Player {
// The Player will have two attributes that track its state:
public:
    /**
    * @brief constructor
    */
    Player();

    /**
    * @brief constructor with a given player name
    */
    Player(const int PLAYER_NUM);

    /**
    * @brief A function to return the player number (or name)
    * @return integer player number
    */
    int getPlayer();

    /**
    * @brief A function to return the number of chips the player currently has
    * @return integer number of chips a player has
    */
    int getChipCount();

    /**
    * @brief A function to give a single chip to another player. 
    * This function will decrement the callee's chips by one and increase the target's chips by one
    * @param PLAYER_NUM player that will be receiving the chip
    */
    void chipToPlayer(Player& PLAYER_NUM);
    
    /**
    * @brief A function to give a single chip to the center. 
    * This function will decrement the callee's chips by one
    */
    void chipToCenter();

private:
    /**
    * @brief The number associated to the player
    */
    int playerNum;
    /**
    * @brief The number of chips the play has
    */
    int numChips;
};

#endif