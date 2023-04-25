/* CSCI 200: Assignment 5: A5 - a-MAZE-ing
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *     https://en.cppreference.com/w/cpp/container/vector  
 *
 * We will now combine the two-dimensional lists, stacks, and queues to implement a Maze solver using Breadth-First Search and Depth-First Search. 
 * The maze will be represented as a 2D List. 
 * The goal is to determine if it is possible to move from the Start Space to the End Space. 
 */

#ifndef MULTI_FUNCTIONS_H
#define MULTI_FUNCTIONS_H

#include <string>
#include <vector>
#include <fstream>

 /**
 * @brief Opens the file
 * @param inputFile inputfile passed by reference in order to use it and set it for the rest of the program
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * @return Nested Vector Aray with all the characters from the file
 */
std::vector<std::vector<char>> read_file(std::ifstream &inputFile, const int ARRAYROW, const int ARRAYCOL);

/**
 * @brief Sovles the program based on BFS
 * @param ogList maze 2D array to iterate over and solve
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * @return True if successfully completed
 */
bool BFS(std::vector<std::vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL);

/**
 * @brief Sovles the program based on DFS
 * @param ogList maze 2D array to iterate over and solve
 * @param ARRAYROW number of rows for the file
 * @param ARRAYCOL number of cols for the file
 * @return True if successfully completed
 */
bool DFS(std::vector<std::vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL);

#endif