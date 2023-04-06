/* CSCI 200: Assignment 4: A4 - Wild Left-Center-Right Simulation
 *
 * Author: Tristen Erpelding
 * 
 * Simulates a game of the wild version of the dice game Left-Center-Right. Instead of
 * rolling a die, the program uses a random number generator ti create the same effect and
 * simulates the passing of chips around the circle in the same way the game does using a 
 * circularly linked list.
 */

#include "Player.h"

Player::Player() 
{
    playerNum = 1;
    numChips = 3;
}

Player::Player(const int PLAYER_NUM) 
{
    playerNum = PLAYER_NUM;
    numChips = 3;
}

int Player::getPlayer() 
{
    return playerNum;
}

int Player::getChipCount() 
{
    return numChips;
}

void Player::chipToPlayer(Player &PLAYER_NUM) 
{
    numChips--;
    PLAYER_NUM.numChips++;
}

void Player::chipToCenter() 
{
    numChips--;
}