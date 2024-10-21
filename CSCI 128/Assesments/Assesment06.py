import random

seed = input("SEED> ")
random.seed(seed)
months = int(input("MONTHS> "))
interest = float(input("INTEREST> "))
method = input("METHOD> ")

starter_investment = 2000


if method == "1":
    total = 0
    for i in range(months):
        total = (total + starter_investment) * (interest + 1)
    print(f"OUtPUT {total:.2f}")

elif method == "2":
    total = 0
    for i in range(months):
        total = (total + starter_investment)
        probability = random.random()
        if probability >= 0.7 and probability < 0.9:
            total = total * 1.01
        elif probability > 0.9:
            total = total * 0.97
    print(f"OUTPUT {total:.2f}")

elif method == "3":
    total = 0
    months_left = months
    for i in range(months):
        number = random.randint(0, 36)
        if number == 15:
            total = 70000
            months_left = months - i
            break
    for i in range(months_left):
        total = total * (interest + 1)
    print(f"OUTPUT {total:.2f}")
