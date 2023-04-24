#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "multi_functions.h"
using namespace std;

int main(int argc, char* argv[]) {
    // get filename to open
    string filename;
    if(argc == 1) {
        filename = prompt_user_for_filename();
    } else if(argc == 2) {
        filename = argv[1];
        cout << "Opening file '" << filename << "'" << endl;
    } else {
        cerr << "Usage: " << argv[0] << " [filename]" << endl;
        cerr << "  filename   - optional file to open upon start" << endl;
        return -2;
    }
    // open file for parsing
    ifstream fileIn;
    if( !open_file(fileIn, filename) ) {
        cerr << "Could not open file \"" << filename << "\"" << endl;
        cerr << "Shutting down" << endl;
        return -1;
    }
    // Asks the user for input and branches to either BFS or DFS
    cout << "Enter 1 to search via BFS." << endl;
    cout << "Enter 2 to search via DFS." << endl;
    // Collecting the array rows and cols
    int arrayRow;
    fileIn >> arrayRow;
    int arrayCol;
    fileIn >> arrayCol;
    // Creating the 2D array of the maze
    vector<vector<char>> multiList = read_file(fileIn, arrayRow, arrayCol);
    int choice = get_user_input();
    bool result;
    // Performing BFS or DFS on the maze
    if(choice == 1){result = BFS(multiList, arrayRow, arrayCol);}
    else{result = DFS(multiList, arrayRow, arrayCol);}
    // Logic to return if the maze was found or not
    if(result == true){
      cout << "End was reached" << endl;
    } else{
      cout << "End cannot be reached" << endl;
    }
    // Printing out the maze
    for(int i = 0; i < arrayRow; i++){
      for(int j = 0; j < arrayCol; j++){
        cout << multiList.at(i).at(j) << " ";
      }
      cout << endl;
    }
}