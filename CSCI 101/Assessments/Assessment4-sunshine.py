# Tristen Erpelding
# CSCI 101 - H 
# Python Assessment 5
# References: 
# Time: 60 minutes

import csv
import os.path

# Make sure to follow the provided instructions for each of the steps on Canvas!

# Step 1: Month based stats
def monthly_summary(filename, month_abbr):
    data = []
    index = 0
    
    # Get the key potions and store them in a list
    with open(filename, 'r', encoding='utf-8') as file:
        fileReader = csv.DictReader(file)
        dict_from_csv = dict(list(fileReader)[0])
        list_of_column_names = list(dict_from_csv.keys()) 
    
    # Get the value at the key position
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        header = next(csv_reader)
        for i in range(len(list_of_column_names)):
            if list_of_column_names[i] == month_abbr: # Find which key index equals the month
                index = i
        if header != None: # Skip the header position
            for line in csv_reader:
                data.append(float(line[index]))
    
    # Perform Calculations
    average = sum(data) / len(data)
    print(f"OUTPUT Average: {average:.0f} | Minimum: {min(data):.0f} | Maximum: {max(data):.0f}")
    
# Step 2: City based stats
def city_summary(filename, city, country):
    predata = []
    data = []

    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            for column in range(len(row)):
                if row[column] == country: # if column is the country
                    if row[column+1] == city: # if next column is city
                        predata.append(row) # add the row to a list
    
    predata = predata[0] # make the list not a nested list
    predata.pop(0) # remove country
    predata.pop(0) # remove city
    for i in predata:
        data.append(float(i)) # make the valyes floats
    
    # Perform Calculations
    average = sum(data) / len(data)
    print(f"OUTPUT Average: {average:.0f} | Minimum: {min(data):.0f} | Maximum: {max(data):.0f}")

# Step 3: Sunshine threshold
def cities_in_range(filename, minimum, maximum):        
    predata = []
    data = []
    positon = []
    csvdata = []
    locations = []
    
    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        header = next(csv_reader)
        if header != None: # Skip the header position
            for row in csv_reader:
                for column in range(len(row)):
                    predata.append(row[column+2:]) # Add all the temperatures to a list
                    break
    for row in range(len(predata)):
        for col in range(len(predata[row])):
            predata[row][col] = float(predata[row][col]) # Change the str to floats
                                      
    for row in predata:
        data.append(sum(row)) # Add the yearly_sum to a list
        
    for i in range(len(data)):
        if maximum == None and maximum == None:
            positon.append(i)
        elif minimum == None and maximum != None:
            if data[i] <= float(maximum):
                positon.append(i)
        elif maximum == None and minimum != None:
            if data[i] >= float(minimum):
                positon.append(i)
        else:
            if data[i] >= float(minimum) and data[i] <= float(maximum):
                positon.append(i)

    with open(filename, 'r', encoding='utf-8') as file:
        csv_reader = csv.reader(file)
        header = next(csv_reader)
        if header != None: # Skip the header position
            for row in csv_reader:
                csvdata.append(row) # Add all the csv data to a list
    
    for i in positon:
        word = csvdata[i][1]
        word = word.split()
        test = ", ".join(word)
        test = test +  ", " + csvdata[i][0] # Join the city and the country
        locations.append(test) # Add the location to the list of locations

    # locations = [*set(locations)]
    print("OUTPUT", locations)
    return locations
          
# Extra credit: Validating your input file
def valid_csv(filename):
    ext = os.path.splitext(filename)[-1].lower()
    if os.path.exists(filename) == True:
        if ext != ".csv":
            return 2
        else:
            return 0
    if os.path.exists(filename) == False:
        if ext != ".csv":
            return 3
        else:
            return 1

# Step 4: Tying it all together
if __name__ == "__main__":
    print("Here comes the sun!")
    print("Provide the name of your csv file containing sunshine duration data")
    filename = input("FILENAME> ")
    print("--------------------------------------------")
    print("Which of the following would you like to do? \n(1) See the stats for a specific month \n(2) See the stats for a specific city \n(3) Find the cities that fit my sunshine preference")
    print("--------------------------------------------")
    choice = input("CHOICE> ")

    while choice.isdigit():
        choice = int(choice)
        if choice == 1:
            print("Enter the month abbreviation")
            month = input("MONTH> ")
            monthly_summary(filename, month)
            print("--------------------------------------------")
        elif choice == 2:
            print("Enter the city's name")
            city = input("CITY> ")
            print("Enter the country's name")
            country = input("COUNTRY> ")
            city_summary(filename, city, country)
            print("--------------------------------------------")
        elif choice == 3:
            print("Enter the minimum amount (in hours) of yearly sunshine")
            minimum = input("MIN> ")
            print("Enter the maximum amount (in hours) of yearly sunshine")
            maximum = input("MAX> ")
            locations = cities_in_range(filename, minimum, maximum)
        else:
            print("Goodbye!")
            break
        choice = input("CHOICE> ")
    print("Goodbye!")



