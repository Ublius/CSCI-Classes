# Tristen Erpelding
# CSCI 101 – H Sections
# Python Lab 5 
# References:
# Time: 150 minutes

rows = int(input("ROWS> "))
columns = int(input("COLUMNS> "))

subdivision = []
unpowered = []
pow = 0 # building has power

# Get the amount of the rows and columns and add it to the array 
for i in range(rows):
    row = input(f"ROW{i}> ")
    if sum([len(row) for row in row.split()]) == columns:
        test = row.rstrip() # remove any blank space at the end
        subdivision.append(test.split(" "))
    else:
        print(subdivision)

for i in range(len(subdivision)): # Loops through rows
    for j in range(len(subdivision[i])): # Loops through columns
        if subdivision[i][j] == "b": # if index is equal to "b"
            for k in range(-2,3): # Look 2 above and below
                for l in range(-2,3): # Look 2 right and left
                    if j+l >= 0 and j+l <= len(subdivision[i])-1 and i+k >= 0 and i+k <= len(subdivision)-1 and subdivision[i+k][j+l] == "S": # look for S
                        pow = 0 # set powered
                        break
                    elif j+l >= 0 and j+l <= len(subdivision[i])-1 and i+k >= 0 and i+k <= len(subdivision)-1 and k >= -1 and k <= 1 and l >= -1 and l <= 1 and subdivision[i+k][j+l] == "T": # look for T
                        pow = 0 # set powered
                        break
                    else: # not powered set to unpowered
                        pow = 1
                else: # if nothing then continue to nrxt index
                    continue
                break
            if pow == 1: # if its unpowered add to unpowered list
                unpowered.append([i,j])

# Check if there is any data in the list underpowered if nothing then true else print the underpowered
if any(unpowered):
    print("Not all buildings are powered!")
    print("OUTPUT False")
    print("The following buildings are not powered:")
    unpowered.sort() # Sort the underpowered list
    print(f"OUTPUT {unpowered}")
else:
    print("All buildings are powered!")
    print("OUTPUT True")
