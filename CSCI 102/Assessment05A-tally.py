#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 05A
#   References: 
#   Time: 15 minutes

# Variables Initalized
number = str("")
totalAmountofNumbers = 0
total = 0

# Print to tell the user what to do
print("Enter values to add. Enter quit when done.")

# Loop through till the user enteres quit
while number != "quit":
    number = input("NUMBER> ")
    if number == "quit": # if user enters quit then break the loop
        break
    total += int(number) # add the numbers
    totalAmountofNumbers += 1 # show how many time the loop iterated

# OUTPUT
print(f"The addition of the {totalAmountofNumbers} numbers entered is:")
print(f"OUTPUT {totalAmountofNumbers} numbers")
print(f"OUTPUT {total} total")