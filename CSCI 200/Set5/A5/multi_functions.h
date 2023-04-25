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