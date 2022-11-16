# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 08B
# References:
# Time: 30 minutes

print("Enter a DNA string S:")
s = input("S> ") # Get the DNA Sequence
print("Enter a substring T:")
t = input("T> ")  # Get the DNA substring

position= []
        
if len(t) > len(s): # If substring is greater then error
    print("ERROR: Substring is longer than DNA string")
    print("OUTPUT ERROR")
else: # Print the locations of the substring
    for i in range(0,len(s)):
        if s[i:i+len(t)] == t:
            position.append(i) # When find the substring add the index location to the list
    
    print(f"The total number of substrings found is {len(position)}")
    print(f"OUTPUT {len(position)}")
    print("The starting indices of these substrings in S are:",*position)
    print("OUTPUT",*position)