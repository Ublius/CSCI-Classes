/*
 * main.cpp
 *
 * Includes the main() function for the trie project.
 *
 * This code creates the executable run-main.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "trie.h"

using namespace std;

int main() {
    // You can use this main() to run your own analysis or testing code.
    cout << "If you are seeing this, you have successfully run main!" << endl;

// You can uncomment the code below to read in a large word list and insert
// them all into your trie, to set up your own tests.
/*
    ifstream fin("dictionary.txt");
    if (!fin) {
        cout << "Error opening dictionary.txt, exiting." << endl;
        return -1;
    }

    vector<string> words;
    while (fin) {
        string s;
        fin >> s;
        if (!s.empty()) words.push_back(s);
    }
    fin.close();

    trie t;
    for (string s: words) {
        t.insert(s);
    }
*/

    return 0;
}
