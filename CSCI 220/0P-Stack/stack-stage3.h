/*
 * stack-stage3.h
 *
 * Implements a simple stack class using dynamic arrays.
 * This may be implemented in 3 stages:
 *   Stage 1: non-template stack class storing strings,
 *            unsafe copies/assignments
 *   Stage 2: template stack class, unsafe copies/assignments
 *   Stage 3: template stack class, safe copies/assignments
 *
 * Note: no underflow detection is performed.  Performing pop() or top()
 * on an empty stack results in undefined behavior (possibly crashing your
 * program)!
 *
 * Author: Tristen Erpelding
 */

#ifndef _STACK_H
#define _STACK_H

#include <cstddef> // for size_t

template<typename T>
class stack {
  public:
    stack(); // Constructor

    /* --< Big 3 >-- */
    ~stack(); // Destructor
    stack(const stack<T>& other); // Copy constructor
    stack<T>& operator=(const stack<T>& other); // Assignment operator


    void push(const T &);
    void pop();

    T top();
    
    size_t size();
    bool is_empty();

  private:
    // helper functions
    void deepCopy(const stack<T>& other);

    T* _data;
    size_t _size;
    size_t _capacity;
};

template<typename T>
stack<T>::stack() 
{
    _size = 0;
    _capacity = 1;
    _data = new T[_capacity]; // Allocate memory for the stack
}

template<typename T>
stack<T>::~stack() 
{
    delete[] _data; // Deallocate memory
}

template<typename T>
stack<T>::stack(const stack<T>& other) 
{
    // Check for self-assignment
    if (this == &other)
        return;

    deepCopy(other); // Copy the data from the other stack
}

template<typename T>
stack<T>& stack<T>::operator=(const stack<T>& other)
{
    if(this == &other) // // Check for self-assignment
        return *this;

    deepCopy(other); // Copy the data from the other stack

    return *this;  // Return for chaining assignments
}

template<typename T>
void stack<T>::push(const T &s) 
{
    // If the stack is full,
    if (_size == _capacity) 
    {
        _capacity *= 2; // Double the capacity
        T* new_data = new T[_capacity]; // Create a new array with the doubled capacity

        for (size_t i = 0; i < _size; i++) // Copy elements from the old array to the new array
            new_data[i] = _data[i];

        delete[] _data; // Deallocate the old array
        _data = new_data; // Update the pointer to point to the new array
    }

    _data[_size++] = s; // Add the new element to the stack and increment the size
}

template<typename T>
void stack<T>::pop() 
{
    if(!is_empty())
        _size--; // Decrease the size to remove the top element
    
}

template<typename T>
T stack<T>::top() 
{
    if(!is_empty())
        return _data[_size - 1]; // Decrease the size to remove the top element
    return 0; // Return an empty string if the stack is empty
}

template<typename T>
size_t stack<T>::size() 
{
    return _size;
}

template<typename T>
bool stack<T>::is_empty() 
{
    return _size == 0;  // Return true if size is 0
}

template<typename T>
void stack<T>::deepCopy(const stack<T>& other)
{
    // Copy the data from the other stack
    _size = other._size;
    _capacity = other._capacity;

    _data = new T[_capacity]; // Allocate memory for the stack

    for (size_t i = 0; i < _size; i++) // Copy elements from the old array to the new array
        _data[i] = other._data[i];
}

#endif