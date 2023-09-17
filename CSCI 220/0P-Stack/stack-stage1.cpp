/*
 * stack-stage1.cpp
 *
 * Method definitions for the stack implementation (stage 1).
 *
 * Author: Tristen Erpelding
 */

#include "stack-stage1.h"
#include <string>

stack::stack() 
{
    _size = 0;
    _capacity = 1;
    _data = new string[_capacity]; // Allocate memory for the stack
}

stack::~stack() 
{
    delete[] _data; // Deallocate memory
}

void stack::push(const string &s) 
{
    // If the stack is full,
    if (_size == _capacity) 
    {
        _capacity *= 2; // Double the capacity
        string* new_data = new string[_capacity]; // Create a new array with the doubled capacity

        for (size_t i = 0; i < _size; i++) // Copy elements from the old array to the new array
            new_data[i] = _data[i];

        delete[] _data; // Deallocate the old array
        _data = new_data; // Update the pointer to point to the new array
    }

    _data[_size++] = s; // Add the new element to the stack and increment the size
}

void stack::pop() 
{
    if(!is_empty())
        _size--; // Decrease the size to remove the top element
    
}

string stack::top() 
{
    if(!is_empty())
        return _data[_size - 1]; // Decrease the size to remove the top element
    return ""; // Return an empty string if the stack is empty
}

size_t stack::size() 
{
    return _size;
}

bool stack::is_empty() 
{
    return _size == 0;  // Return true if size is 0
}