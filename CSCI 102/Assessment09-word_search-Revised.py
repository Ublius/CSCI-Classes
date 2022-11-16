#Tristen Erpelding - terpelding@mines.edu
# Jesse Molnar - jmolnar@mines.edu
# Melodie Montoya - melodiemontoya@mines.edu

import random

# Get the length of words to search for
print("Enter the length of the words to find: ")
lenword = int(input("LENGTH> "))

# Get the seed for the random word
print("Enter the seed to use: ")
seed = input(("SEED> "))

countwords = 0 # Initalzie the count of words with the set length
wordbank = [] # Create a list of words that are the length

with open('dictionary.txt', "r") as file:
    for line in file:
        # Erases any whitespace at the end of the line (tab, spaces, newline)
        line = line.strip()
        # If the word remaining in the line is the desired length
        if len(line) == lenword:
            countwords += 1 # increase the count
            # Add it to the word bank
            wordbank.append(line)

if countwords == 0: # Check to see if there are any word
    print(f"The number of words with length {lenword} is:")
    print(f"OUTPUT {countwords}")
    print(f"There are no words of length {lenword} in the dictionary.")
    print("OUTPUT None") # No words

else: # If words print
    print(f"The number of words with length {lenword} is:")
    print(f"OUTPUT {countwords}")
    print(f"Here is a random word with length {lenword}:")
    random.seed(seed)
    print(f"OUTPUT {random.choice(wordbank)}")