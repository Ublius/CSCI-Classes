/*
 * main-stage3.cpp
 *
 * Includes the main() function for the stack project (stages 2 & 3).
 *
 * This code is included in the build target "run-stage3-main", and
 * in the convenience targets "stage2", and "stage3".
 */

#include <iostream>
#include <chrono>

#include "stack-stage3.h"

using namespace std;

// template <typename T>
double time_n_pushes(unsigned n) {
    stack<unsigned> s;
    // get starting clock value
    auto start_time = chrono::system_clock::now();
    // do the n pushes
    for (unsigned i = 0; i < n; i++) {
        s.push(i);
    }
    // get ending clock value
    auto stop_time = chrono::system_clock::now();
    // compute elapsed time in seconds
    chrono::duration<double> elapsed = stop_time - start_time;
    return elapsed.count() / n;
}

int main() {
    // You can use this main() to run your own analysis or initial testing code.
    cout << "100000: " << time_n_pushes(100000) << endl;
    cout << "200000: " << time_n_pushes(200000) << endl;
    cout << "300000: " << time_n_pushes(300000) << endl;
    cout << "400000: " << time_n_pushes(400000) << endl;
    cout << "500000: " << time_n_pushes(500000) << endl;
    cout << "600000: " << time_n_pushes(600000) << endl;
    cout << "700000: " << time_n_pushes(700000) << endl;
    cout << "800000: " << time_n_pushes(800000) << endl;
    cout << "900000: " << time_n_pushes(900000) << endl;
    cout << "1000000: " << time_n_pushes(1000000) << endl;
    return 0;
}