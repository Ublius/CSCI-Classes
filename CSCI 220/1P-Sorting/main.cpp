/*
 * main.cpp
 *
 * Includes the main() function for the sorting project. 
 *
 * This code is included in the build target "run-main"
*/

#include <iostream>
#include <cstddef>
#include <vector>
#include <chrono>

#include "sorter.h"

using namespace std;

int main() {
    // You can use this main() to run your own analysis or testing code.
    // cout << "If you are seeing this, you have successfully run main!" << endl;
    srand(1234);

    // Time vs input size; at different k values

    vector<int> a;
    for (int j = 100000; j <= 500000; j+=100000){
        for (int i = 0; i < j; i++) {
            a.push_back(rand());
        }
        
        // get starting clock value  
        auto start_time = chrono::system_clock::now(); 
        
        sorter(a, 2);

        // get ending clock value 
        auto stop_time = chrono::system_clock::now(); 
        chrono::duration<double> elapsed = stop_time - start_time; 
        cout << elapsed.count() << endl; 
    }


    // Time vs k; at different input sizes

    // int input_size = 500000;
    // vector<int> a;
    // vector<int> k_values = {2, 3, 5, 10, 50, 100, 200};
    // for (int j : k_values)
    // {
    //     // cout << "k = " << j << endl;

    //     for (int i = 0; i < input_size; i++)
    //         a.push_back(rand());
        
    //     // get starting clock value  
    //     auto start_time = chrono::system_clock::now(); 
        
    //     sorter(a, j);

    //     // get ending clock value 
    //     auto stop_time = chrono::system_clock::now(); 
    //     chrono::duration<double> elapsed = stop_time - start_time; 
    //     cout << elapsed.count() << endl; 
    // }
    return 0;
}


