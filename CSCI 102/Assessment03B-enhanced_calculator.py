#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 03B
#   References: 
#   Time: 20 minutes


# Variable Decleration
operand_one = 0.0
operand_two = 0.0

# Operation Decleration
sum = 0.0
difference = 0.0
quotient = 0.0
product = 0.0
remainder = 0.0

# Get Input
print("Input the first operand.")
operand_one = float(input("FIRST> "))
print("Input the second operand.")
operand_two = float(input("SECOND> "))

# Operations
operation = {
    1: float(operand_one + operand_two),
    2: float(operand_one - operand_two),
    3: float(operand_one * operand_two),
    4: float(operand_one // operand_two),   
}

# Chose Operation
print("Choose one of the following operations:")
print(" 1 - addition\n 2 - subtraction\n 3 - multiplication\n 4 - division")

# Get Operation Chosen
chosenOperation = int(input("OPERATION> "))

# Output
for i in operation:
    if chosenOperation == i & chosenOperation != 4:
        print(f"OUTPUT {operation[i]:.6f}")
        break
    elif chosenOperation == 4:
        print(f"OUTPUT {operation[4]:.0f}\nOUTPUT {(operand_one % operand_two):.0f}")
        break

print("Thank you for using our calculator.")