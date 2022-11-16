#Tristen Erpelding - terpelding@mines.edu
#Thomas Fischer - tfischer1@mines.edu
#Callen Hacker - callenhacker@mines.edu

# Get the dimensions for how many rows and columns
row = int(input("ROWS> "))
col = int(input("COLUMNS> "))

# Get which characters to use to build the board
first = str(input("FIRST> "))
second = str(input("SECOND> "))

board = []

for i in range(row): # Loop the rows
    array = []
    if i % 2 == 0: # If even number row start with first 
        array.append(first)
    else: # If odd number row start with second 
        array.append(second)
    for j in range(1,col): # Loop through the columns
        if array[j-1] == first: # ensures pattern alternates
            array.append(second)
        else:
            array.append(first)
    board.append(array) # Adds to the board
    print(f"OUTPUT {array}")

print(f"OUTPUT {board}")