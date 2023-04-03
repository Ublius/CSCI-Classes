/* CSCI 200: Assignment 4: L4C - A Templated Linked List Class
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        L4A, L4B
 * Generalize the Linked List created in Lab4B. 
 * Change your Node and LinkedList classes to be templated
 */

#include <iostream>
#include "LinkedList.hpp"

using namespace std;

int main()
{
    // Create a doubly linked list of integers
    LinkedList<int> myList;

    // Add values to the list
    myList.insert(0, 6);
    myList.insert(0, 5);
    myList.insert(5, 7);
    myList.insert(-3, 1);
    myList.insert(1, 2);
    myList.insert(2, 9);
    myList.insert(2, 3);

    // Print the list forwards
    cout << "List forwards: ";
    for (int i = 0; i < myList.size(); i++) {
        cout << myList.get(i) << " ";
    }
    cout << endl;

    // Set pos 3 to be 4
    myList.set(3, 4);

    // Print the list forwards
    cout << "List forwards: ";
    for (int i = 0; i < myList.size(); i++) {
        cout << myList.get(i) << " ";
    }
    cout << std::endl;

    // Remove elements from the list
    myList.remove(-2);
    myList.remove(9);
    myList.remove(2);

    // Get and print pos 2
    cout << "Value at pos 2: " << myList.get(2) << endl;

    // Print the size
    cout << "List size: " << myList.size() << endl;

    // Print the list forwards
    cout << "List forwards: ";
    for (int i = 0; i < myList.size(); i++) {
        cout << myList.get(i) << " ";
    }
    cout << endl;

    // Create a second list of integers using the default constructor
    LinkedList<int> myList2;

    // Print out the second list's size
    cout << "Second list size: " << myList2.size() << endl;

    // Assign the first list to the second list
    myList2 = myList;

    // Print out both their sizes
    cout << "First list size: " << myList.size() << endl;
    cout << "Second list size: " << myList2.size() << endl;

    // Add values to the original first list
    myList.insert(0, 1);
    myList.insert(0, 0);

    // Print out both of their sizes
    cout << "First list size: " << myList.size() << endl;
    cout << "Second list size: " << myList2.size() << endl;

    // Create a third list of integers using the copy constructor
    LinkedList<int> myList3(myList);

    // Print out all three sizes
    cout << "First list size: " << myList.size() << endl;
    cout << "Second list size: " << myList2.size() << endl;
    cout << "Third list size: " << myList3.size() << endl;

    // Add values to the original first list
    myList.insert(10, 8);
    myList.insert(10, 9);

    // Print out all three sizes
    cout << "First list size: " << myList.size() << endl;
    cout << "Second list size: " << myList2.size() << endl;
    cout << "Third list size: " << myList3.size() << endl;

    // TODO
    // delete all the lists
    
    LinkedList<string> myStringList;

    // Add values to the string list
    myStringList.insert(0, "Hello");
    myStringList.insert(1, "World");

    // Print the list forwards
    cout << "List forwards: ";
    for (int i = 0; i < myStringList.size(); i++) {
        cout << myStringList.get(i) << " ";
    }
    cout << endl;

    // TODO
    // Delete the list of strings

    return 0;
}