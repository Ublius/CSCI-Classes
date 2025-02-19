empty = int(input())
found = int(input())
cost = int(input())

final = 0
total = empty + found

# The loop subtracts the amount of bottles he has to turn in
while (total >= cost):
    total -= cost
    total += 1 # adds empty bottle
    final += 1

# OUTPUT
print(final)
