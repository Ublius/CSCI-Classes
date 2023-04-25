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

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "multi_functions.h"
using namespace std;

int main(int argc, char* argv[]) 
{
  // get filename to open
  string filename;
  if(argc > 2) 
  {
    cout << "Only one argument taken" << endl;
    return -1;
  }
  else
  {
    filename = argv[1];
    cout << "Opening file '" << filename << "'" << endl;
  }
  // open file for parsing
  ifstream fileIn(filename);
  if(fileIn.fail()) 
  {
    cerr << "Could not open file \"" << filename << "\"" << endl;
    cerr << "Shutting down" << endl;
    return -1;
  }
  else
    cout << "File opened successfully!" << endl;

  cout << endl;
  // Asks the user for input and branches to either BFS or DFS
  cout << "Enter 1 to search via BFS." << endl;
  cout << "Enter 2 to search via DFS." << endl;
  // Collecting the array rows and cols
  int arrayRow;
  fileIn >> arrayRow;
  int arrayCol;
  fileIn >> arrayCol;
  // Creating the 2D array of the maze
  vector<vector<char>> nestedList = read_file(fileIn, arrayRow, arrayCol);
  int choice;
  cout << "Choice: ";
  cin >> choice;
  bool result;
  // Performing BFS or DFS on the maze
  if(choice == 1)
    result = BFS(nestedList, arrayRow, arrayCol);
  else
    result = DFS(nestedList, arrayRow, arrayCol);
  // Logic to return if the maze was found or not
  if(result == true)
    cout << "End was reached" << endl;
  else
    cout << "End cannot be reached" << endl;
  // Printing out the maze
  for(int i = 0; i < arrayRow; i++)
  {
    for(int j = 0; j < arrayCol; j++)
      cout << nestedList.at(i).at(j) << " ";
    cout << endl;
  }
}