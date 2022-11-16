#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 06B
#   References: 
#   Time: 25 minutes

# The list store the numbers
numbers = []

# Get the count for how many numbers to be inputed
print("Count of numbers to estimate:")
count = int(input("COUNT> "))

print("Input each number to estimate. ")

# Loop through the count to get the input
for i in range(count):
    num = float(input("NUMBER> "))
    numbers.append(num)

# For all the numbers in the list run the calculation
for i in numbers:
    intital_guess = 10
    better_guess = 0
    iterations = 0
    while True:
        better_guess = ((intital_guess) + (float(i) / intital_guess)) / 2
        if intital_guess == better_guess:
            iterations += 1
            break
        else:
            intital_guess = better_guess
            iterations += 1
    print(f"OUTPUT After {iterations} iterations, {i}^0.5 = {better_guess:.2f}")