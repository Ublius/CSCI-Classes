// https://en.cppreference.com/w/cpp/container/vector

#include <iostream>
#include <vector>
#include <string>
#include "multi_functions.h"

using namespace std;

// Create a structure for the x, y and value
struct Point 
{
    int x;
    int y;
    char value; 
};

vector<vector<char>> read_file(ifstream &inputFile, const int ARRAYROW, const int ARRAYCOL)
{
  // Create a Vector of type Vector of type char
  vector<vector<char>> nestedList;
  // Create a Vector of type char
  vector<char> innerList;
  char tempInt;
  // Loop through the amount of rows
  for(int i = 0; i < ARRAYROW; i++)
  {
    // Loop through the amount of columns
    for(int j = 0; j < ARRAYCOL; j++)
    {
      // Get each character
      inputFile >> tempInt;
      // Add to the Inner vector
      innerList.push_back(tempInt);
    }
    // Add the inner vector to the outer vector
    nestedList.push_back(innerList);
    // Clear the inner vector
    innerList.clear();
  }
  // Return the nested vector
  return nestedList;
}


bool BFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL)
{
  // Create a vector of the type Point from the structure defined
  vector<Point> bfsQueue;
  // Loop through the amount of rows
  for(int i = 0; i < ARRAYROW; i++)
  {
    // Loop through the amount of columns
    for(int j = 0; j < ARRAYCOL; j++)
    {
      // Find the starting position
      if(ogList.at(i).at(j) == 'S')
      {
        Point start;
        start.value = ogList.at(i).at(j);
        start.y = i;
        start.x = j;
        bfsQueue.push_back(start);
        break;
      }
    }
  }

  // While the vector isnt empty
  while(!bfsQueue.empty())
  {
    // Check if the front node of bfsQueue has value 'S' and erase
    if(bfsQueue.front().value == 'S')
      bfsQueue.erase(bfsQueue.begin());
    // Check if the front node of bfsQueue has value '@' and erase
    else if(bfsQueue.front().value == '@')
      bfsQueue.erase(bfsQueue.begin());

    // Check if the node below the front node is within the bounds of the array and is either a '.' or 'E'
    if(bfsQueue.front().y+1 < ARRAYROW && (ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == '.' || ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == 'E'))
    {
      // Create a new point 
      Point next;
      // If the node below the front node is 'E', return true 
      if(ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == 'E')
        return true;
      // Update the value of the node below the front node to '@'
      ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) = '@';
      // New point representing the node below the front node and push it to bfsQueue
      next.value = ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x); 
      next.y = bfsQueue.front().y+1; 
      next.x = bfsQueue.front().x; 
      bfsQueue.push_back(next);
    }
    // Check if the node to the right of the front node is within the bounds of the array and is either a '.' or 'E'
    if(bfsQueue.front().x+1 < ARRAYCOL && (ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == '.' || ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == 'E'))
    {
      // Create a new point 
      Point next; 
      // If the node to the right of the front node is 'E', return true
      if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == 'E')
        return true;
      // Update the value of the node to the right of the front node to '@'
      ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) = '@';
      // New point representing the node to the right of the front node and push it to bfsQueue
      next.value = ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1); 
      next.y = bfsQueue.front().y; 
      next.x = bfsQueue.front().x+1; 
      bfsQueue.push_back(next);
    }
    // Check if the node above the front node is within the bounds of the array and is either a '.' or 'E'
    if(bfsQueue.front().y-1 >= 0 && ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) != '#' && (ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == '.' || ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == 'E' ))
    {
      // Create a new point 
      Point next; 
      // If the node to the above of the front node is 'E', return true
      if(ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == 'E')
        return true;
       // Update the value of the node to the above of the front node to '@'
      ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) = '@';
      // New point representing the node to the above of the front node and push it to bfsQueue
      next.value = ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x); 
      next.y = bfsQueue.front().y-1; 
      next.x = bfsQueue.front().x; 
      bfsQueue.push_back(next);
    } 
    // Check if the node to the left the front node is within the bounds of the array and is either a '.' or 'E'
    if(bfsQueue.front().x-1 >= 0 && ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) != '#' && (ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == '.' || ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == 'E'))
    {
      // Create a new point 
      Point next; 
      // If the node to the left of the front node is 'E', return true
      if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == 'E')
        return true;
      // Update the value of the node to the left of the front node to '@'
      ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) = '@';
      // New point representing the node to the left of the front node and push it to bfsQueue
      next.value = ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1); 
      next.y = bfsQueue.front().y; 
      next.x = bfsQueue.front().x-1; 
      bfsQueue.push_back(next);
    }
  }
  return false;
}


bool DFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL)
{
  // Create a vector of the type Point from the structure defined
  vector<Point> dfsStack;
  // Loop through the amount of rows
  for(int i = 0; i < ARRAYROW; i++)
  {
    // Loop through the amount of columns
    for(int j = 0; j < ARRAYCOL; j++)
    {
      // Find the starting position
      if(ogList.at(i).at(j) == 'S'){
        Point start;
        start.value = ogList.at(i).at(j);
        start.y = i;
        start.x = j;
        dfsStack.push_back(start);
        break;
      }
    }
  }
  // While the last point in the stack is not the end point
  while(dfsStack.back().value != 'E')
  {
    // Get the y and x values of the last point in the stack
    int y = dfsStack.back().y;
    int x = dfsStack.back().x;
    
    // If the point below is a valid move
    if(y+1 < ARRAYROW && (ogList.at(y+1).at(x) == '.' || ogList.at(y+1).at(x) == 'E'))
    {
      // Create a new point
      Point next; 
      if(ogList.at(y+1).at(x) == 'E')
        return true;
      // Update the value of the point and add it to the stack
      ogList.at(y+1).at(x) = '@';
      next.value = ogList.at(y+1).at(x); 
      next.y = y+1; 
      next.x = x; 
      dfsStack.push_back(next);
    }
    // If the point to the right is a valid move
    else if(x+1 < ARRAYCOL && (ogList.at(y).at(x+1) == '.' || ogList.at(y).at(x+1) == 'E'))
    {
      // Create a new point
      Point next; 
      if(ogList.at(y).at(x+1) == 'E')
        return true;
      // Update the value of the point and add it to the stack
      ogList.at(y).at(x+1) = '@';
      next.value = ogList.at(y).at(x+1); 
      next.y = y; 
      next.x = x+1; 
      dfsStack.push_back(next);
    }
    // If the point above is a valid move
    else if(y-1 >= 0 && (ogList.at(y-1).at(x) == '.' || ogList.at(y-1).at(x) == 'E' ))
    {
      // Create a new point
      Point next; 
      if(ogList.at(y-1).at(x) == 'E')
        return true;
      // Update the value of the point and add it to the stack
      ogList.at(y-1).at(x) = '@';
      next.value = ogList.at(y-1).at(x); 
      next.y = y-1; 
      next.x = x; 
      dfsStack.push_back(next);
    }
    // If the point to the left is a valid move
    else if(x-1 >= 0 && (ogList.at(y).at(x-1) == '.' || ogList.at(y).at(x-1) == 'E'))
    {
      // Create a new point
      Point next; 
      if(ogList.at(y).at(x-1) == 'E')
        return true;
      // Update the value of the point and add it to the stack
      ogList.at(y).at(x-1) = '@';
      next.value = ogList.at(y).at(x-1); 
      next.y = y; 
      next.x = x-1; 
      dfsStack.push_back(next);
    }
    // If there are no valid moves, remove the last point from the stack
    else
      dfsStack.pop_back();
    // If the last point in the stack is the starting point, return false
    if(dfsStack.back().value == 'S')
      return false;

  }
  return false;
}