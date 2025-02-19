num = float(input())
inital = 1
better = 0

while True:
    better = (inital + num / inital) / 2
    if inital == better:
        break
    inital = better
print(f"{better:.2f}")
