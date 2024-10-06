# An additive prime is a number which is both prime and the sum of its digits forms a prime number. For example, 23 is an additive prime, since 23 is prime, and 2+3=5 is also prime, but 13 is not, since 1+3=4, and 4 is not prime.

def is_prime(number):
    if number == 1:
        return False
    for i in range(2, number):
        if number % i == 0:
            return False
    return True


def digit_sum(number):
    num = 0
    number = str(number)
    for i in range(len(str(number))):
        if number[i] == ".":
            continue
        elif number[i] == "-":
            temp = int(number[i+1]) * -1
            num += int(temp)
        else:
            temp =  int(number[i])
        num += int(temp)
    return num


def additive_prime(number):
    # Call the previous two functions, this function should be no more than 15 lines :)
    if is_prime(number):
        temp = digit_sum(number)
        if is_prime(temp):
            return("ADDITIVE PRIME")
        else:
            return("PRIME")
    else:
        return("COMPOSITE")
