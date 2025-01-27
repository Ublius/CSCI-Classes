name = input()
age =  int(input())

minutes = age * 365 * 24 * 60
seconds = (age + 2) * 365 * 24 * 60 * 60

print(f'{name}: Now: {minutes} mins old; Future: {seconds} secs old.')
