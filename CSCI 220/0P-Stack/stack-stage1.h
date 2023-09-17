/*
 * stack-stage1.h
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
#include <string>  // for stage 1

using namespace std;

class stack {
  public:
    stack(); // Constructor
    ~stack(); // Destructor


    void push(const string &);
    void pop();

    string top();
    
    size_t size();
    bool is_empty();

  private:
    string* _data;
    size_t _size;
    size_t _capacity;
};

#endif
