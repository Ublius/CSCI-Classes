#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 05B
#   References: 
#   Time: 15 minutes


# Get the amount for empty,found, and cost
print("Enter the number of empty bottles in Blaster's possession at the start of the day.")
empty = int(input("EMPTIES> "))
print("Enter the number of empty bottles that Blaster found during the day.")
found = int(input("FOUND> "))
print("Enter the number of empty soda bottles required to buy a new soda.")
cost = int(input("COST> "))

# Do the calulation for getting the total amount blaster drank
gathered = empty + found
total = gathered // cost
newdrank = total // cost
leftover = total - (newdrank*cost)
total = total + newdrank
newdrank = ((total // cost) + leftover) // cost
total = total + newdrank

# OUTPUT
print("The total number of sodas that Blaster drank is:")
print(f"OUTPUT {total:.0f}")
