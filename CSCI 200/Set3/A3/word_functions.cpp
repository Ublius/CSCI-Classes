/* CSCI 200: Assignment 3: A3 - Green Eggs and Ham
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        Used a lot of the CPP reference website, https://cplusplus.com/reference    
 *
 *  This program reads in a text file and parses the contents to count word data.
 */


#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "word_functions.h"

using namespace std;

bool open_file(ifstream& fileIn, const string& filename)
{
    fileIn.open(filename); // Open the file
    if(!fileIn.is_open())
        return false; // false if file could not be opened
    return true;
}

vector<string> read_words_from_file(ifstream& fileIn)
{
    vector<string> words;
    string word;
    while(fileIn >> word) // While their is words
        words.push_back(word); // add to the vector of words
    return words; // return words
}

void remove_punctuation(vector<string>& allWords, const string& PUNCTUATION_TO_REMOVE)
{
    for (int i = 0; i < allWords.size(); i++) // Iterate over each word in the vector
    { 
        string word = allWords[i]; // get each individual word
        for (int j = 0; j < PUNCTUATION_TO_REMOVE.length(); j++) // Iterate over each punc character
        {
            char punc = PUNCTUATION_TO_REMOVE[j]; // set the punc character to a character
            int pos = word.find(punc); // find the pos of the punc in the word
            while (pos != string::npos) // while their are still characters in the word
            {
                word.erase(pos, 1); // delete the punc
                pos = word.find(punc, pos); // check if there is more punc
            }
        }
        allWords[i] = word; // set the word to the new word
    }
}

void capitalize_words(vector<string>& words)
{
    for(int i = 0; i < words.size(); i++) // Iterate over each word in the vector
    {
        string word = words[i];
        for(int j = 0; j < word.size(); j++) // Iterate over each character in the word
            word[j] = toupper(word[j]); // Convert each character to upper case
        words[i] = word;
    }
}

vector<string> filter_unique_words(const vector<string>& words)
{
    vector<string> unique;
    for(int i = 0; i < words.size(); i++) // Iterate over each word in the vector
    {
        bool bIsDup = false;
        for(int j = 0; j < i; j++)
        {
            if(words[i] == words[j]) // Check if two words match
            {
                bIsDup = true;
                break;
            }
        }
        if(!bIsDup)
            unique.push_back(words[i]); // Only add words that dont match
    }
    for (int i = 0; i < unique.size() - 1; i++) // Iterate over the unique words
    {
        int minIndex = i;
        for (int j = i + 1; j < unique.size(); j++) 
        {
            if (unique[j] < unique[minIndex]) // Check if the word is less than the other word
                minIndex = j;
        }
        if (minIndex != i) 
            swap(unique[i], unique[minIndex]); // Swap the words
    }
    return unique;
}

vector<unsigned int> count_unique_words(vector<string>& words, vector<string>& unique)
{
    vector<unsigned int> wordCount(unique.size(), 0);
    for(int i = 0; i < unique.size(); i++) // Loop over the unique words
    {
        for(int j = 0; j < words.size(); j++) // Loop over all the words
        {
            if(unique[i] == words[j]) // If the words match 
                wordCount[i]++; // add one to the unique word count
        }
    }
    return wordCount;
}

void print_unique_word_counts(const vector<string>& words, const vector<unsigned int>& counts)
{
    int maxWordLen = 0;
    for(int i = 0; i < words.size(); i++)
    {
        string word = words[i];
        if(word.length() > maxWordLen)
            maxWordLen = word.length(); // Get the size of the biggest word
    }

    unsigned int maxCount = 0;
    for(int i = 0; i < counts.size(); i++)
    {
        unsigned int count = counts[i];
        if(count > maxCount)
            maxCount = count; // Get the size of the largest count
    }

    for(int i = 0; i < words.size(); i++)
    {
        string word = words[i];
        unsigned int count = counts[i];
        cout << left << setw(maxWordLen) << word << " : " << right << setw(to_string(maxCount).length()) << count << endl;
    }
}

void count_letters(const vector<string>& words, unsigned int letterCounts[])
{
    for(int i = 0; i < words.size(); i++) // Iterate over the words
    {
        for(int j = 0; j < words[i].length(); j++) // Iterate over the characters
        {
            char letter = toupper(words[i][j]); // Convert all to uppercase
            if(letter >= 'A' && letter <= 'Z') // Make sure only checking A-Z
                letterCounts[letter - 'A']++; // Add to the count
        }
    }
}

void print_letter_counts(unsigned int letterCounts[])
{
    int maxCount = 0;
    for(int i = 0; i < 26; i++)
    {
        if(letterCounts[i] > maxCount)
            maxCount = letterCounts[i]; // Find the greatest count for spacing
    }
    for (int i = 0; i < 26; i++) // Loop through all the letters
        cout << char('A' + i) << ": " << right << setw(to_string(maxCount).length()) << letterCounts[i] << endl;
}

void print_max_min_letter(const unsigned int letters[])
{
    char minLetter = 'A';
    char maxLetter = 'A';
    unsigned int minCount = letters[0];
    unsigned int maxCount = letters[0];
    unsigned int totCount = 0;

    // Find the min and max letters and their counts
    for (size_t i = 0; i < 26; i++) {
        totCount += letters[i];
        if (letters[i] < minCount) {
            minLetter = 'A' + i;
            minCount = letters[i];
        }
        if (letters[i] > maxCount) {
            maxLetter = 'A' + i;
            maxCount = letters[i];
        }
    }

    // Print out the min and max letters and their counts
    cout << "Least Frequent Letter: " << left << setw(2) << minLetter << " " << right << setw(to_string(maxCount).length()) << minCount << " (" << fixed << setprecision(3) << (static_cast<double>(minCount) / totCount) * 100 << "%)" << endl;
    cout << " Most Frequent Letter: " << left << setw(2) << maxLetter << " " << right << setw(to_string(maxCount).length()) << maxCount << " (" << fixed << setprecision(3) << (static_cast<double>(maxCount) / totCount) * 100 << "%)" << endl;

}