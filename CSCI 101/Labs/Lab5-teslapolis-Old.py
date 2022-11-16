# Tristen Erpelding
# CSCI 101 – H Sections
# Python Lab 4 
# References:
# Time: 120 minutes

rows = int(input("ROWS> "))
columns = int(input("COLUMNS> "))

subdivision = []
unpowered = []

# Get the amount of the rows and columns and add it to the array 
for i in range(rows):
    row = input(f"ROW{i}> ")
    if sum([len(row) for row in row.split()]) == columns:
        subdivision.append(row.split(" "))
    else:
        print(subdivision)
        
for row,sublist in enumerate(subdivision):
    for column,item in enumerate(sublist):
        if item == 't' or item == "T": # Find Normal charge
            if not(column == len(sublist)-1): # Not the first index
                if not(subdivision[row][column+1] == "S"):
                    if not(subdivision[row][column+1] == "T"):
                        subdivision[row][column+1] = 'r' # right
            if not(row == 0): # Not in the top row
                if not(subdivision[row-1][column] == "S"):
                    if not(subdivision[row-1][column] == "T"):
                        subdivision[row-1][column] = 'r' # above
                if not(column == len(sublist)-1): # Not the last index
                    if not(subdivision[row-1][column+1] == "S"):
                        if not(subdivision[row-1][column+1] == "T"):
                            subdivision[row-1][column+1] = 'r' # above right diagonal
                if not(column == 0): # Not the first index
                    if not(subdivision[row-1][column-1] == "S"):
                        if not(subdivision[row-1][column-1] == "T"):
                            subdivision[row-1][column-1] = 'r' # above left diagonal
            if not(row == len(subdivision)-1): # not last row
                if not(subdivision[row+1][column] == "S"):
                        if not(subdivision[row+1][column] == "T"):
                            subdivision[row+1][column] = 'r' # below
                if not(column == len(sublist)-1): # Not last index
                    if not(subdivision[row+1][column+1] == "S"):
                        if not(subdivision[row+1][column+1] == "T"):
                            subdivision[row+1][column+1] = 'r' # below right diagonal
                if not(column == 0): # Not the first index
                    if not(subdivision[row+1][column-1] == "S"):
                        if not(subdivision[row+1][column-1] == "T"):
                            subdivision[row+1][column-1] = 'r' # below left diagonal
            if not(column == 0): # Not the first index
                if not(subdivision[row][column-1] == "S"):
                        if not(subdivision[row][column-1] == "T"):
                            subdivision[row][column-1] = 'r' # left
        if item == 's' or item == "S": # Find Super charge
            if not(column == len(sublist)-1): # not last index
                if not(subdivision[row][column+1] == "S"):
                        if not(subdivision[row][column+1] == "T"):
                            subdivision[row][column+1] = 'r' # right
                if not(column == len(sublist)-2): # not second last index
                    if not(subdivision[row][column+2] == "S"):
                        if not(subdivision[row][column+2] == "T"):
                            subdivision[row][column+2] = 'r' # right +2
            if not(row == 0): # Not first row
                if not(subdivision[row-1][column] == "S"):
                        if not(subdivision[row-1][column] == "T"):
                            subdivision[row-1][column] = 'r' # above
                if not(row == 1): # Not second row 
                    if not(subdivision[row-2][column] == "S"):
                        if not(subdivision[row-2][column] == "T"):
                            subdivision[row-2][column] = 'r' # above +2
                if not(column == len(sublist)-1): # not last index
                    if not(subdivision[row-1][column+1] == "S"):
                        if not(subdivision[row-1][column+1] == "T"):
                            subdivision[row-1][column+1] = 'r' # above right diagonal
                    if not(column == len(sublist)-2): # Not second last index
                        if not(subdivision[row-2][column+2] == "S"):
                            if not(subdivision[row-2][column+2] == "T"):
                                subdivision[row-2][column+2] = 'r' # above right diagonal +2
                if not(column == 0): # Not the first index
                    if not(subdivision[row-1][column-1] == "S"):
                        if not(subdivision[row-1][column-1] == "T"):
                            subdivision[row-1][column-1] = 'r' # above left diagonal
                    if not(column == 1): # Not the second index
                        if not(subdivision[row-2][column-2] == "S"):
                            if not(subdivision[row-2][column-2] == "T"):
                                subdivision[row-2][column-2] = 'r' # above left diagonal +2
            if not(row == len(subdivision)-1): # Not the last row
                if not(subdivision[row+1][column] == "S"):
                    if not(subdivision[row+1][column] == "T"):
                        subdivision[row+1][column] = 'r' # below
                if not(row == len(subdivision)-2): # Not the second last row
                    if not(subdivision[row+2][column] == "S"):
                        if not(subdivision[row+2][column] == "T"):
                            subdivision[row+2][column] = 'r' # below +2
                if not(column == len(sublist)-1): # Not the last index
                    if not(subdivision[row+1][column+1] == "S"):
                        if not(subdivision[row+1][column+1] == "T"):
                            subdivision[row+1][column+1] = 'r' # below right diagonal
                    if not(row == len(subdivision)-2): # Not the second  last row
                        if not(column == len(sublist)-2): # Not the second last index
                            if not(subdivision[row+2][column+2] == "T"):
                                if not(subdivision[row+2][column+2] == "S"):
                                    subdivision[row+2][column+2] = 'r' # below right diagonal +2  
                if not(column == 0): # Not the first index
                    if not(subdivision[row+1][column-1] == "S"):
                        if not(subdivision[row+1][column-1] == "T"):
                            subdivision[row+1][column-1] = 'r' # below left diagonal
                    if not(row == len(subdivision)-2): # Not the second  last row
                        if not(column == 1): # Not the second index
                            if not(subdivision[row+2][column-2] == "S"):
                                if not(subdivision[row+2][column-2] == "T"):
                                    subdivision[row+2][column-2] = 'r' # below left diagonal +2
            if not(column == 0): # Not the first index
                if not(subdivision[row][column-1] == "S"):
                        if not(subdivision[row][column-1] == "T"):
                            subdivision[row][column-1] = 'r' # left
                if not(column == 1): # Not the second index
                    if not(subdivision[row][column-2] == "S"):
                        if not(subdivision[row][column-2] == "T"):
                            subdivision[row][column-2] = 'r' # left +2


for row,sublist in enumerate(subdivision):
    for column,item in enumerate(sublist):
        if item == 'b':# check if there are any "b"'s can be powered by those around it
            if row == 0: # First Row
                if column == 0: # First Column
                    if (subdivision[row][column+1] == 'r') and (subdivision[row+1][column+1] == 'r') and (subdivision[row+1][column] == 'r'):
                       subdivision[row][column] = 'r'
                elif column == len(sublist)-1: # Last Column
                    if (subdivision[row][column-1] == 'r') and (subdivision[row-1][column-1] == 'r') and (subdivision[row+1][column] == 'r'):
                       subdivision[row][column] = 'r'
                else:
                    if (subdivision[row][column-1] == 'r') and (subdivision[row+1][column-1] == 'r') and (subdivision[row+1][column] == 'r') and (subdivision[row][column+1] == 'r') and (subdivision[row+1][column+1] == 'r'):
                       subdivision[row][column] = 'r'
            if row == len(subdivision)-1: # Last Row
                if column == len(sublist)-1: # Last Column
                    if (subdivision[row][column-1] == 'r') and (subdivision[row-1][column-1] == 'r') and (subdivision[row-1][column] == 'r'):
                       subdivision[row][column] = 'r'
                elif column == 0: # First Column
                    if (subdivision[row][column+1] == 'r') and (subdivision[row-1][column+1] == 'r') and (subdivision[row-1][column] == 'r'):
                       subdivision[row][column] = 'r'
                else:
                    if (subdivision[row][column-1] == 'r') and (subdivision[row-1][column-1] == 'r') and (subdivision[row-1][column] == 'r') and (subdivision[row][column+1] == 'r') and (subdivision[row-1][column+1] == 'r'):
                       subdivision[row][column] = 'r'
            if not(row == 0): # Not First row 
                if not(row == len(subdivision)-1): #  Not Last Row
                    # if column == 0: # First Index Only while not being first row or last row
            #           if (subdivision[row][column+1] == 'r') and (subdivision[row+1][column+1] == 'r') and (subdivision[row+1][column] == 'r') and (subdivision[row-1][column] == 'r') and (subdivision[row-1][column+1] == 'r'):
            #               subdivision[row][column] = 'r'
                    if column == len(sublist)-1: # Last index not first row or last row
                        if (subdivision[row][column-1] == 'r') and (subdivision[row-1][column-1] == 'r') and (subdivision[row-1][column] == 'r') and (subdivision[row+1][column-1] == 'r') and (subdivision[row+1][column] == 'r'):
                            subdivision[row][column] = 'r'
                    else:
                        if (subdivision[row][column-1] == 'r') and (subdivision[row+1][column-1] == 'r') and (subdivision[row+1][column] == 'r') and (subdivision[row+1][column+1] == 'r') and (subdivision[row][column+1] == 'r'):
                            subdivision[row][column] = 'r'

# If their is any "b" then get its location and add to underpowered
for row,sublist in enumerate(subdivision):
    for column,item in enumerate(sublist):
        if subdivision[row][column] == 'b':
            unpowered.append([row,column])
    
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
    