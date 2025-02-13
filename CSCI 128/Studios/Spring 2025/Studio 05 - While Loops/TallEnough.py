h = int(input())
result = True
while h != 0:
    if h < 48:
        result = False
    h = int(input())

print(result)
