#Tristen Erpelding - terpelding@mines.edu
#Thomas Fischer - tfischer1@mines.edu

# Get the amount for empty,found, and cost
print("Enter the number of empty bottles in Blaster's possession at the start of the day.")
empty = int(input("EMPTIES> "))
print("Enter the number of empty bottles that Blaster found during the day.")
found = int(input("FOUND> "))
print("Enter the number of empty soda bottles required to buy a new soda.")
cost = int(input("COST> "))

final = 0
total = empty + found

# The loop subtracts the amount of bottles he has to turn in
while (total >= cost):
    total -= cost
    total += 1 # adds empty bottle
    final += 1

# OUTPUT
print(f"OUTPUT {final}")

