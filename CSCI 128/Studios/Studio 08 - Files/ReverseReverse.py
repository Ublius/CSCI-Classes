output = []

with open("text.txt", "r") as file:
    for line in file:
        # Reverse the word order in each line without using reversed function
        words = line.strip().split()
        reversed_line = ' '.join(words[::-1])
        output.append(reversed_line + "\n")
        

with open("text.txt", "w") as file:
    for line in reversed(output):
        file.write(line)
