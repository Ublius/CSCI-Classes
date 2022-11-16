#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 01B
#   References: 
#   Time: 20 minutes

# Convert the yard given and return the feet
def YardToFeet(Length):
    return Length * 3

# Calculate the Area of the supplied yards and return the amount
def CalculateArea(Length):
    return round((Length / 4) *(Length / 4), 1)

# Get yard length
print("Input the total length of fencing available (in yards).")
YardLength = int(input("LENGTH> "))

# Calculate feet and area
FeetLength = YardToFeet(YardLength)
TotalArea = CalculateArea(FeetLength)

# OUTPUT
print("The maximum rectangular area that can be bound (in feet) is: ")
print("OUTPUT ", TotalArea)




