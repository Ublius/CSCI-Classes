/* CSCI 200: Assignment 3: A3 - Green Eggs and Ham
 *
 * Author: Tristen Erpelding
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *        Used a lot of the CPP reference website, https://cplusplus.com/reference    
 *
 *  This program reads in a text file and parses the contents to count word data.
 */

#ifndef WORD_FUNCTIONS_H
#define WORD_FUNCTIONS_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Open the input file stream for the corresponding filename. 
 * Check that the file opened correctly. The string filename will remain unchanged. 
 * 
 * @param fileIn Ifstream File
 * @param filename location of the file
 * @return true of file successfully opened
 */
bool open_file(std::ifstream& fileIn, const std::string& filename);

/**
 * @brief Read all the words that are in the file stream 
 * and return a vector of all the words in the order present in the file. 
 * 
 * @param fileIn Ifstream File
 * @return vector of type strings
 */
std::vector<std::string> read_words_from_file(std::ifstream& fileIn);

/**
 * @brief For each word in the vector, remove all occurrences of all the punctuation characters denoted by the punctuation string. 
 * When complete, the input vector will now hold all the words with punctuation removed. 
 * The punctuation string will remain unchanged.  
 * 
 * @param allWords vector of type string
 * @param PUNCTUATION_TO_REMOVE string with all the punctuation to remove 
 */
void remove_punctuation(std::vector<std::string>& allWords, const std::string& PUNCTUATION_TO_REMOVE);

/**
 * @brief For each word in the vector, convert each character to its upper case equivalent. 
 * When complete, the input vector will now hold all the words capitalized. 
 * 
 * @param words vector of type string
 */
void capitalize_words(std::vector<std::string>& words);

/**
 * @brief The function will return only the unique words present in the input vector. 
 * The output vector will not contain any duplicate words. 
 * 
 * @param words vector of type string
 * @return vector of type strings
 */
std::vector<std::string> filter_unique_words(const std::vector<std::string>& words);

/**
 * @brief The function will count the number of occurrences of each unique word in the entire text. 
 * The output vector will be the same size as the vector of unique words with element positions corresponding to the same word and count. 
 * 
 * @param words vector of type string
 * @param unique vector of type string
 * @return vector of type strings
 */
std::vector<unsigned int> count_unique_words(std::vector<std::string>& words, std::vector<std::string>& unique);

/**
 * @brief For each word, print out the word and its corresponding count to the standard out.
 * 
 * @param words vector of type string
 * @param counts vector of type unsigned int
 */
void print_unique_word_counts(const std::vector<std::string>& words, const std::vector<unsigned int>& counts);

/**
 * @brief Count the number of occurrences of each letter present in all words. 
 * Each position of the array corresponds to each letter as ordered by the English alphabet. 
 * Upon completion, the array will hold the counts of each letter and the vector of strings will remain unchanged. 
 * 
 * @param words vector of type string
 * @param letterCounts unsigned int
 */
void count_letters(const std::vector<std::string>& words, unsigned int letterCounts[]);

/**
 * @brief For each letter, print out the letter and its corresponding count to the standard out.
 * 
 * @param letterCounts unsigned int
 */
void print_letter_counts(unsigned int letterCounts[]);

/**
 * @brief Print out the two letters that occur least often and most often to the standard out. 
 * If there is more than one letter that occurs the same number of times, print the one that comes first alphabetically. 
 * Upon completion, the input array will remain unchanged. 
 * 
 * @param letters unsigned int
 */
void print_max_min_letter(const unsigned int letters[]);

#endif