# Define and implement a function digit_sum that takes a single number parameter (integer or float) and returns the sum of the digits of the number. If the number is negative, make the digit immediately following the negative sign negative (keep the rest of the digits in the number positive). Ignore periods representing decimal points.

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
