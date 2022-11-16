#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Assessment 3
#   References: 
#   Time: 50 min

#  Imports
import string

# Variables
count = 0
wordbank = [] # Create a list of words that are the length
secretword = ""
secrectlength = 0

# Get the choice for which action to perform
print("Would you like to print the number of times a specific word appears OR print the number of words of a specific length? (1 or 2)")
choice = int(input("CHOICE> "))

# If look for specific word get the secrectword to find
if choice == 1:
    print("Enter a word:")
    secretword = str(input("WORD> "))
    secretword = secretword.lower() # Make the secretword lower

# If look for specfic length get the length to find the words
if choice == 2:
    print("Enter a length:")
    secrectlength = int(input("LENGTH> "))

with open('peter_pan.txt', "r") as file:
    if choice == 1: # Check if choice was 1
        for line in file:
            line = line.strip() # Remove beginning and ending spaces
            line = line.lower() # Convert each line to lower
            line = line.replace("-", " ") # Replace each hyphon with a space
            line = line.translate(line.maketrans("", "", string.punctuation)) # Remove punctation
            words = line.split(" ") # Create a list of words
            for word in words: # Get each word in the list of words
                if word == secretword: # if word is eqaul to the secret word 
                    count += 1 # Increment the count by 1
        print(f"The number of times {secretword} appears in the document is:")
        print(f"OUTPUT {count}")

    if choice == 2: # Check if choice was 2
        for line in file:
            line = line.strip() # Remove beginning and ending spaces
            line = line.lower() # Convert each line to lower
            line = line.replace("-", " ") # Replace each hyphon with a space
            line = line.translate(line.maketrans("", "", string.punctuation)) # Remove punctation
            words = line.split(" ") # Create a list of words
            for word in words: # Get each word in the list of words
                    if len(word) == secrectlength and word.isalpha(): # if the length of each word equals the secrentlength to find
                        count += 1 # increase the count
                        wordbank.append(word) # Add all the length words to the list
        wordbank = list(dict.fromkeys(wordbank)) # Remove repeat words
        print(f"The number of words in the document with length {secrectlength} is:")
        print(f"OUTPUT {count}")
        print(f"The number of unique words in the document with length {secrectlength} is:")
        print(f"OUTPUT {len(wordbank)}")
