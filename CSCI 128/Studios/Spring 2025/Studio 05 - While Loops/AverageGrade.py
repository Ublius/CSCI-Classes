num = int(input())
sum = 0
counter = 0

while num != -1:
    sum += num
    counter += 1
    num = int(input())

average = sum / counter
print(f'Average: {average:.1f}%')
