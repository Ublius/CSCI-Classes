/*
 * hashtable.h
 *
 * A basic hashtable implementation.  This hashtable uses vectors,
 * rather than linked lists, to implement chaining.
 * 
 * Author: Tristen Erpelding
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
    {   
        numSize = 4;
        numKeys = 0;
        table.resize(numSize);
    };

    // basic hashset operations
    void insert(const T& key)
    {
        if(contains(key))
            return;

        // Append the key to the appropriate chain
        table[hash(key)%numSize].push_back(key);
        // Increment the number of keys in the hashtable
        numKeys++;

        // Check if the load factor is greater than 0.75
        if(load_factor() > 0.75)
        {
            // Resize the hashtable
            numSize *= 2;
            // Create a temporary hashtable
            vector<vector<T>> temp(numSize);
            // Iterate through the elements in the hashtable
            for(unsigned int i = 0; i < table.size(); i++)
            {
                // Iterate through the elements in the chain at the current index
                for(unsigned int j = 0; j < table[i].size(); j++)
                    // Append the current element to the appropriate chain in the temporary hashtable
                    temp[hash(table[i][j])%numSize].push_back(table[i][j]);
            }
            // Set the hashtable to the temporary hashtable
            table.swap(temp);
        }
    };

    void remove(const T& key)
    {
        if(!contains(key))
            return;

        // Iterate through the elements in the chain at the calculated index
        for(unsigned int i = 0; i < table[hash(key)%numSize].size(); i++)
        {
            // Check if the current element in the chain is equal to the key
            if(table[hash(key)%numSize][i] == key)
            {
                // Remove the key from the chain by erasing it from the vector
                table[hash(key)%numSize].erase(table[hash(key)%numSize].begin()+i);
                // Decrement the number of keys in the hashtable
                numKeys--;
            }
        }
    };

    bool contains(const T& key)
    {
        // Iterate through the elements in the chain at the calculated index
        for(unsigned int i = 0; i < table[hash(key)%numSize].size(); i++)
        {
            // Check if the current element in the chain is equal to the key
            if(table[hash(key)%numSize][i] == key)
                return true;
        }
        return false;
    };

    size_t size()
    {return numKeys;};

    // diagnostic functions
    double load_factor()
    {return (double)numKeys/(double)numSize;};

    // convenience method, invokes the "hash" template parameter
    // function object to get a hash code
    static size_t hash(const T &key) 
    {
        H h;
        return h(key);
    }

private:
    vector<vector<T>> table;
    size_t numSize;
    size_t numKeys;
 
};

#endif
