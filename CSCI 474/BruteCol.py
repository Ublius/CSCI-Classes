from Crypto.Hash import SHA256
import random
import string
import time

def modified_hash(data):
    h = SHA256.new(data.encode('utf-8')).digest()
    return h[:3]

def random_string(length=6):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

def brute_force_collision():
    seen_hashes = {}
    trials = 0
    start_time = time.time()
    while True:
        guess = random_string()
        h = modified_hash(guess)
        h_hex = h.hex()
        trials += 1
        if h_hex in seen_hashes:
            end_time = time.time()
            return seen_hashes[h_hex], guess, h_hex, trials, end_time - start_time
        seen_hashes[h_hex] = guess

input1, input2, collision_hash, num_trials, elapsed = brute_force_collision()
print(f"Collision found!")
print(f"Input 1: {input1}")
print(f"Input 2: {input2}")
print(f"Hash: {collision_hash}")
print(f"Trials: {num_trials}")
print(f"Time taken: {elapsed:.4f} seconds")
