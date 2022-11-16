# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 07 
# References:
# Time: 30 minutes

# Get the dimensions for how many rows and columns
row = int(input("ROWS> "))
col = int(input("COLUMNS> "))

# Get which characters to use to build the board
first = str(input("FIRST> "))
second = str(input("SECOND> "))

rows = []
test = 0

# Loop the rows
for x in range(row):
    columns = []
    # Loop through the columns
    for y in range(col):
        if test%2 == 0: # alternate every other row
            columns.append(first)
            columns.append(second)
        else:
            columns.append(second)
            columns.append(first)
    for j in range(0, len(columns)-col): # remove extra in the columns
        columns.pop()
    print(f"OUTPUT {columns}")
    rows.append(columns) # add to the board
    test += 1

# OUTPUT
print(f"OUTPUT {rows}")