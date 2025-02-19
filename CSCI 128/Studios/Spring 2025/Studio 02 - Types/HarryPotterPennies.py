money = int(input())

dollars = money // 100
quaters = (money - (dollars * 100)) // 25
dimes = (money - (dollars * 100) - (quaters * 25)) // 10
nickles = (money - (dollars * 100) - (quaters * 25) - (dimes * 10)) // 5
pennies = (money - (dollars * 100) - (quaters * 25) - (dimes * 10) - (nickles * 5))

# print(dollars, quaters, dimes, nickles, pennies)
lst = f'{dollars} dollars,{quaters} quarters,{dimes} dimes,{nickles} nickels,{pennies} pennies'.split(',')
print(lst)
