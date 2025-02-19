num = int(input())
steps = 0

while num != 1:
    if num % 2 == 1:
        num = (3 * num) + 1
    else:
        num /= 2
    steps += 1
print(steps)
