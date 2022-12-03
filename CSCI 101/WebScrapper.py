import requests
from bs4 import BeautifulSoup
import csv

# Tags to remove at the start
pointlessTags = ['a', 'b', 'i', 'u', "style", "script", "meta", "html", "i", "input", "svg", "br", "center", "nav", "header", "footer", "form", "path"]
# Header for the csv file
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
        print("Select which tags you would like to get data from?")
        print("Enter nothing to continue")
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
    
def writeToCSV(combinedData, selectedTags, filename):
    with open(filename, "w") as csvfile:  # Open the csv file
        csvwriter = csv.DictWriter(csvfile, headerList)
        csvwriter.writeheader() # write the header
        
        index = 0
        
        for data in range(len(combinedData)):
            if combinedData[index-1][0] != combinedData[index][0]:
                index = 0 # Reset to 0 after the tag changes to a new tag

            for col in range(len(combinedData[data])):
                if combinedData[data][col] in selectedTags:
                    tag = (f"{combinedData[data][col]}_{index}") # Convert the tag to string in case of edits
                    contentData = (f"{combinedData[data][col+1]}") # Convert content to string in case of edits
                    csvwriter.writerow({'Tag': tag, 'Content': contentData}) # Write the data to csv
                    index += 1 # Increase index for how many of each tag there are 


def combineTagData(data):
    combinedData = []
    for tag in data:
        combinedData.append([tag.name, tag.text]) # Make combined list with tag and content associated with tag
    return combinedData

# Get User Defined URL to parse data
# URL = 'https://www.geeksforgeeks.org/'
URL = input("Enter URL> ")

# Page content from Website URL
page = requests.get( URL )
# parse html content
soup = BeautifulSoup(page.content, "html.parser")

# get all tags
selectedTags = getTags(soup)

# Get content from tags
content = soup.find_all(selectedTags)

# Combine the Tags with there associated Data
combinedData = combineTagData(content)

# Get user defined filename to export data to
# filename = "scrapedData.csv"
print("Enter the filename you wish to export the data to (include .csv)")
filename = input("Enter Filename> ")
            
# Write to csv
writeToCSV(combinedData, selectedTags, filename)

print("Thank you for using the Modular HTML Scrapper! \nPlease you again :)")