/* CSCI 200: Assignment 2: A2 - Watch the Wumpus
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     Looked up the proper way to store a variable in the header file to be used within the corresponding
 *     cpp file
 * 
 * The header file for all the functions to be used with in the Wumpus game
 * 
 */

#ifndef WUMPUS_FUNCTIONS_H
#define WUMPUS_FUNCTIONS_H

#include <fstream>
#include <string>

/**
 * @brief Prompts the user for the filename of the Wumpus game history (.wgh) file to be processed
 * @return String of the (relative (or absolute) path to the file)
*/
std::string enter_wumpus_file();

/**
 * @brief Prompts the user for the filename of the game log file to be output
 * @return String of the (relative (or absolute) path to the file)
*/
std::string enter_game_log_file();

/**
 * @brief Opens the input and output files, and returns false if either could not be opened
 * @param wumpusFileIn:  Input game file to get the values from
 * @param gameLogOut: Ouput log file to store the results of the game
 * @param wumpusFileName: String name for the input file (relative (or absolute) path to the file)
 * @param gameLogFileName: String name for the output file (relative (or absolute) path to the file)
 * @return Returns true if both files successfully opened, false otherwise
*/
bool open_files(std::ifstream& wumpusFileIn, std::ofstream& gameLogOut, const std::string& wumpusFileName, const std::string& gameLogFileName);

/**
 * @brief Reads the cave width and height from the input stream
 * @param wumpusFileIn: Input game file to get the values from
 * @param caveWidth: Width of the cave
 * @param caveHeight: Height of the cave
*/
void read_world_header(std::ifstream& wumpusFileIn, int& caveWidth, int& caveHeight);

/**
 * @brief Reads the initial placement of the hero, wumpus, and pit from the input stream
 * @param wumpusFileIn: Input game file to get the values from
 * @param wumpusX: X cord of the starting location of the wumpus
 * @param wumpusY: Y cord of the starting location of the wumpus
 * @param heroX: X cord of the starting location of the hero
 * @param heroY: Y cord of the starting location of the hero
 * @param pitX: X cord of the pit location
 * @param pitY: Y cord of the pit location
*/
bool read_object_header(std::ifstream& wumpusFileIn, int& wumpusX, int& wumpusY, int& heroX, int& heroY, int& pitX, int& pitY);

/**
 * @brief Reads the number of moves to be played from the input stream
 * @param wumpusFileIn: Input game file to get the values from
 * @param numMoves: Reference to store the number of moves that can be played
*/
void read_number_moves(std::ifstream& wumpusFileIn, int& numMoves);

/**
 * @brief Asks the user if they want to play as the hero or wumpus
 * @return Returns true if the user wants to play as the hero, false otherwise
*/
bool choose_player();

/**
 * @brief Simulates the game play as laid out in the input file and writes the game log to the output file.
 * @param wumpusFileIn: Input game file to get the values from
 * @param gameLogOut: Ouput log file to store the results of the game
 * @param caveWidth: Width of the cave
 * @param caveHeight: Height of the cave
 * @param numMoves: Number of moves allowed
 * @param wumpusX: X cord of the starting location of the wumpus
 * @param wumpusY: Y cord of the starting location of the wumpus
 * @param heroX: X cord of the starting location of the hero
 * @param heroY: Y cord of the starting location of the hero
 * @param pitX: X cord of the pit location
 * @param pitY: Y cord of the pit location
 * @param playAsHero: True if player chose to play as hero, false otherwise 
*/
void play_game(std::ifstream& wumpusFileIn, std::ofstream& gameLogOut, int caveWidth, int caveHeight, int numMoves, int wumpusX, int wumpusY, int heroX, int heroY, int pitX, int pitY, bool playAsHero);

extern int CAVE_WIDTH;
extern int CAVE_HEIGHT;

#endif //WUMPUS_FUNCTIONS_H