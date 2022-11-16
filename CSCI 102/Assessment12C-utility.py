# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 12
# References:
# Time:  minutes

def load_file(filename):
    lines = []
    with open(filename, mode="r") as file:
        for row in file:
            row = row.strip()
            lines.append(row)
    return lines

def update_string(str1, str2, pos):
    word = str1[0:pos] + str2 + str1[pos+1:]
    print("OUTPUT", word)

def find_word_count(list, word):
    count = 0
    for row in list:
        test = row.split()
        for i in test:
            if i == word:
                count += 1
    return count


players = ["Mary", "Cody", "Joe", "Jill", "Xai", "Bodo"]
scores = [5, 8, 10, 6, 10, 4]
score_finder(players, scores, "jill")


