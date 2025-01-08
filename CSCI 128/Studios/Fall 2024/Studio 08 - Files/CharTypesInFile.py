import string


with open ("textfile.txt", "r") as file:
    lettrs = 0
    digits = 0
    punc = 0
    for line in file:
        for word in line:
            for char in word:
                if char in string.punctuation:
                    punc += 1
                elif char in string.digits:
                    digits += 1
                elif char in string.ascii_letters:
                    lettrs += 1

print(f"There are {lettrs} letter(s), {digits} digit(s), and {punc} punctuation(s) in the file.")
