#include "wumpus_functions.h"

#include <iostream>
#include <fstream>
#include <string>


using namespace std;

// Prompts the user for the filename of the Wumpus game history (.wgh) file to be processed
string enter_wumpus_file() {
    string fileName;
    cout << "Enter the filename of the Wumpus game history (.wgh) file to be processed: ";
    // cin >> fileName;
    fileName = "input/heroEvade.wgh";
    return fileName;
}

// Prompts the user for the filename of the game log file to be output
string enter_game_log_file() {
    string fileName;
    cout << "Enter the filename of the game log file to be output: ";
    // cin >> fileName;
    fileName = "test.log";
    return fileName;
}

// Opens the input and output files, and returns false if either could not be opened
bool open_files(ifstream& wumpusFileIn, ofstream& gameLogOut, const string& wumpusFileName, const string& gameLogFileName) {
    wumpusFileIn.open(wumpusFileName);
    gameLogOut.open(gameLogFileName);
    if (!wumpusFileIn.is_open() || !gameLogOut.is_open()) {
        return false;
    }
    return true;
}

// Reads the cave width and height from the input stream
void read_world_header(ifstream& wumpusFileIn, int& caveWidth, int& caveHeight) {
    wumpusFileIn >> caveWidth >> caveHeight;
    CAVE_WIDTH = caveWidth;
    CAVE_HEIGHT = caveHeight;
}

// Reads the initial placement of the hero, wumpus, and pit from the input stream
bool read_object_header(ifstream& wumpusFileIn, int& wumpusX, int& wumpusY, int& heroX, int& heroY, int& pitX, int& pitY) {
    char w,h,p;

    wumpusFileIn >> w >> wumpusX >> wumpusY >> h >> heroX >> heroY >> p >> pitX >> pitY;

    if (wumpusX < 1 || wumpusX > CAVE_WIDTH || wumpusY < 1 || wumpusY > CAVE_HEIGHT ||
        heroX < 1 || heroX > CAVE_WIDTH || heroY < 1 || heroY > CAVE_HEIGHT ||
        pitX < 1 || pitX > CAVE_WIDTH || pitY < 1 || pitY > CAVE_HEIGHT) {
        return false;
    }

    // all objects are within the cave
    return true;
}

// Reads the number of moves to be played from the input stream
void read_number_moves(ifstream& wumpusFileIn, int& numMoves) {
    wumpusFileIn >> numMoves;
}

// Asks the user if they want to play as the hero or wumpus, and returns true if the user wants to play as the hero
bool choose_player() {
    char playerChoice;
    bool validChoice = false;

    while (!validChoice) {
        cout << "Who do you wish to play as?\n  H - Hero\n  W - Wumpus" << endl;
        cout << "Character: ";
        cin >> playerChoice;
        if (playerChoice == 'H' || playerChoice == 'W') 
            validChoice = true;
        else 
            cout << "Invalid choice, please select again." << endl;
    }

    return playerChoice;
}

// Plays the game and writes the game log to the output stream
void play_game(ifstream& wumpusFileIn, ofstream& gameLogOut, int caveWidth, int caveHeight, int numMoves, int wumpusX, int wumpusY, int heroX, int heroY, int pitX, int pitY, bool playAsHero) {
    int wumpus_curr_x = wumpusX, wumpus_curr_y = wumpusY;
    int hero_curr_x = heroX, hero_curr_y = heroY;
    string move;

    for (int i = 0; i < numMoves; i++) {
        // read the move from the input file
        wumpusFileIn >> move;

        // update the position of the player that made the move
        if (playAsHero) {
            if (move[0] == 'H') {
                if (move[1] == 'N' && hero_curr_y < caveHeight) {
                    hero_curr_y++;
                } else if (move[1] == 'S' && hero_curr_y > 1) {
                    hero_curr_y--;
                } else if (move[1] == 'E' && hero_curr_x < caveWidth) {
                    hero_curr_x++;
                } else if (move[1] == 'W' && hero_curr_x > 1) {
                    hero_curr_x--;
                }
            } else {
                // the wumpus made a move
                if (move[1] == 'N' && wumpus_curr_y < caveHeight) {
                    wumpus_curr_y++;
                } else if (move[1] == 'S' && wumpus_curr_y > 1) {
                    wumpus_curr_y--;
                } else if (move[1] == 'E' && wumpus_curr_x < caveWidth) {
                    wumpus_curr_x++;
                } else if (move[1] == 'W' && wumpus_curr_x > 1) {
                    wumpus_curr_x--;
                }
            }
        } else {
            // the player is the wumpus
            if (move[0] == 'W') {
                if (move[1] == 'N' && wumpus_curr_y < caveHeight) {
                    wumpus_curr_y++;
                } else if (move[1] == 'S' && wumpus_curr_y > 1) {
                    wumpus_curr_y--;
                } else if (move[1] == 'E' && wumpus_curr_x < caveWidth) {
                    wumpus_curr_x++;
                } else if (move[1] == 'W' && wumpus_curr_x > 1) {
                    wumpus_curr_x--;
                }
            } else {
                // the hero made a move
                if (move[1] == 'N' && hero_curr_y < caveHeight) {
                    hero_curr_y++;
                } else if (move[1] == 'S' && hero_curr_y > 1) {
                    hero_curr_y--;
                } else if (move[1] == 'E' && hero_curr_x < caveWidth) {
                    hero_curr_x++;
                } else if (move[1] == 'W' && hero_curr_x > 1) {
                    hero_curr_x--;
                }
            }
        }

        // write the move to the output file
        gameLogOut << "The Wumpus is at (" << wumpus_curr_x << ", " << wumpus_curr_y << ")." << endl;
    }
}