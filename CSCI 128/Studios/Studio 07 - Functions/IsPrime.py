#Write a function which, given a number, returns a boolean indicating the given number is prime (true) or composite (false). Recall that a prime number is only divisible by one and itself (2 is considered prime).

def is_prime(number):
    # Fill in your code here.
    if number == 1:
        return False
    for i in range(2, number):
        if number % i == 0:
            return False
    return True
