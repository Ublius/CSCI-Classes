/* CSCI 200: Lab 6C - The Abstract List
 *
 * Author: Tristen Erpelding
 *
 * We will create an abstract templated List class that supports a subset of the list operations we have been working with.
 * Create the Array class which extends List using public inheritance.
 * Create the LinkedList class which extends List using public inheritance.
 * 
*/

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include<iostream>
#include "List.hpp"

template<typename T>
class Array : public List<T> {
public:
    /**
     * @brief Deallocates an array, returning its memory back to the free store.
     * Upon function return, pArray will be set to be a null pointer and the array size will be set to zero.
    */
    ~Array() { delete[] mData; }
    /**
     * @brief Returns the value of the element at a target position. 
     * If the position it not within the array size, throws a standard exception
     * @param POS position to retrieve value from
     * @return element value at given position
     * @throws standard exception if array is null or position out of range
     * @note Runtime of function O(1)
    */
    T get(const int POS) const override;
    /**
     * @brief Sets the value of the element at a target position. 
     * If the position is out of range, does nothing.
     * @param POS position to set value at
     * @param VALUE value to place at target position
     * @note Runtime of function O(1)
    */
    void set(const int POS, const T VALUE) override;
    /**
     * @brief Inserts a value at a target position.  
     * If the position is out of range, inserts at the front/back as appropriate.  
     * Upon function return, the array will be increased in size by one.
     * The array size will be updated to the new size.
     * @param POS position to insert value at
     * @param VALUE value to insert at target position
     * @note Runtime of function O(n)
    */
    void insert(const int POS, const T VALUE) override;
    /**
     * @brief Removes a value at a target position.
     * If the position is out of range, removes from the front/back as appropriate.
     * Upon function return, the array will be decreased in size by one.
     * The array size will be updated to the new size.
     * @param POS position to remove value from
     * @note Runtime of function O(n)
    */
    void remove(const int POS) override;

private:
    T* mData;
};

template<typename T>
T Array<T>::get(const int POS) const 
{
    if(mData == nullptr) // null check
        throw ("Array is null");
    if(POS < 0 || POS >= this->mSize) // range check
        throw ("Position out of range");
    return mData[POS]; // number at index position
}

template<typename T>
void Array<T>::set(const int POS, const T VALUE) 
{
    if(POS >= 0 && POS < this->mSize) // Check the position is within bounds
        mData[POS] = VALUE; // Set the position with the val
}

template<typename T>
void Array<T>::insert(const int POS, const T VALUE) 
{
    int pos = POS;
    if(pos <= 0) // range check
        pos = 0;
    else if(pos >= this->mSize) // range check
        pos = this->mSize;

    int* newArray = new int[this->mSize + 1]; // Create new array in the heap with size + 1
    for(int i = 0; i < pos; i++) // Loop through old array up to the position 
        newArray[i] = mData[i]; // Add to new array
    newArray[pos] = VALUE; // Set pos to value
    for(int i = pos; i < this->mSize; i++) // Loop through old array up to the size
        newArray[i+1] = mData[i]; // Add to new array
    delete[] mData; // Delete old array in the heap
    mData = newArray; // Set Array to new array in the heap
    this->mSize++; // Increase the size
}

template<typename T>
void Array<T>::remove(const int POS) 
{
    int pos = POS;
    if(mData == nullptr || this->mSize == 0) // null check
        return;
    if(pos <= 0) // range check
        pos = 0;
    else if(pos >= this->mSize) // range check
        pos = this->mSize - 1;

    for(int i = pos; i < this->mSize -1; i++)
        mData[i] = mData[i + 1]; // Loop through the array and subtract the position
    this->mSize--;
}

#endif // ARRAY_HPP
