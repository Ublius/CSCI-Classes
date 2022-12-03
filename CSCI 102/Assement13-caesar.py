# Tristen Erpelding
# CSCI 102 – Section F 
# Assessment 13
# References: https://www.asciitable.com/, CodeForGeeks found how to get the max number in a nested list in the nth index
# Time: 100 mins

import string

alphabet = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z']
alphabetCap = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']

dictWords = []

with open("dictionary.txt", 'r', encoding='utf-8') as dictionary_file:
    # For each line in the given text file
    for line in dictionary_file:
        # Erases any whitespace at the end of the line (tab, spaces, newline)
        line = line.strip()
        dictWords.append(line)
            
def bruteforceDecrypt(text_to_decrypt, shift):
    plain_text = ""
    for letter in text_to_decrypt:
        if letter == ' ':
            # Skip the space
            plain_text = plain_text + letter
        elif letter in string.punctuation:
            # Skip special puncation
            plain_text = plain_text + letter
        elif letter.isdigit():
            # Skip numbers
            plain_text = plain_text + letter
        elif letter.isupper(): # Shift the upper case letters
            position = alphabetCap.index(letter)
            new_position = position - shift
            plain_text += alphabetCap[new_position]
        else: # Shift the lower case letters
            position = alphabet.index(letter)
            new_position = position - shift
            plain_text += alphabet[new_position]
    return plain_text
    

# TODO: Write encryption functiond
def encrypt(text_to_encrypt, shift):
    encyrptedText = ""
    for char in text_to_encrypt:
        if char == ' ':
            # Skip the space
            encyrptedText = encyrptedText + char
        elif char in string.punctuation:
            # Skip special puncation
            encyrptedText = encyrptedText + char
        elif char.isdigit():
            # Skip numbers
            encyrptedText = encyrptedText + char
        elif char.isupper():
            # Traverse through 65 as that is the mathmatical equivlent for capital A in ASCII
            # Add shift to work for both pos and neg 
            encyrptedText = encyrptedText + chr((ord(char) + shift - 65) % 26 + 65)
        else:
            # Traverse through 97 as that is the mathmatical equivlent for lowercase a in ASCII
            # Add shift to work for both pos and neg 
            encyrptedText = encyrptedText + chr((ord(char) + shift - 97) % 26 + 97)
    return encyrptedText

# TODO: Write decryption function
def decrypt(text_to_decrypt):
    counter = 0
    bruteShift = 0
    lst = []
    
    while bruteShift <= 26:
        plain_text = bruteforceDecrypt(text_to_decrypt, bruteShift)
                
        words = plain_text.split() # Convert the text into a list of words
        
        for i in range(len(words)):
            words[i] = words[i].lower()
        
        for wordDict in dictWords:
            for word in words:
                if word == wordDict: # Compare the list of words with the dictionary of words
                    counter += 1 # Add to the counter if words match
        
        lst.append([bruteShift, counter]) # Add to the list with the shift and how many matching words
        bruteShift += 1 # Increase the shift
        counter = 0 # Rest the counter
        plain_text = ""
    
    finaList = max(lst, key=lambda x: x[1]) # Get the max number in a nest list at the nth index
    shift = finaList[0] # Get the actual shift amount
    plain_text = bruteforceDecrypt(text_to_decrypt, shift) # Run the function to get the decrypted message 
    
    return plain_text, shift

# The below syntax may seem weird - Basically, it allows us to know if you are running your code with
#  the run command / from the command line, or if you actually are running each function individually.
#  For more information check out this resource: https://www.geeksforgeeks.org/what-does-the-if-__name__-__main__-do/
if __name__ == "__main__":
    print(
        "Welcome to our Caesar Cipher Algorithm!\n" +
        "Choose one of the following methods:\n" +
        "\t1 - Encryption\n" +
        "\t2 - Decryption"
    )
    while True:
        userIn = str(input("OPERATION> "))

        # If the user inputs 1, for encryption
        if userIn == "1":
            # TODO: use your encryption function here
            print("Input the message to encrypt and the desired shift.")
            message = input("MESSAGE> ")
            shift = int(input("SHIFT> "))
            encyptWord = encrypt(message,shift)
            print(f"Your message encrypted with shift {shift}: ")
            print(f"OUTPUT {encyptWord}")
            break

        # If the user inputs 2, for encryption
        elif userIn == "2":
            # TODO: Use your decryption function here
            print("Input the message to decrypt using brute force.")
            message = input("MESSAGE_TO_DECRYPT> ")
            decryptMessage,shift = decrypt(message)
            decrypt(message)
            print("The original message is: ")
            print(f"OUTPUT {decryptMessage}")
            print("The original message was encrypted with a shift of: ")
            print(f"OUTPUT {shift}")
            break
        else:
            print("Invalid Operation!")
