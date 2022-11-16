# Tristen Erpelding
# CSCI 101 – H Sections
# Python Lab 4 - Wordle
# References:
# Time: 25 minutes

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Code sections within these borders are complete and should NOT be changed
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Code sections within these borders will have a task for you to complete
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# random is a library available to us from the Python Language.
# It lets us generate random or predicted numbers to add
# some variety into our code. You'll use it yourselves later this term.
import random

# Prints welcome message
print("Welcome to Wordle!")
print("Here you will provide the length of the words you want to play with and")
print("a random seed value to start the game!")

# The provided dictionary file only has words of certain lengths
valid_lengths = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                 15, 16, 17, 18, 19, 20, 21, 22, 24, 28, 29]
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Continues to ask for word_length until the given number is valid
length_is_valid = False
while not length_is_valid:
    # TODO: Ask player for the INTEGER length of word to play with
    word_length = int(input("LENGTH> "))

    # Checks if words of the requested length exist in our dictionary
    if word_length in valid_lengths:
        length_is_valid = True
    else:
        print(f"There are no {word_length}-letter words in the game.")
        print("Please pick again.")

# TODO: Ask the player for a STRING to help start the random number generator
seed = str(input("SEED> "))
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'''
The following portion will read in the dictionary file and store all words
of the player-given desired length. It will then choose a random word from
the list to play the game with.
'''

# Creates empty list to store words
word_bank = []

# Opens the file based on the given name ("dictionary.txt"), in read mode ('r'),
# and interprets the binary data using the utf-8 protocol.
# Then, it assigns the file to a variable called 'dictionary_file'.
# If this is confusing, don't worry about details now, we will cover them later.
with open("dictionary.txt", 'r', encoding='utf-8') as dictionary_file:
    # For each line in the given text file
    for line in dictionary_file:
        # Erases any whitespace at the end of the line (tab, spaces, newline)
        line = line.strip()

        # If the word remaining in the line is the desired length
        if len(line) == word_length:
            # Add it to our word bank for the game
            word_bank.append(line)

'''
After and outside of the with statement on line 64 above,
the document is closed automatically and the word list is complete.
'''

# Uses the entered seed value to 'kickstart' the random number generator
random.seed(seed)

# Uses the random generator to pick one word from the list to play with.
# random.choice(list) takes in a list and returns one randomly chosen value.
secret_word = random.choice(word_bank)
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# Uncomment the next line to see what words were found, if you'd like.
# print(f"Word bank:\n{word_bank}")

# Uncomment the next line to see what word was randomly picked, if you'd like.
# print(f"Secret word: {secret_word}")
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'''
After the secret word is chosen, we need more structures to start playing.
'''

# Players are allowed 6 tries to guess the secret word
num_guesses_allowed = 6

# Players start with 0 guesses made
num_guesses_used = 0

# Until the word is guessed, or the number of tries has run out, keep playing.
# The game is not over yet, so game_result is an empty string for now.
game_result = ""

# Creates an empty list to store the guesses
guess_list = []

'''
Time to play!
'''

# While we can still play
while game_result == "":

    # Continues to ask the player for new_guess until:
    # the guess is the right size,
    # the guess is a real word,
    # and the guess hasn't been used before.
    # When all the conditions are true, new_guess is valid.
    guess_is_valid = False
    while not guess_is_valid:
        # Collects a new guess from the player
        new_guess = input("GUESS> ")

        # Forces all letters to lowercase, just in case
        new_guess = new_guess.lower()

        # Checks new_guess length
        if len(new_guess) != word_length:
            print(f"Please enter a {word_length}-letter word.")

        # Checks if new_guess is a real word
        elif new_guess not in word_bank:
            print(f"Please enter a real word.")

        # Checks if new_guess has been used before
        elif new_guess in guess_list:
            print(f"Please enter a word you haven't guessed yet.")

        else:
            guess_is_valid = True

    # Creates list for the result of this guess and fills with placeholders '_'
    result_list = list('_' * word_length)

    # Creates a list of the secret word characters to work with.
    # (We use a list here instead of a string, because it's easier
    # to change individual characters of a list.)
    secret_list = list(secret_word)
    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    '''
    Now we move through the player's guess trying match letters and locations
    '''

    # TODO: Use the pseudocode below as an outline to evaluate the new_guess
    
    # For each index i in the new_guess
    for i,char in enumerate(new_guess):
        # print(f"Searching for a match at index {i}...")
        
        if new_guess[i] == secret_list[i]: # If new_guess at i is equal to secret_list at i
            # print("Match found!")
            # result_list at i should be changed to 'x'
            result_list[i] = "x"
            # secret_list at i should be changed to '_' to avoid double matches
            secret_list[i] = "_"



    # For each index i in the new_guess
    for i,char in enumerate(new_guess):
        
        # If we've already matched this letter, result_list here will be 'x'
        # and you should skip to the next i loop
        if result_list[i] == "x":
            continue          
            
        # Else
        else:
            # Uncomment the next line to help with debugging
            # print(f"Searching for a match for character {new_guess[i]}")

            # For each index j in the secret_list
            for j,char2 in enumerate(secret_list):

                # If new_guess at i is equal to secret_list at j
                if char == char2:
                    # Letter exists in secret word!
                    # Uncomment the next line to help with debugging
                    # print("Match found!") 
                    # result_list at i should be changed to 'o'
                    result_list[i] = "o"
                    # secret_list at j should be changed to '_', like before
                    secret_list[j] = "_"
                    # skip the rest of the j loops
                    break

    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    # It would be easier for the player to see result_list as a string.
    # This creates a string from all entries in a given list,
    # with spaces separating each entry for clarity.
    print(f"OUTPUT {' '.join(result_list)}")

    # Appends new_guess to guess_list
    # This helps to make sure the player doesn't guess the same thing twice
    guess_list.append(new_guess)

    # Increments guess count
    num_guesses_used += 1
    # ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    '''
    Time to check if the player has won, lost, or needs to keep playing.
    '''

    # If all characters are exact matches in the right spot
    # The entire result_list will be full of 'x' and the game is over
    # TODO: Write an if statement to check if the number of 'x'
    #  present in result_list is the same as word_length.
    #  If so, set game_result to the appropriate value.
    if result_list.count("x") == word_length:
        game_result = "Won"



    # The game might also be over if the player is out of guesses
    # TODO: Write the elif statement to check if the number guesses used
    #  is equal to the number of guesses available.
    #  If so, set game_result to the appropriate value.
    elif num_guesses_used == num_guesses_allowed:
        game_result = "Lost"


    # !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# If we reach this line of code, the game is over; we've exited the while loop.
print("Game over.")

# Print statements without OUTPUT aren't graded, but they are helpful.
# Checks the game_result and tells the player
if game_result == "Won":
    print(f"Congratulations!")
    print(f"You guessed the word '{secret_word}' in {num_guesses_used} guesses!")
    print("You win!")
elif game_result == "Lost":
    print(f"You were not able to guess the word '{secret_word}'.")
    print("You lose.")

# Prints the outputs
print(f"OUTPUT {secret_word}")
print(f"OUTPUT {game_result}")
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
