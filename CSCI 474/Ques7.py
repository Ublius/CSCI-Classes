from Crypto.Protocol.KDF import PBKDF2
from Crypto.Random import get_random_bytes
from Crypto.Hash import SHA256
import time

def generate_key(password: str):
    salt = get_random_bytes(16)  # 128-bit salt

    hash_function = SHA256
    iterations = 1000  

    start_time = time.time()
    derived_key = PBKDF2(password, salt, dkLen=16, count=iterations, hmac_hash_module=hash_function)
    elapsed_time = time.time() - start_time
    
    if elapsed_time > 1:
        print(f"Warning: Key derivation took {elapsed_time:.3f} seconds, which is above the recommended threshold.")

    return derived_key, salt, elapsed_time

def test_iterations(password: str, salt: bytes, target_time=1):
    for count in [1000, 5000, 10000, 20000, 1000000]:
        print(f"Testing with {count} iterations...")
        start_time = time.time()
        derived_key = PBKDF2(password, salt, dkLen=16, count=count, hmac_hash_module=SHA256)
        elapsed_time = time.time() - start_time
        print(f"Iterations: {count}, Time Taken: {elapsed_time:.3f} seconds")
        if elapsed_time > target_time:
            print(f"Warning: Key derivation took too long at {elapsed_time:.3f} seconds with {count} iterations.")
            break
        else:
            print(f"Passed with {count} iterations.")
        

# Example usage
password = "SecurePassword123"  # Example user-provided password
key, salt, elapsed_time = generate_key(password)

# print(f"Generated Key: {key.hex()}")
print(f"Salt: {salt.hex()}")
# print(f"Time Taken for Key Generation: {elapsed_time:.3f} seconds")

test_iterations(password, salt)
