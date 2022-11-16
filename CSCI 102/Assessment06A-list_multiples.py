#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 06A
#   References: 
#   Time: 5 minutes

# Print and get user values for calulating variables
print("Enter the number to create multiples for. ")
num = int(input("NUMBER> "))
print("Enter the maximum index of the list. ")
max_index = int(input("INDEX> "))

# Create the list of numbers to be added
numbers = []

# Loop the max_index number and append the list with the multiply
for i in range(max_index+1):
    numbers.append(num*(i+1))
 
# OUTPUT   
print("Your list of multiples is as follows: ")
print(f"OUTPUT {numbers}")
print("The first multiple calculated is: ")
print(f"OUTPUT {num}")