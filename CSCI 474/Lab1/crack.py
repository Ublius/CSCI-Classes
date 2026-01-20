from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
from Crypto.Random import get_random_bytes
import binascii

def decrypt_aes_cbc(ciphertext, key):
    key = key.ljust(16, b' ')  # Ensure the key is 16 bytes by padding with spaces (0x20)
    iv = b'\x00' * 16  # IV is all zeros

    cipher = AES.new(key, AES.MODE_CBC, iv)

    decrypted = unpad(cipher.decrypt(ciphertext), AES.block_size)
    return decrypted

def brute_force_key(ciphertext, word_list):
    for word in word_list:
        key = word.encode('utf-8')  
        try:
            decrypted = decrypt_aes_cbc(ciphertext, key)
            if decrypted.decode('utf-8') == "This is a top secret.":
                print(f"Key found: {word}")
                return word
        except ValueError:
            continue
    return None

ciphertext_hex = "c34c12baffcb609d5b63da20d2cbf058c8e26f3903ab070b99f18d6fbcdfcd19"
ciphertext = binascii.unhexlify(ciphertext_hex) 

with open("words.txt", "r") as file:
    word_list = file.read().splitlines()

key = brute_force_key(ciphertext, word_list)
if key:
    print(f"Successfully decrypted with key: {key}")
else:
    print("Key not found.")
