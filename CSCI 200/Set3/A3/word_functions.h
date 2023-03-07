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

bool open_file(std::ifstream& fileIn, const std::string& filename);

std::vector<std::string> read_words_from_file(std::ifstream& fileIn);

void remove_punctuation(std::vector<std::string>& allWords, const std::string& PUNCTUATION_TO_REMOVE);

void capitalize_words(std::vector<std::string>& words);

std::vector<std::string> filter_unique_words(const std::vector<std::string>& words);

std::vector<unsigned int> count_unique_words(std::vector<std::string>& words, std::vector<std::string>& unique);

void print_unique_word_counts(const std::vector<std::string>& words, const std::vector<unsigned int>& counts);

void count_letters(const std::vector<std::string>& words, unsigned int letterCounts[]);

void print_letter_counts(unsigned int letterCounts[]);

void print_max_min_letter(const unsigned int letters[]);

#endif