/*
 * main.cpp
 *
 * Includes the main() function for the hashtable project.
 *
 * This code is included in the default build target.  It
 * produces the program run-main.
*/

#include <ios>
#include <iostream>
#include <string>
#include <fstream>

#include "hashtable.h"
#include "hash_functions.h"
#include "random220.h"


using namespace std;

void experiment(int M)
{
    // Constants
    const int W = 172823;  // Total number of words in dictionary.txt
    const double p = static_cast<double>(2 * M) / W;  // Probability for word inclusion in experiment

    // Container to store words for testing
    vector<string> words;
    hashtable<string, hash4> table;


    // Sampling loop
    string str;
    ifstream inputfile("dictionary.txt");
    if (!inputfile.is_open()) {
        cerr << "Error opening dictionary.txt" << endl;
        return;
    }

    while (inputfile >> str) {
        // Should we use it for the experiment?
        if (bernoulli(p)) {
            words.push_back(str);  // Save the word for testing

            // Should it go into the table?
            if (bernoulli(0.5)) {
                table.insert(str);  // Insert the word into the hashtable
            }
        }
    }

    // Testing loop
    double tests = 0;
    double comparisons = 0;
    for (string str : words) {
        // Should we use it for testing?
        if (bernoulli(1000.0 / words.size())) {
            tests++;
            comparisons += table.countContains(str);
        }
    }

    // Calculate average number of comparisons per test
    double averageComparisons = comparisons / tests;

    // Output the result
    ofstream output;
    output.open("hash4.csv", std::ios_base::app);
    output << table.size() << "," << averageComparisons << "\n";
    output.close();
}
int main() {
    seed_now();

    const int W = 172823;
    const int num_experiments = 500;
    for (int i = 0; i < num_experiments; ++i) 
        experiment(equalikely(0, W / 2));

    return 0;
}

