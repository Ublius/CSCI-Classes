#ifndef WUMPUS_FUNCTIONS_H
#define WUMPUS_FUNCTIONS_H

#include <fstream>

using namespace std;

// Prompts the user for the filename of the Wumpus game history (.wgh) file to be processed
string enter_wumpus_file();

// Prompts the user for the filename of the game log file to be output
string enter_game_log_file();

// Opens the input and output files, and returns false if either could not be opened
bool open_files(ifstream& wumpusFileIn, ofstream& gameLogOut, const string& wumpusFileName, const string& gameLogFileName);

// Reads the cave width and height from the input stream
void read_world_header(ifstream& wumpusFileIn, int& caveWidth, int& caveHeight);

// Reads the initial placement of the hero, wumpus, and pit from the input stream
bool read_object_header(ifstream& wumpusFileIn, int& wumpusX, int& wumpusY, int& heroX, int& heroY, int& pitX, int& pitY);

// Reads the number of moves to be played from the input stream
void read_number_moves(ifstream& wumpusFileIn, int& numMoves);

// Asks the user if they want to play as the hero or wumpus, and returns true if the user wants to play as the hero
bool choose_player();

// Plays the game and writes the game log to the output stream
void play_game(ifstream& wumpusFileIn, ofstream& gameLogOut, int caveWidth, int caveHeight, int numMoves, int wumpusX, int wumpusY, int heroX, int heroY, int pitX, int pitY, bool playAsHero);

int CAVE_WIDTH = 0;
int CAVE_HEIGHT = 0;

#endif //WUMPUS_FUNCTIONS_H