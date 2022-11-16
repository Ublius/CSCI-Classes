#   Tristen Erpelding
#  ​ CSCI 101 – Section H 
#   Python Lab 3
#   References: an article to check contained characters https://datascienceparichay.com/article/python-check-if-string-contains-only-certain-characters/
#   Time: 1 hour

row = str("")
allowedChar = "XEO"

size = int(input("SIZE> "))

# Get input based on the size of the board
for i in range(size):
    row += str(input(f"ROW{i}> "))
    
for i, char in enumerate(row):
    # Make sure there is not more entries then required
    if len(row) > (size**2):
        print("OUTPUT ERROR")
        break
    # Make sure the there is the right amount of entries
    if len(row) < (size**2):
        print("OUTPUT ERROR")
        break
    # Check to make sure there is only the allowed characters
    if all(char in allowedChar for char in row):
        # Diagonal Left-Right
        if row[i] == row[i+(size+1)] == row[i+(size+1)*2] and char != "E":
            print(f"OUTPUT {row[0]}")
            break
        # Column 1
        elif row[i] == row[size] == row[size*2] and char != "E":
            print(f"OUTPUT {row[0]}")
            break
        # Column 2
        elif row[i+1] == row[size+1] == row[(size*2)+1] and char != "E":
            print(f"OUTPUT {row[1]}")
            break
        # Column 3
        elif row[size-1] == row[size+2] == row[(size*2)+2] and char != "E":
            print(f"OUTPUT {row[2]}")
            break
        # row 1
        elif row[i] == row[size-2] == row[size-1] and char != "E":
            print(f"OUTPUT {row[0]}")
            break
        # row 2
        elif row[size] == row[size+1] == row[size-2] and char != "E":
            print(f"OUTPUT {row[3]}")
            break
        # row 3
        elif row[size*2] == row[(size*2)+1] == row[(size*2)+2] and char != "E":
            print(f"OUTPUT {row[6]}")
            break
        # Diagonal Right-Left
        if row[size-1] == row[size+1] == row[size*2] and char != "E":
            print(f"OUTPUT {row[0]}")
            break
        else:
            print("OUTPUT N")
            break
    else:
        print("OUTPUT ERROR")
        break
        
