x = input("Input: ").split()
n = 0
width = []
length = []
num = int(x[0])
for i in range(num):
    y = input("input2: ").split()
    width.append(y[0])
    length.append(y[1])

for i in range(len(width)):
    n += int(width[1]) * int(length[1])

print(n / int(x[1]))
