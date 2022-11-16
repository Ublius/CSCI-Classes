#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 02B
#   References: 
#   Time: 10 minutes


#Get user inputed string
print("Enter your string: ")
InputedString = str(input("STRING> "))

#Get user numbers to slice
print("Enter four numbers to slice the string")
A = int(input("A> "))
B = int(input("B> "))
C = int(input("C> "))
D = int(input("D> "))

#Slice the inputed string
str1 = InputedString[A+1:B]
str2 = InputedString[C+1:D]

#Print sliced String
print(f"OUTPUT {str1} {str2}")
