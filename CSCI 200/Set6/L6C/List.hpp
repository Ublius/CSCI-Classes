/* CSCI 200: Lab 6C - The Abstract List
 *
 * Author: Tristen Erpelding
 *
 * We will create an abstract templated List class that supports a subset of the list operations we have been working with.
 * Create the Array class which extends List using public inheritance.
 * Create the LinkedList class which extends List using public inheritance.
 * 
*/

#ifndef LIST_HPP
#define LIST_HPP

template<typename T>
class List {
public:
    // Constructor initializes the size to zero
    List() : mSize(0) {}
    // Virtual destructor
    virtual ~List() {}

    // Getter for the size of the list
    int getSize() const { return mSize; }
    // Pure virtual function to get the value at a given position
    virtual T get(const int POS) const = 0;
    // Pure virtual function to set the value at a given position
    virtual void set(const int POS, const T VALUE) = 0;
    // Pure virtual function to insert a value at a given position
    virtual void insert(const int POS, const T VALUE) = 0;
    // Pure virtual function to remove the value at a given position
    virtual void remove(const int POS) = 0;

protected:
    // The size of the list
    int mSize;
};

#endif
