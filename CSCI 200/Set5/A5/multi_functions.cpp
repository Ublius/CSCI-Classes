// https://en.cppreference.com/w/cpp/container/vector

#include <iostream>
#include <vector>
#include <fstream> 
#include <string>
#include "multi_functions.h"

using namespace std;

string prompt_user_for_filename(){
  // Prompting the user for the file they want to open
  cout << "Please enter the name for your file here: ";
  string filename;
  getline(cin, filename);
  cout << "Opening file '" << filename << "'" << endl;
  return filename;
}
 
bool open_file(ifstream &inputFile, string filename){
  // Opens the input file for the program to read from 
  inputFile.open(filename);
  if (inputFile.fail()) {
    cerr << "Error opening input file";
    return false;
  }
  cout << "File opened successfully" << endl;
  return true;
}

vector<vector<char>> read_file(ifstream &inputFile, const int ARRAYROW, const int ARRAYCOL){
  vector<vector<char>> multiList;
  vector<char> tempList;
  char tempInt;
  for(int i = 0; i < ARRAYROW; i++){
    for(int j = 0; j < ARRAYCOL; j++){
        inputFile >> tempInt;
        tempList.push_back(tempInt);
    }
    multiList.push_back(tempList);
    tempList.clear();
  }
  return multiList;
}


int get_user_input() {
  int choice;
  // Allowing the user to choose what they want to do and validates their input
    while(true) {
      cout << "Choice: ";
      cin >> choice;
      // Checks user input here to ensure that a valid value is entered
      if (!cin.fail() && choice >= 1 && choice <= 2) break;
      cerr << "Invalid input please enter a number between 1 and 4" << endl;
      cin.clear();
      char badChar; 
      do { badChar = cin.get(); } while( badChar != '\n' );
    }
    return choice;
  
}

bool BFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL){
  struct Point {
    int x;
    int y;
    char value; 
  };
  vector<Point> bfsQueue;
  for(int i = 0; i < ARRAYROW; i++){
    for(int j = 0; j < ARRAYCOL; j++){
      if(ogList.at(i).at(j) == 'S'){
        Point start;
        start.value = ogList.at(i).at(j);
        start.y = i;
        start.x = j;
        bfsQueue.push_back(start);
        break;
      }
    }
  }
  while(!bfsQueue.empty()){
    if(bfsQueue.front().value == 'S'){bfsQueue.erase(bfsQueue.begin());}
    else if(bfsQueue.front().value == '@'){bfsQueue.erase(bfsQueue.begin());;}

    if(ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == '.' || ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == 'E'){
      Point next; 
      if(ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) == 'E'){return true;}
      ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x) = '@';
      next.value = ogList.at(bfsQueue.front().y+1).at(bfsQueue.front().x); 
      next.y = bfsQueue.front().y+1; 
      next.x = bfsQueue.front().x; 
      bfsQueue.push_back(next);
    } 
    if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == '.' || ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == 'E'){
      Point next; 
      if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) == 'E'){return true;}
      ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1) = '@';
      next.value = ogList.at(bfsQueue.front().y).at(bfsQueue.front().x+1); 
      next.y = bfsQueue.front().y; 
      next.x = bfsQueue.front().x+1; 
      bfsQueue.push_back(next);
    } 
    if(ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == '.' || ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == 'E' ){
      Point next; 
      if(ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) == 'E'){return true;}
      ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x) = '@';
      next.value = ogList.at(bfsQueue.front().y-1).at(bfsQueue.front().x); 
      next.y = bfsQueue.front().y-1; 
      next.x = bfsQueue.front().x; 
      bfsQueue.push_back(next);
    } 
    if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == '.' || ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == 'E'){
      Point next; 
      if(ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) == 'E'){return true;}
      ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1) = '@';
      next.value = ogList.at(bfsQueue.front().y).at(bfsQueue.front().x-1); 
      next.y = bfsQueue.front().y; 
      next.x = bfsQueue.front().x-1; 
      bfsQueue.push_back(next);
    }

  }
  return false;
}


bool DFS(vector<vector<char>> &ogList, const int ARRAYROW, const int ARRAYCOL){
  struct Point {
    int x;
    int y;
    char value; 
  };
  vector<Point> dfsStack;
  for(int i = 0; i < ARRAYROW; i++){
    for(int j = 0; j < ARRAYCOL; j++){
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
  
  while(dfsStack.back().value != 'E'){
    
    if(ogList.at(dfsStack.back().y+1).at(dfsStack.back().x) == '.' || ogList.at(dfsStack.back().y+1).at(dfsStack.back().x) == 'E'){
      Point next; 
      if(ogList.at(dfsStack.back().y+1).at(dfsStack.back().x) == 'E'){return true;}
      ogList.at(dfsStack.back().y+1).at(dfsStack.back().x) = '@';
      next.value = ogList.at(dfsStack.back().y+1).at(dfsStack.back().x); 
      next.y = dfsStack.back().y+1; 
      next.x = dfsStack.back().x; 
      dfsStack.push_back(next);
    } 
    else if(ogList.at(dfsStack.back().y).at(dfsStack.back().x+1) == '.' || ogList.at(dfsStack.back().y).at(dfsStack.back().x+1) == 'E'){
      Point next; 
      if(ogList.at(dfsStack.back().y).at(dfsStack.back().x+1) == 'E'){return true;}
      ogList.at(dfsStack.back().y).at(dfsStack.back().x+1) = '@';
      next.value = ogList.at(dfsStack.back().y).at(dfsStack.back().x+1); 
      next.y = dfsStack.back().y; 
      next.x = dfsStack.back().x+1; 
      dfsStack.push_back(next);
    } 
    else if(ogList.at(dfsStack.back().y-1).at(dfsStack.back().x) == '.' || ogList.at(dfsStack.back().y-1).at(dfsStack.back().x) == 'E' ){
      Point next; 
      if(ogList.at(dfsStack.back().y-1).at(dfsStack.back().x) == 'E'){return true;}
      ogList.at(dfsStack.back().y-1).at(dfsStack.back().x) = '@';
      next.value = ogList.at(dfsStack.back().y-1).at(dfsStack.back().x); 
      next.y = dfsStack.back().y-1; 
      next.x = dfsStack.back().x; 
      dfsStack.push_back(next);
    } 
    else if(ogList.at(dfsStack.back().y).at(dfsStack.back().x-1) == '.' || ogList.at(dfsStack.back().y).at(dfsStack.back().x-1) == 'E'){
      Point next; 
      if(ogList.at(dfsStack.back().y).at(dfsStack.back().x-1) == 'E'){return true;}
      ogList.at(dfsStack.back().y).at(dfsStack.back().x-1) = '@';
      next.value = ogList.at(dfsStack.back().y).at(dfsStack.back().x-1); 
      next.y = dfsStack.back().y; 
      next.x = dfsStack.back().x-1; 
      dfsStack.push_back(next);
    } else{
      dfsStack.pop_back();
    }

    if(dfsStack.back().value == 'S'){return false;}

  }
  return false;

}