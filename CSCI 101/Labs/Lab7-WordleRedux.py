# Tristen Erpelding
# CSCI 101 – H Sections
# Python Lab 7 - WordleRedux
# References:
# Time: 45 minutes

import random

valid_lengths = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                 15, 16, 17, 18, 19, 20, 21, 22, 24, 28, 29]

def get_word_bank(word_length):
    word_bank = []
    # Opens the file based on the given name ("dictionary.txt"), in read mode ('r'),
    # and interprets the binary data using the utf-8 protocol.
    # Then, it assigns the file to a variable called 'dictionary_file'.
    with open("dictionary.txt", 'r', encoding='utf-8') as dictionary_file:
        # For each line in the given text file
        for line in dictionary_file:
            # Erases any whitespace at the end of the line (tab, spaces, newline)
            line = line.strip()

            # If the word remaining in the line is the desired length
            if len(line) == word_length:
                # Add it to our word bank for the game
                word_bank.append(line)
    return word_bank


def gen_guess_result(new_guess, secret_word, word_length):
    # Creates list for the result of this guess and fills with placeholders '_'
    result_list = list('_' * word_length)

    # Creates a list of the secret word characters to work with.
    # (We use a list here instead of a string, because it's easier
    # to change individual characters of a list.)
    secret_list = list(secret_word)
    
    # For each index i in the new_guess
    for i in range(len(new_guess)):
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
    return result_list


# Recommended function, not required
def get_guess(guess_list, word_length, word_bank):
    guess_is_valid = False
    new_guess = ""
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
    return new_guess


if __name__ == '__main__':

    # Use this function to run your test cases
    # Create more or comment these out as necessary

    # Example Word Bank function calls from Canvas 
    # print(get_word_bank(5)[:5])
    # print(get_word_bank(6)[:5])
    # print(get_word_bank(7)[:5])

    # Example Result List function calls from Canvas
    # word = "hello"
    # print(gen_guess_result("heavy", word, 5))
    # print(gen_guess_result("hails", word, 5))
    # print(gen_guess_result("hello", word, 5))

    # Example Get Guess function calls from Canvas
    # random.seed("CS101")
    # bank = get_word_bank(5)
    # word = random.choice(bank)
    # guess_list = ["cover", "togas"]
    # guess_list.append(get_guess(guess_list, 5, bank))
    # print(guess_list)

    # When your minimum 3 functions are ready to go, comment out the examples
    # above and start writing the functionality for user inputs and multiple
    # round of play
    
    # Begin Play
    print("Welcome to Wordle!")
    print("Enter a seed to start the random number generator.")
    
    seed = str(input("SEED> "))
    
    print("Enter a length of word to play with.")
    word_length = 0
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
            
    word_bank = get_word_bank(word_length)
    # Players are allowed 6 tries to guess the secret word
    num_guesses_allowed = 6
    # Player start 0 wins
    num_wins = 0
    # Player start 0 losses
    num_losses = 0
    # Continue game str
    continue_game = "Yes"
    
    # Uses the entered seed value to 'kickstart' the random number generator
    random.seed(seed)
    
    while continue_game == "Yes":
        # Uses the random generator to pick one word from the list to play with.
        # random.choice(list) takes in a list and returns one randomly chosen value.
        print(f"I've chosen a secret {word_length}-letter word. Make a guess.")
        secret_word = random.choice(word_bank)
        # Players start with 0 guesses made
        num_guesses_used = 0
        # Until the word is guessed, or the number of tries has run out, keep playing.
        # The game is not over yet, so game_result is an empty string for now.
        game_result = ""
        # Creates an empty list to store the guesses
        guess_list = []
        while game_result == "":
            guess_list.append(get_guess(guess_list, word_length, word_bank))
            num_guesses_used += 1
            result_list = gen_guess_result(guess_list[-1], secret_word, word_length)
            print(f"OUTPUT {' '.join(result_list)}")
            if guess_list[-1] == secret_word:
                print("Game Over.")
                print("Congratulations!")
                print(f"You guessed the word '{secret_word}' in 3 guesses!")
                game_result = "Won"
                num_wins += 1
                break
            elif num_guesses_used == num_guesses_allowed:
                print("Game Over.")
                print(f"You were not able to guess the secret word '{secret_word}'.")
                game_result = "Lost"
                num_losses += 1
                break
        
        print(f"OUTPUT {secret_word}")
        print(f"OUTPUT {game_result}")
        print("Would you like to play again?")
        continue_game = str(input("CONTINUE> "))
        continue
    
    print(f"You won {num_wins} times and lost {num_losses} times.")
    print(f"OUTPUT {num_wins}")
    print(f"OUTPUT {num_losses}")