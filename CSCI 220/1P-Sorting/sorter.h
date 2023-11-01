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
#include <queue>

using namespace std;


//Improved

// template <class T>
// void merge(vector<T>& arr, int left, int middle, int right) {
//     int n1 = middle - left + 1;
//     int n2 = right - middle;

//     vector<T> leftArr(n1);
//     vector<T> rightArr(n2);

//     for (int i = 0; i < n1; i++) {
//         leftArr[i] = arr[left + i];
//     }
//     for (int j = 0; j < n2; j++) {
//         rightArr[j] = arr[middle + 1 + j];
//     }

//     int i = 0, j = 0, k = left;

//     while (i < n1 && j < n2) {
//         if (leftArr[i] <= rightArr[j]) {
//             arr[k] = leftArr[i];
//             i++;
//         } else {
//             arr[k] = rightArr[j];
//             j++;
//         }
//         k++;
//     }

//     while (i < n1) {
//         arr[k] = leftArr[i];
//         i++;
//         k++;
//     }

//     while (j < n2) {
//         arr[k] = rightArr[j];
//         j++;
//         k++;
//     }
// }

//Improved

// template <class T>
// void mergeSort(vector<T>& arr, int left, int right) {
//     if (left < right) {
//         int middle = left + (right - left) / 2;

//         mergeSort(arr, left, middle);
//         mergeSort(arr, middle + 1, right);

//         merge(arr, left, middle, right);
//     }
// }


//OLD

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

    //Improved

    // int n = vec.size();

    // if (n <= 1) {
    //     return;
    // }

    // if (n <= k) {
    //     mergeSort(vec, 0, n - 1);
    // }
    // else {
    //     int partitionSize = n / k;
    //     int remainder = n % k;

    //     for (int i = 0; i < k; i++) {
    //         int partitionStart = i * partitionSize + min(i, remainder);
    //         int partitionEnd = partitionStart + partitionSize + (i < remainder ? 1 : 0);

    //         mergeSort(vec, partitionStart, partitionEnd - 1);
    //     }

    //     queue<vector<T>> partitions;

    //     for (int i = 0; i < k; i++) {
    //         partitions.push(vector<T>(vec.begin() + i * partitionSize + min(i, remainder),
    //                                          vec.begin() + i * partitionSize + min(i, remainder) +
    //                                          partitionSize + (i < remainder ? 1 : 0)));
    //     }

    //     while (partitions.size() > 1) {
    //         vector<T> partition1 = partitions.front();
    //         partitions.pop();
    //         vector<T> partition2 = partitions.front();
    //         partitions.pop();

    //         vector<T> mergedPartition;
    //         int i = 0, j = 0;
    //         while (i < partition1.size() && j < partition2.size()) {
    //             if (partition1[i] <= partition2[j]) {
    //                 mergedPartition.push_back(partition1[i]);
    //                 i++;
    //             } else {
    //                 mergedPartition.push_back(partition2[j]);
    //                 j++;
    //             }
    //         }

    //         while (i < partition1.size()) {
    //             mergedPartition.push_back(partition1[i]);
    //             i++;
    //         }

    //         while (j < partition2.size()) {
    //             mergedPartition.push_back(partition2[j]);
    //             j++;
    //         }

    //         partitions.push(mergedPartition);
    //     }

    //     vec = partitions.front();
    // }


    //OLD

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