/*
 * sorter.h
 *
 * Declares a template function:
 *    
 *   sorter() : k-way merge sort.
 * 
 * 
 * You may add other functions if you wish, but this template function
 * is the only one that need to be exposed for the testing code.
 * 
 * Authors: C. Painter-Wakefield & Tolga Can
 */

#ifndef _SORTER_H
#define _SORTER_H

#include <string>
#include <cstddef>
#include <vector>

using namespace std;

template <class T>
vector<T> kMerge(vector<vector<T>>& partitions)
{
    int k = partitions.size();
    vector<int> indices(k, 0);
    vector<T> result;

    if (partitions.empty())
        return result;

    while (true) 
	{
        T minElement; // Initialize minElement within the loop
        int minPartition = -1;

        // Find the minimum element among the partitions
        for (int i = 0; i < k; i++) 
		{
			// Check if the current partition has remaining elements to compare
            if (indices[i] < partitions[i].size()) 
			{
				/// If minPartition is -1 (no minimum found yet) or the current element in the partition is smaller than the current minimum
                if (minPartition == -1 || partitions[i][indices[i]] < minElement) 
				{
					// Update minElement to current element
                    minElement = partitions[i][indices[i]];
					// Update minPartition to current partition
                    minPartition = i;
                }
            }
        }

        // If no minimum element is found, all partitions are empty, so we can break the loop
        if (minPartition == -1)
            break;

        // Add the minimum element to the result vector and update the index for the partition
        result.push_back(minElement);
        indices[minPartition]++;
    }

    return result;
}

template <class T>
void sorter(vector<T> &vec, int k)
{
    int n = vec.size();
    if (n <= 1)
        return;

	// Create k new vectors to store partitions
    vector<vector<T>> partitions(k);

	// Distribute elements from the input vector to partitions in a round-robin manner
    for (int i = 0; i < n; i++)
        partitions[i % k].push_back(vec[i]);

	// Recursively sort each partition
    for (int i = 0; i < k; i++)
        sorter(partitions[i], k);
	
	// Merge the sorted partitions back into the original vector
    vec = kMerge(partitions);
}

#endif
