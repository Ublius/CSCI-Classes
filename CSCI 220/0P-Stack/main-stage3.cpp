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
    return elapsed.count();
}

int main() {
    // You can use this main() to run your own analysis or initial testing code.
    cout << time_n_pushes(50000) << endl;
    cout << time_n_pushes(70000) << endl;
    cout << time_n_pushes(90000) << endl;
    cout << time_n_pushes(110000) << endl;
    cout << time_n_pushes(130000) << endl;
    cout << time_n_pushes(150000) << endl;
    cout << time_n_pushes(170000) << endl;
    cout << time_n_pushes(190000) << endl;
    cout << time_n_pushes(210000) << endl;
    cout << time_n_pushes(230000) << endl;
    cout << endl;
    cout << time_n_pushes(10000) << endl;
    cout << time_n_pushes(20000) << endl;
    cout << time_n_pushes(30000) << endl;
    cout << time_n_pushes(40000) << endl;
    cout << time_n_pushes(50000) << endl;
    cout << time_n_pushes(60000) << endl;
    cout << time_n_pushes(70000) << endl;
    cout << time_n_pushes(80000) << endl;
    cout << time_n_pushes(90000) << endl;
    cout << time_n_pushes(100000) << endl;
    cout << endl;
    cout << time_n_pushes(100000) << endl;
    cout << time_n_pushes(200000) << endl;
    cout << time_n_pushes(300000) << endl;
    cout << time_n_pushes(400000) << endl;
    cout << time_n_pushes(500000) << endl;
    cout << time_n_pushes(600000) << endl;
    cout << time_n_pushes(700000) << endl;
    cout << time_n_pushes(800000) << endl;
    cout << time_n_pushes(900000) << endl;
    cout << time_n_pushes(1000000) << endl;
    return 0;
}