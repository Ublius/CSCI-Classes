# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 12
# References:
# Time: 45 minutes

import math
import string

def load_file(filename):
    lines = []
    with open(filename, mode="r") as file:
        for row in file:
            row = row.strip() # remove white space in front and back
            lines.append(row) # Store the rows in a file into a list
    return lines

def update_string(str1, str2, pos):
    word = str1[0:pos] + str2 + str1[pos+1:] # Combine the second string at index pos
    print("OUTPUT", word)

def find_word_count(list, word):
    count = 0
    for row in list:
        # row = row.lower() # Convert each line to lower
        # row = row.replace("-", " ") # Replace each hyphon with a space
        # row = row.translate(row.maketrans("", "", string.punctuation)) # Remove punctation
        test = row.split()
        for i in test:
            if word in i:
                count += 1 # Count how mnay words are in the list given
    return count

def score_finder(players, scores, playername):
    playername = playername.lower() # make the playername lower
    lower = []
    for i in range(len(players)):
        lower.append(players[i].lower()) # Convert the list of nams to lower
    for i in range(len(lower)):
        if lower[i] == playername: # Check if same name
            print(f"OUTPUT {players[i]} got a score of {scores[i]}")
            return # Early return
    print("OUTPUT player not found") # Nothing then player not found
    
def union(list1, list2):
    list = list1 + list2 # Join the two list given
    return(list)

def intersect(list1, list2):
    list= []
    for i in list1:
        for j in list2:
            if i == j: # Check if list1 indexs match index 2 if so add to the list
                list.append(i)
    return list

def not_in(list1, list2):
    notList = list(set(list1).difference(list2)) # Find the differences between the two lists
    return notList

def is_prime(num):
    prime = 0
    if(num > 1): # Number needs to be greater than 1
        for i in range(2, int(math.sqrt(num)) + 1): # Go from 2 to the sqrt of num + 1
            if (num % i == 0): # if num mod i is 0
                prime = 1 # Then not prime and break
                break
        if (prime == 0): 
            return True
        else:
            return False
    else:
        return False