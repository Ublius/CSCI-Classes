#Tristen Erpelding - terpelding@mines.edu
# Ava Courtney - acourtney@mines.edu
# Josh Kim - joshuakim@mines.edu

#Get user inputed string
print("Enter your string: ")
InputedString = str(input("STRING> "))

#Get user numbers to slice
print("Enter four numbers to slice the string")
A = int(input("A> "))
B = int(input("B> "))
C = int(input("C> "))
D = int(input("D> "))

#Print sliced String
# Add +1 to the index because when indexing a string it starts at 0
print(f"OUTPUT {InputedString[A+1:B]} {InputedString[C+1:D]}")