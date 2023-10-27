/*
 * hashtable.h
 *
 * A basic hashtable implementation.  This hashtable uses vectors,
 * rather than linked lists, to implement chaining.
 * 
 * Author: <your name here>
 */

#ifndef _HASHTABLE_PROJECT_H
#define _HASHTABLE_PROJECT_H

#include <functional>   // for std::hash
#include <vector>
#include <iostream>

using namespace std;

template <class T, class H = std::hash<T>>
class hashtable {
public:
    // constructor
    hashtable()
    {};

    // basic hashset operations
    void insert(const T& key)
    {};
    void remove(const T& key)
    {};
    bool contains(const T& key)
    {return false;};
    size_t size()
    {return 0;};

    // diagnostic functions
    double load_factor()
    {return 0.0;};

    // convenience method, invokes the "hash" template parameter
    // function object to get a hash code
    static size_t hash(const T &key) 
    {
        H h;
        return h(key);
    }

private:
    vector<vector<T>> m_table;
    size_t m_size;
    size_t m_n;
 
};

#endif
