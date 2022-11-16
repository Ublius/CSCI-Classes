# Tristen Erpelding
# CSCI 101 – H Sections
# Python Lab 6
# References:
# Time: 90 minutes

import csv

print("Enter the name of the file containing the math problems. ")
mathFile = input("MATHFILE> ") # Get the name of the math file to check
outputFile = input("OUTPUTFILE> ") # Get the name to ouput the file

csvList = [] # List of equations from the math csv

opertion1 = [] # A list of the operations to complete
opertion2 = [] # Second half of the operations to complete

outputFirst = [] # Get the output of the first half
outputSecond = [] # Get the output of the second half

checker = False # Check the bool value
finalList = [] # Final list of answers and bool

with open(mathFile, mode="r") as file:
    csvFile = csv.reader(file)
    for row in csvFile:
        csvList.append(row) # add the rows to the csvList

for lines in range(len(csvList)):
    firstOperand = []
    secondOperand = []
    equalSignPassed = False # check if the eqal sign has been hit
    for column in range(len(csvList[lines])):
        if csvList[lines][column] == "==":
            equalSignPassed = True # Set the equal sign passed
            continue
        if not(equalSignPassed):
            firstOperand.append(csvList[lines][column]) # add the first equation to the list
        if equalSignPassed:
            secondOperand.append(csvList[lines][column]) # add the second equation the list
    opertion1.append(firstOperand) # add the first equation to the list
    opertion2.append(secondOperand) # add the second equation the list
 
for i in range(len(opertion1)):
    num1 = float(opertion1[i][0])
    for j in range(len(opertion1[i])):
        if opertion1[i][j] == "+": # Check if addtion
            num1 += float(opertion1[i][j+1])
        if opertion1[i][j] == "-": # Check if subtraction
            num1 -= float(opertion1[i][j+1])
        if opertion1[i][j] == "*": # Check if multiplication
            num1 *= float(opertion1[i][j+1])
        if opertion1[i][j] == "/": # chech if divison
            num1 /= float(opertion1[i][j+1])
        if opertion1[i][j] == "**": # check if exponent
            num1 **= float(opertion1[i][j+1])
        if opertion1[i][j] == "%": # check if modulus
            num1 %= float(opertion1[i][j+1])
    num1 = round(num1, 0)
    num1 = int(num1) 
    outputFirst.append(num1) # append the answer to the output

for i in range(len(opertion2)): # repeat above for the second process
    num1 = float(opertion2[i][0])
    for j in range(len(opertion2[i])):
        if opertion2[i][j] == "+":
            num1 += float(opertion2[i][j+1])
        if opertion2[i][j] == "-":
            num1 -= float(opertion2[i][j+1])
        if opertion2[i][j] == "*":
            num1 *= float(opertion2[i][j+1])
        if opertion2[i][j] == "/":
            num1 /= float(opertion2[i][j+1])
        if opertion2[i][j] == "**":
            num1 **= float(opertion2[i][j+1])
        if opertion2[i][j] == "%":
            num1 %= float(opertion2[i][j+1])
    num1 = round(num1, 0)
    num1 = int(num1)
    outputSecond.append(num1)

for i in range(len(outputFirst)):
    if outputFirst[i] == outputSecond[i]: # Check if the two ouputs are the same
        checker = True
    else:
        checker = False
    finalList.append([outputFirst[i], outputSecond[i], checker]) # append to the final list

with open(outputFile, mode="w") as file:
    csvFile = csv.writer(file, delimiter=",")
    for i in finalList:
        csvFile.writerow(i) # write to the outpute file
