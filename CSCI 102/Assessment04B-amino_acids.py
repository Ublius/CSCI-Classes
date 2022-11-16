#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 04B
#   References: 
#   Time: 20 minutes

# Define the different Amino Acids
aminoAcids = {
    "Alanine": "3C--7H--1N--2O--0S",
    "Arginine": "6C--14H--4N--2O--0S",
    "Asparagine": "4C--8H--2N--3O--0S",
    "Cysteine": "3C--7H--1N--2O--1S",
    "Histidine": "6C--9H--3N--2O--0S",
    "Glutamine": "5C--10H--2N--3O--0S"
}

# Get input on number of elements
carbon = int(input("CARBON> "))
hydrogen = int(input("HYDROGEN> "))
nitrogen = int(input("NITROGEN> "))
oxygen = int(input("OXYGEN> "))
sulfur = int(input("SULFUR> "))

# Combine the elements given
combinedElement = str(f"{carbon}C--{hydrogen}H--{nitrogen}N--{oxygen}O--{sulfur}S")

# Loop through table with all the Amino Acids and check if one is correct
for i in aminoAcids:
    if combinedElement == aminoAcids[i]:
        # If one is correct then OUTPUT
        print(f"The amino acid for {combinedElement} is {i}")
        print(f"OUTPUT {combinedElement}")
        print(f"OUTPUT {i}")
        break