/* CSCI 200: Assignment 3: L3C - Array Test Suite
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        Used a lot of the CPP reference website, https://cplusplus.com/reference     
 *
 * implement each function such that all tests pass, update the comment with the Big O complexity of the function, make the function prototypes and definition
 */

#include "array_functions.h"

using namespace std;

void array_allocate(int*& pArray, int INIT_SIZE) {
    pArray = new int[INIT_SIZE](); // Create array in the heap
}

int array_get_element_at(const int* P_ARRAY, const int SIZE, const int POS)
{
    if(P_ARRAY == nullptr) // null check
        throw ("Array is null");
    if(POS < 0 || POS >= SIZE) // range check
        throw ("Position out of range");
    return P_ARRAY[POS]; // number at index position
}

void array_set_element_at(int* const P_array, int SIZE, int POS, int VALUE) 
{
    if(POS >= 0 && POS < SIZE) // Check the position is within bounds
        P_array[POS] = VALUE; // Set the position with the value
}

void array_deallocate(int*& pArray, int& size) 
{
    delete[] pArray; // Delete the array in the heap
    pArray = nullptr; // Set to null pointer
    size = 0; // size is 0
}

void array_insert_at_position(int*& pArray, int& size, int POS, int VALUE)
{
    if(POS <= 0) // range check
        POS = 0;
    else if(POS >= size) // range check
        POS = size;

    int* newArray = new int[size + 1]; // Create new array in the heap with size + 1
    for(int i = 0; i < POS; i++) // Loop through old array up to the position 
        newArray[i] = pArray[i]; // Add to new array
    newArray[POS] = VALUE; // Set pos to value
    for(int i = POS; i < size; i++) // Loop through old array up to the size
        newArray[i+1] = pArray[i]; // Add to new array
    delete[] pArray; // Delete old array in the heap
    pArray = newArray; // Set Array to new array in the heap
    size++; // Increase the size
}

int array_min(const int* const P_ARRAY, const int SIZE)
{
    if(P_ARRAY == nullptr) // null check
        throw ("Array is null");
    if(SIZE == 0) // size check
        throw ("Array size is zero");

    int min = P_ARRAY[0]; // set inital min to the first index
    for (int i = 1; i < SIZE; i++) // Loop through the values skipping the first
    {
        if(P_ARRAY[i] < min) // Check if value is les than min
            min = P_ARRAY[i]; // set min
    }
    return min; // return min
}

int array_max(const int* const P_ARRAY, const int SIZE) 
{
    if(P_ARRAY == nullptr) // null check
        throw ("Array is null");
    if(SIZE == 0) // size check
        throw ("Array size is zero");

    int max = P_ARRAY[0]; // set inital max to the first index
    for (int i = 1; i < SIZE; i++) // Loop through the values skipping the first
    {
        if(P_ARRAY[i] > max) // Check if value is greater than max
            max = P_ARRAY[i]; // set max
    }
    return max; // return max
}

int array_find(const int* P_ARRAY, int SIZE, int TARGET) 
{
    for (int i = 0; i < SIZE; i++)  // Loop through the array
    {
        if (P_ARRAY[i] == TARGET) // Check if the target
            return i; // return the index
    }
    
    return -1;
}

void array_remove_from_position(int*& pArray, int& size, int POS) 
{
    if(pArray == nullptr || size == 0) // null check
        return;
    if(POS <= 0) // range check
        POS = 0;
    else if(POS >= size) // range check
        POS = size - 1;

    for(int i = POS; i < size -1; i++)
        pArray[i] = pArray[i + 1]; // Loop through the array and subtract the position
    size--;
}

