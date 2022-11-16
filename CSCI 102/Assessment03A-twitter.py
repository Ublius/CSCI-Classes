#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 03A
#   References: 
#   Time: 10 minutes

# List of abbreviations and their meaning
AbrivWords = {
    "LOL": "Laughing out loud",
    "BTW": "By the way",
    "IDK": "I don't know",
    "DM": "Direct message",
    "AFAIK": "As far as I know"
}

# Get user input
print("Enter the Tweet or Message abbreviation to decode.")
word = str(input("TWEET> "))

# Loop through Abriveated Words if in List print
for i in AbrivWords:
    if word == i:
        print(f'OUTPUT {i} = {AbrivWords[i]}')
        break
    

        
    
    