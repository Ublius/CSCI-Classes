/* CSCI 200: Assignment 5: L5C - A Needle in the Haystack
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L5B
 * To get started, prompt the user to enter the number of integers to store in a list. 
 * Then generate n random integers. Finally, sort the list into ascending order.
 * Now we'll have the user give us a target value to search for. 
 * Implement the binary search algorithm
*/

#include <iostream>
#include <ctime>
#include "LinkedList.hpp"

using namespace std;

int main() {
    int n, min, max;
    LinkedList<int> list;

    // Seed the random number generator
    srand(time(NULL));

    // Get user input
    cout << "Enter the number of integers to generate: ";
    cin >> n;
    cout << "Enter the smallest value to generate: ";
    cin >> min;
    cout << "Enter the largest value to generate: ";
    cin >> max;

    // Generate and add random values to list
    for (int i = 0; i < 10 * n; i++) 
    {
        int val = rand() % (max - min + 1) + min;
        list.insert(list.size(), val);
    }

    // Print the list forwards
    cout << "List forwards:" << endl;
    for (unsigned int i = 0; i < list.size(); i++) 
        cout << list.get(i) << " ";
    cout << endl;

    // Sort the list ascending
    mergeSort(&(list.mpHead));

    // Print the list forwards
    cout << "List forwards (sorted):" << endl;
    for (unsigned int i = 0; i < list.size(); i++) 
        cout << list.get(i) << " ";
    cout << endl;

    // Get user input for target values
    int numTargets;
    cout << "Enter the number of target values to search for: ";
    cin >> numTargets;

    for (int i = 0; i < numTargets; i++) 
    {
        int target;
        cout << "Enter target value " << i + 1 << ": ";
        cin >> target;

        // Perform binary search and print result
        int pos = binarySearch(list, target);
        if (pos == -1)
            cout << "Target value not found: -1." << endl;
        else 
            cout << "Target value " << target << " found at position " << pos << "." << endl;
    }

    return 0;
}