#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 02A
#   References: 
#   Time: 15 minutes


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
sum = float(operand_one + operand_two)
difference = float(operand_one - operand_two)
product = float(operand_one * operand_two)
quotient = float(operand_one / operand_two)
remainder = float(operand_one % operand_two)

# Output
print("The sum of", operand_one, "and", operand_two, "is", f'{sum:.1f}')
print("OUTPUT", f'{sum:.1f}')

print("The difference of", operand_one, "and", operand_two, "is", f'{difference:.1f}')
print("OUTPUT", f'{difference:.1f}')

print("The product of", operand_one, "and", operand_two, "is", f'{product:.1f}')
print("OUTPUT", f'{product:.1f}')

print("The quotient of", operand_one, "and", operand_two, "is", f'{quotient:.0f}')
print("OUTPUT", f'{quotient:.0f}')

print("The remainder of", operand_one, "and", operand_two, "is", f'{remainder:.2f}')
print("OUTPUT", f'{remainder:.2f}')