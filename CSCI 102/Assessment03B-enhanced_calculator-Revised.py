#Tristen Erpelding - terpelding@mines.edu
# Ava Courtney - acourtney@mines.edu
# Josh Kim - joshuakim@mines.edu


#prompts user to input two operands 
print('Welcome to our Enhanced Calculator!')
frst = float(input('Input the first operand.\nFIRST> '))
scnd = float(input('Input the second operand.\nSECOND> '))


# Operations
operation = {
    1: float(frst + scnd),
    2: float(frst - scnd),
    3: float(frst * scnd),
    4: float(frst // scnd),   
}

# Chose Operation
print("Choose one of the following operations:")
print(" 1 - addition\n 2 - subtraction\n 3 - multiplication\n 4 - division")

# Get Operation Chosen
chosenOperation = int(input("OPERATION> "))

# Iterate the dictionary with the operation given
for i in operation:
    if chosenOperation == 4:
        print(f"OUTPUT {operation[4]:.0f}\nOUTPUT {(frst % scnd):.0f}")
        break
    elif chosenOperation == i:
        print(f"OUTPUT {operation[i]:.6f}")
        break
    # else:
    #     print("That is not a valid operation")
    #     break

print("Thank you for using our calculator.")