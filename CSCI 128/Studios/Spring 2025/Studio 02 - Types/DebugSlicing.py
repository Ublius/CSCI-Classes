string1 = input()
string2 = input()

# Print the first letter of string1
print(string1[0])

# Print the last letter of string1
print(string1[-1])

# Print string1 back to back with no spaces in between.
print(string1[::] * 2)

# Print the length of string1 divided by 2 (rounding down to the nearest integer)
print(len(string1) // 2)

# The sum of the first and last number in string2
print(int(string2[0]) + int(string2[-1]))

# Everything except the first and last numbers in string2
print(string2[1:-1])

# The second through fifth numbers in string2
print(string2[1:5])
