from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import binascii

# Function to encrypt using AES-128-CBC
def encrypt_aes_cbc(plaintext, key):
    # Ensure the key is exactly 16 bytes (128 bits)
    key = key.ljust(16, b' ')[:16]  # Pad with spaces (0x20) and truncate if longer than 16 bytes
    iv = b'\x00' * 16  # IV is all zeros

    # Create AES cipher object in CBC mode
    cipher = AES.new(key, AES.MODE_CBC, iv)

    # Pad plaintext to a multiple of the block size and encrypt
    encrypted = cipher.encrypt(pad(plaintext.encode('utf-8'), AES.block_size))
    return encrypted

# Main function to encrypt words and compare ciphertexts
def encrypt_and_compare(word_list, target_ciphertext):
    for word in word_list:
        key = word.encode('utf-8')  # Convert word to bytes
        # Encrypt the word
        encrypted = encrypt_aes_cbc(word, key)
        
        # Compare the encrypted ciphertext with the provided ciphertext
        if encrypted == target_ciphertext:
            print(f"Found matching key: {word}")
            return word

    print("No matching key found.")
    return None

# The provided ciphertext (hexadecimal format)
ciphertext_hex = "c34c12baffcb609d5b63da20d2cbf058c8e26f3903ab070b99f18d6fbcdfcd19"
target_ciphertext = binascii.unhexlify(ciphertext_hex)  # Convert to bytes

# Load the word list (the provided words.txt)
with open("words.txt", "r") as file:
    word_list = file.read().splitlines()

# Attempt to find the key by comparing ciphertexts
key = encrypt_and_compare(word_list, target_ciphertext)
if key:
    print(f"Successfully found matching key: {key}")
else:
    print("No matching key found.")
