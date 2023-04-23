/* CSCI 200: Assignment 5: L5B - Recursion II: Merge Sort
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L4C
 * Implement the LinkedList Merge Sort algorithm
*/

#include <iostream>
#include "LinkedList.hpp"

using namespace std;

int main() {
    LinkedList<int> myList;
    myList.insert(myList.size(), 4);
    myList.insert(myList.size(), 3);
    myList.insert(myList.size(), 8);
    myList.insert(myList.size(), 1);
    myList.insert(myList.size(), 5);
    myList.insert(myList.size(), 9);
    myList.insert(myList.size(), 7);
    myList.insert(myList.size(), 2);
    myList.insert(myList.size(), 6);

    cout << "Before sorting: ";
    for (unsigned int i = 0; i < myList.size(); i++) 
    {
        cout << myList.get(i) << " ";
    }
    cout << endl;
    
    mergeSort(&(myList.mpHead));

    cout << "After sorting: ";
    for (unsigned int i = 0; i < myList.size(); i++) 
    {
        cout << myList.get(i) << " ";
    }

    return 0;
}