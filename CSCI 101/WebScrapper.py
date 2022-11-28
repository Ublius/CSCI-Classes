import requests
from bs4 import BeautifulSoup
import csv

# Tags to remove at the start
pointlessTags = ["style", "script", "meta", "html", "i", "input", "svg", "br", "center", "nav", "header", "footer", "form", "path"]
# Header
headerList = ["Tag", "Content"]

def getTags(webpage):
    allTags = [tag.name for tag in webpage.find_all()]
    resultTags = []
    selectedNum = []
    selectedTags = []

    # Add all the tags read into a list
    for item in allTags:
         if item not in resultTags:
             resultTags.append(item)

    # Remove the pointless tags
    for i in resultTags:
        if i in pointlessTags:
            print(i)
            resultTags.remove(i)

    print("Select which tags you would like to get data from?")
    print("Enter nothing to continue")

    # Check if the tag is already selected
    for i ,j in enumerate(resultTags):
        if i not in selectedNum:
            print(i,j)
    
    # Add the index number to selected numbers
    chocieNum = input("CHOICE> ")
    if chocieNum.isdigit():
        selectedNum.append(int(chocieNum))

    while chocieNum.isdigit():
        for i ,j in enumerate(resultTags):
            if i not in selectedNum:
                print(i,j) # Print the tags not selected
            else:
                selectedTags.append(j) # Add the tag to selected tags

        chocieNum = input("Choice: ")
        if chocieNum.isdigit():
            selectedNum.append(int(chocieNum)) # Add the number to numbers
        else:
            break # Close the loop if selected nothing

    selectedTags = [*set(selectedTags)] # Remove duplicate tags
    print(f"The tags you have selected are: {selectedTags}")
    return selectedTags
    
def writeToCSV(tags, content):
    with open("scrapedData.csv", "w") as csvfile:  # Open the csv file
        csvwriter = csv.writer(csvfile, delimiter=",")
        # writing the fields 
        csvwriter.writerow(headerList)
        # writing the data rows 
        for i in tags:
            columns = [c.strip() for c in i.strip(', ').split(',')]
            csvwriter.writerow(columns,)


URL = 'https://www.geeksforgeeks.org/'
# URL = input("Enter URL> ")

# Page content from Website URL
page = requests.get( URL )
# parse html content
soup = BeautifulSoup(page.content, "html.parser")

# get all tags
selectedTags = getTags(soup)

# Get content from tags
content = soup.find_all(selectedTags)

# Write to csv
writeToCSV(selectedTags, content)