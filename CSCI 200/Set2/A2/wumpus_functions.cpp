/* CSCI 200: Assignment 2: A2 - Watch the Wumpus
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     Looked up the proper way to store a variable in the header file to be used within the corresponding
 *     cpp file
 * 
 * The cpp file for all the functions implementations to be used in the Wumpus game
 * 
 */

#include "wumpus_functions.h"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int CAVE_WIDTH;
int CAVE_HEIGHT;

string enter_wumpus_file() 
{
    string fileName;
    cout << "Enter wumpus file to read from: ";
    cin >> fileName;
    return fileName;
}

string enter_game_log_file() 
{
    string fileName;
    cout << "Enter game log file to write to: ";
    cin >> fileName;
    return fileName;
}

bool open_files(ifstream& wumpusFileIn, ofstream& gameLogOut, const string& wumpusFileName, const string& gameLogFileName) 
{
    wumpusFileIn.open(wumpusFileName);
    gameLogOut.open(gameLogFileName);
    if (!wumpusFileIn.is_open() || !gameLogOut.is_open())
        return false;

    return true;
}

void read_world_header(ifstream& wumpusFileIn, int& caveWidth, int& caveHeight) 
{
    wumpusFileIn >> caveWidth >> caveHeight;
    CAVE_WIDTH = caveWidth;
    CAVE_HEIGHT = caveHeight;
}

bool read_object_header(ifstream& wumpusFileIn, int& wumpusX, int& wumpusY, int& heroX, int& heroY, int& pitX, int& pitY) 
{
    char w,h,p;

    wumpusFileIn >> w >> wumpusX >> wumpusY >> h >> heroX >> heroY >> p >> pitX >> pitY;

    if (wumpusX < 1 || wumpusX > CAVE_WIDTH || wumpusY < 1 || wumpusY > CAVE_HEIGHT ||
        heroX < 1 || heroX > CAVE_WIDTH || heroY < 1 || heroY > CAVE_HEIGHT ||
        pitX < 1 || pitX > CAVE_WIDTH || pitY < 1 || pitY > CAVE_HEIGHT) 
        return false; // Not all objects are in the cave

    // all objects are within the cave
    return true;
}

void read_number_moves(ifstream& wumpusFileIn, int& numMoves) 
{
    wumpusFileIn >> numMoves;
}

bool choose_player() 
{
 
    char playerChoice;
    bool validChoice = false;

    while (!validChoice) // Check if the user inputs a valid choice
    {
        cout << "Who do you wish to play as?\n  H - Hero\n  W - Wumpus" << endl;
        cout << "Character: ";
        cin >> playerChoice;
        if (playerChoice == 'H' || playerChoice == 'W') // Input correct choice set to true
            validChoice = true;
        else 
            cout << "Invalid choice, please select again." << endl;
    }

    if(playerChoice == 'H') // Check if player is the hero or wumpus
        return true;
    else
        return false;
}

void play_game(ifstream& wumpusFileIn, ofstream& gameLogOut, int caveWidth, int caveHeight, int numMoves, int wumpusX, int wumpusY, int heroX, int heroY, int pitX, int pitY, bool playAsHero) 
{
    /* If I had more time I would make smaller functions so that way I dont have to repeat all these 
        if statements for both the hero and wampus */
    
    int wumpus_curr_x = wumpusX;
    int wumpus_curr_y = wumpusY;
    int hero_curr_x = heroX;
    int hero_curr_y = heroY;
    bool outOfMoves = true;

    if(playAsHero) // Playing as the Hero
    {
        for(int i = 0; i < numMoves; i++) // Loop through the amount of moves
        {
            // Create variables for the who is moving and what direction
            char player;
            char direction;
            // Get the values from the file
            wumpusFileIn >> player;
            wumpusFileIn >> direction;

            // Check if the hero and the wumpus are at the same location
            if(hero_curr_y == wumpus_curr_y && hero_curr_x == wumpus_curr_x)
            {
                gameLogOut << "You have killed the Wumpus.  You win!" << endl;
                outOfMoves = false;
                break;
            }

            // Check if the hero fell into the pit
            if(hero_curr_y == pitY && hero_curr_x == pitX)
            {
                gameLogOut << "You have fallen in the pit.  You lose." << endl;
                outOfMoves = false;
                break;
            }

            // Check if the wumpus fell into the pit
            if(wumpus_curr_y == pitY && wumpus_curr_x == pitX)
            {
                gameLogOut << "The Wumpus has fallen in the pit.  You win!" << endl;
                outOfMoves = false;
                break;
            }

            else if(player == 'H') // Check if its the hero's move
            {
                if(direction == 'N')
                {
                    if(hero_curr_y < caveHeight) // Check if moving in the north direction and are not at the wall
                        hero_curr_y++;
                    else
                        gameLogOut << "You cannot move North." << endl;
                }
                else if(direction == 'S')
                {
                    if(hero_curr_y > 1) // Check if moving in the south direction and are not at the wall
                        hero_curr_y--;
                    else
                        gameLogOut << "You cannot move South." << endl;
                }
                else if(direction == 'E')
                {
                    if(hero_curr_x < caveWidth) // Check if moving in the east direction and are not at the wall
                        hero_curr_x++;
                    else
                        gameLogOut << "You cannot move East." << endl;
                }
                else if(direction == 'W')
                {
                    if(hero_curr_x > 1) // Check if moving in the west direction and are not at the wall
                        hero_curr_x--;
                    else
                        gameLogOut << "You cannot move West." << endl;
                }
                 // Output the movements to the log file
                gameLogOut << "You are at (" << hero_curr_x << ", " << hero_curr_y << ")." << endl;
            }
            
            else
            {
                if(direction == 'N') // Check if moving in the north direction and are not at the wall
                {
                    if(wumpus_curr_y < caveHeight)
                        wumpus_curr_y++;
                    else
                        gameLogOut << "The Wumpus cannot move North." << endl;
                }
                else if(direction == 'S')
                {
                    if(wumpus_curr_y > 1) // Check if moving in the south direction and are not at the wall
                        wumpus_curr_y--;
                    else
                        gameLogOut << "The Wumpus cannot move South." << endl;
                }
                else if(direction == 'E')
                {
                    if(wumpus_curr_x < caveWidth) // Check if moving in the east direction and are not at the wall
                        wumpus_curr_x++;
                    else
                        gameLogOut << "The Wumpus cannot move East." << endl;
                }
                else if(direction == 'W')
                {
                    if(wumpus_curr_x > 1) // Check if moving in the west direction and are not at the wall
                        wumpus_curr_x--;
                    else
                        gameLogOut << "The Wumpus cannot move West." << endl;
                }
                
                // Output the movements to the log file
                gameLogOut << "The Wumpus is at (" << wumpus_curr_x << ", " << wumpus_curr_y << ")." << endl;
            }
        }

        // Ran out of moves
        if(outOfMoves == true)
            gameLogOut << "The Wumpus has evaded you.  You lose." << endl;
    }
    else // Playing as the Wumpus
    {
        for(int i = 0; i < numMoves; i++) // Loop through the amount of moves
        {
            // Create variables for the who is moving and what direction
            char player;
            char direction;
            // Get the values from the file
            wumpusFileIn >> player;
            wumpusFileIn >> direction;

            // Check if the hero and the wumpus are at the same location
            if(hero_curr_y == wumpus_curr_y && hero_curr_x == wumpus_curr_x)
            {
                gameLogOut << "You have been killed by the Hero.  You lose." << endl;
                outOfMoves = false;
                break;
            }

            // Check if the wumpus fell into the pit
            if(wumpus_curr_y == pitY && wumpus_curr_x == pitX)
            {
                gameLogOut << "You have fallen in the pit.  You lose." << endl;
                outOfMoves = false;
                break;
            }

            // Check if the hero fell into the pit
            if(hero_curr_y == pitY && hero_curr_x == pitX)
            {
                gameLogOut << "The Hero has fallen in the pit.  You win!" << endl;
                outOfMoves = false;
                break;
            }

            else if(player == 'H') // Check if its the hero's move
            {
                if(direction == 'N')
                {
                    if(hero_curr_y < caveHeight) // Check if moving in the north direction and are not at the wall
                        hero_curr_y++;
                    else
                        gameLogOut << "The Hero cannot move North." << endl;
                }
                else if(direction == 'S')
                {
                    if(hero_curr_y > 1) // Check if moving in the south direction and are not at the wall
                        hero_curr_y--;
                    else
                        gameLogOut << "The Hero cannot move South." << endl;
                }
                else if(direction == 'E')
                {
                    if(hero_curr_x < caveWidth) // Check if moving in the east direction and are not at the wall
                        hero_curr_x++;
                    else
                        gameLogOut << "The Hero cannot move East." << endl;
                }
                else if(direction == 'W')
                {
                    if(hero_curr_x > 1) // Check if moving in the west direction and are not at the wall
                        hero_curr_x--;
                    else
                        gameLogOut << "The Hero cannot move West." << endl;
                }

                // Output the movements to the log file
                gameLogOut << "The Hero is at (" << hero_curr_x << ", " << hero_curr_y << ")." << endl;
            }
            
            else
            {
                if(direction == 'N')
                {
                    if(wumpus_curr_y < caveHeight) // Check if moving in the north direction and are not at the wall
                        wumpus_curr_y++;
                    else
                        gameLogOut << "You cannot move North." << endl;
                }
                else if(direction == 'S') 
                {
                    if(wumpus_curr_y > 1) // Check if moving in the south direction and are not at the wall
                        wumpus_curr_y--;
                    else
                        gameLogOut << "You cannot move South." << endl;
                }
                else if(direction == 'E')
                {
                    if(wumpus_curr_x < caveWidth) // Check if moving in the east direction and are not at the wall
                        wumpus_curr_x++;
                    else
                        gameLogOut << "You cannot move East." << endl;
                }
                else if(direction == 'W')
                {
                    if(wumpus_curr_x > 1) // Check if moving in the west direction and are not at the wall
                        wumpus_curr_x--;
                    else
                        gameLogOut << "You cannot move West." << endl;
                }

                // Output the movements to the log file
                gameLogOut << "You are at (" << wumpus_curr_x << ", " << wumpus_curr_y << ")." << endl;
            }
        }

        // Ran out of moves
        if(outOfMoves == true)
            gameLogOut << "You have evaded the Hero.  You win!" << endl;
    }

    // Close the files as the game is over
    gameLogOut.close();
    wumpusFileIn.close();
}