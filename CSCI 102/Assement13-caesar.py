# Tristen Erpelding
# CSCI 102 – Section F 
# Assessment 13
# References: https://www.asciitable.com/
# Time:

import string

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
    pass
    # return decrypted_text, shift

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
            print("The original message is: ")
            print(f"OUTPUT {decryptMessage}")
            print("The original message was encrypted with a shift of: ")
            print(f"OUTPUT {shift}")
            break
        else:
            print("Invalid Operation!")
