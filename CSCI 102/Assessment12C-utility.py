# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 12
# References:
# Time:  minutes

def load_file(filename):
    lines = []
    with open(filename, mode="r") as file:
        fileReader = file.readline(file)
        for row in fileReader:
            lines.append(row)

    return lines


