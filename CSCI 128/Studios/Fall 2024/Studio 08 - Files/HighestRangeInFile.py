import string

with open("scores.txt", "r") as file:
    low = 10
    high = 0
    for line in file:
        if not(line == "\n"):
            if float(line) > high:
                high = float(line)
            if float(line) < low:
                low = float(line)
        # else:
        #     low = float(line)

# print(high, low)

diff = float(high - low)
if diff > 0.0:
    print(f"{diff:.1f}")
else:
    print(0.0)
