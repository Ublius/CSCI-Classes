#ifndef MULTI_FUNCTIONS_H
#define MULTI_FUNCTIONS_H

#include <string>
#include <fstream>
#include <vector>
using namespace std;

/**
 * @brief Prompts the user to enter the filename and turns it into a string 
 * 
 */
string prompt_user_for_filename();
 /**
 * @brief Opens the file
 * 
 * @param inputFile inputfile passed by reference in order to use it and set it for the rest of the program
 * @param filename name of the file to open 
 * 
 */
bool open_file(ifstream &inputFile, string filename);

 /**
 * @brief Opens the file
 * 
 * @param inputFile inputfile passed by reference in order to use it and set it for the rest of the program
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * 
 */
vector<vector<char>> read_file(ifstream &inputFile, const int ARRAYROW, const int ARRAYCOL);

 /**
 * @brief Gets user input in order to branch down either bfs or dfs
 * 
 */
int get_user_input();

/**
 * @brief Sovles the program based on BFS
 * 
 * @param ogList maze 2D array to iterate over and solve
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * 
 */
bool BFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL);

/**
 * @brief Sovles the program based on DFS
 * 
 * @param ogList maze 2D array to iterate over and solve
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * 
 */
bool DFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL);

#endif