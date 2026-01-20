from Crypto.Protocol.KDF import PBKDF2
from Crypto.Random import get_random_bytes
from Crypto.Hash import SHA256
import base64

def hash_password(password):
    salt = get_random_bytes(16)
    hashed = PBKDF2(password, salt, dkLen=32, count=100000, hmac_hash_module=SHA256)
    return base64.b64encode(salt + hashed).decode()

def verify_password(stored, password_attempt):
    decoded = base64.b64decode(stored)
    salt = decoded[:16]
    stored_hash = decoded[16:]
    new_hash = PBKDF2(password_attempt, salt, dkLen=32, count=100000, hmac_hash_module=SHA256)
    return new_hash == stored_hash

password = "MySecurePassword123!"
stored_hash = hash_password(password)

print("Stored hash:", stored_hash)

attempt = "MySecurePassword123!"
print("Password Attempt:", attempt)
print("Password match:", verify_password(stored_hash, attempt))

attempt2 = "Password123!"
print("Password Attempt:", attempt2)
print("Password match:", verify_password(stored_hash, attempt2))
