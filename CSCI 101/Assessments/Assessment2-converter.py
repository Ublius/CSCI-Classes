#   Tristen Erpelding
#   CSCI 101 – Section H
#   Python Assessment 3
#   References: looked up how to convert decimal to binary because I forgot how to do it then when I was scrolling I found an easy python way to do it, also looked up how to enumerate a str backwards
#   Time: 50 minutes 

print("Welcome to the Binary-Decimal-Hex Converter")
print("=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=")

# Chosen option
option = int(input("Enter an option:\n1. Binary-Decimal Conversion\n2. Decimal-Binary Conversion\n3. Hexadecimal-Decimal Conversion\n4. Decimal-Hexadecimal Conversion\nOPTION> "))

# Variables
allowedChar = ""
total = 0
totalHex = ""

# Conversion Table for converting a decimal into Hex
conversion_table = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A' , 'B', 'C', 'D', 'E', 'F']

# Settings changed based on the option the user chose
if option == 1:
    userinput = str(input("BINARY-STR> "))
    allowedChar = "01"
    
if option == 2:
    userinput = str(input("DECIMAL-STR> "))
    allowedChar = "0123456789"

if option == 3:
    userinput = str(input("HEX-STR> "))
    allowedChar = "0123456789ABCDEF"
    
if option == 4:
    userinput = str(input("DECIMAL-STR> "))
    allowedChar = "0123456789"
  
# Make sure the user dosent input any funny business  
if all(char in allowedChar for char in userinput):
    if option == 1: # Option 1
        for i, char in zip(reversed(range(len(userinput))), userinput):
            total = total + (int(char) * (2**i))
        print(f"Binary {userinput} is Decimal {total}")
        print(f"OUTPUT {total}")
    elif option == 2: # Option 2
        total = bin(int(userinput))[2:]
        print(f"Decimal {userinput} is Binary {total}")
        print(f"OUTPUT {total}")
    elif option == 3: # Option 3
        for i, char in zip(reversed(range(len(userinput))), userinput):
            if char == "A":
                total += (10 * (16**i))
            elif char == "B":
                total += 11 * (16**i)
            elif char == "C":
                total += 12 * (16**i)
            elif char == "D":
                total += 13 * (16**i)
            elif char == "E":
                total += 14 * (16**i)
            elif char == "F":
                total += 15 * (16**i)
            else:
                total = total + (int(char) * (16**i))        
        print(f"Hexadecimal {userinput} is Decimal {total}")
        print(f"OUTPUT {total}")
    elif option == 4: # Option 4
        decimal = int(userinput)
        while(decimal > 0):
            remainder = decimal % 16
            totalHex = conversion_table[remainder] + totalHex
            decimal = decimal // 16
        print(f"Decimal {userinput} is Hexadecimal {totalHex}")
        print(f"OUTPUT {totalHex}")
else:
    print("OUTPUT ERROR")
    

            