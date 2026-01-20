from Crypto.Hash import SHA256
import random
import string
import time

def modified_hash(data):
    h = SHA256.new(data.encode('utf-8')).digest()
    return h[:3]

def random_string(length=6):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def brute_force_one_way(target_hash):
    trials = 0
    start_time = time.time()
    while True:
        guess = random_string()
        h = modified_hash(guess)
        trials += 1
        if h == target_hash:
            end_time = time.time()
            return guess, trials, end_time - start_time

original_input = random_string()
target_hash = modified_hash(original_input)

print(f"Original input: {original_input}")
print(f"Target hash (24-bit prefix): {target_hash.hex()}")

found_input, num_trials, elapsed = brute_force_one_way(target_hash)
print(f"Match found! Input: {found_input}")
print(f"Trials: {num_trials}")
print(f"Time taken: {elapsed:.4f} seconds")
