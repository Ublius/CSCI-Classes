# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 08A 
# References:
# Time: 30 minutes

import random

print("Input the seed for the random number generator: ")
seed = int(input("SEED> ")) # Get the seed for the random generator

print("Input the number of sides of the die: ")
sideOfDie = int(input("SIDES> ")) # Get how many sides are on the die

print("Input the number of rolls to make: ")
numnberRolls = int(input("ROLLS> ")) # Get the number of rolls to make

random.seed(seed)
rolls = [0] * sideOfDie # Create a list of the sides
for i in range(0,numnberRolls): # Loop through the rolls
    roll = int(random.randint(1,sideOfDie)) # random roll from 1-sides
    rolls[roll -1] += 1 # Add the roll to the list of rolls
    
print(f"Your {numnberRolls} rolls of a {sideOfDie}-sided die follow:")
for i in range(len(rolls)): # Loop through how many each side got
    print(f"OUTPUT {i+1} occurred {rolls[i]} times")
