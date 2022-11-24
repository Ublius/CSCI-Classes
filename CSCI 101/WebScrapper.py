import requests
import re
from bs4 import BeautifulSoup


def getTags(webpage):
    allTags = [tag.name for tag in webpage.find_all()]
    resultTags = []
    selectedNum = []
    selectedTags = []

    for item in allTags:
         if item not in resultTags:
             resultTags.append(item)

    print("Select which tags you would like to get data from?")

    for i ,j in enumerate(resultTags):
        if i not in selectedNum:
            print(i,j)
    
    chocieNum = input("CHOICE> ")
    if chocieNum.isdigit():
        selectedNum.append(int(chocieNum))

    while chocieNum.isdigit():
        for i ,j in enumerate(resultTags):
            if i not in selectedNum:
                print(i,j)
            else:
                selectedTags.append(j)

        chocieNum = input("Choice: ")
        if chocieNum.isdigit():
            selectedNum.append(int(chocieNum))
        else:
            break
    selectedTags = [*set(selectedTags)]
    print(f"The tags you have selected are: {selectedTags}")
    return selectedTags
    


URL = 'https://www.geeksforgeeks.org/'
# URL = input("Enter URL> ")
# class list set
class_list = set()
# Page content from Website URL
page = requests.get( URL )
# parse html content
soup = BeautifulSoup( page.content,"html.parser" )
# get all tags
# tags = {tag.name for tag in soup.find_all()}
getTags(soup)
# print(tags)
# iterate all tags
# for tag in tags:
# # find all element of tag
#     for i in soup.find_all( tag ):
#     # if tag has attribute of class
#         if i.has_attr( "class" ):
#             if len( i['class'] ) != 0:
#                 class_list.add(" ".join( i['class']))
# print( class_list )


# web_response = requests.get('https://www.geeksforgeeks.org/')
# # print("Response from web server : \n", web_response.text)
# soup = BeautifulSoup( web_response.text , 'html.parser')

# # # initializing tag
# # tag = "title"
# # # regex to extract required strings
# # # reg_str = "<" + tag + ">(.*?)</" + tag + ">"
# # reg_str = "<.*?>"
# # res = re.findall(reg_str, web_response.text)
# # # printing result
# # print("The Strings extracted : " + str(res))
# soup.find( class_ = "body" )