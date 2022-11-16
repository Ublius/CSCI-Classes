# Tristen Erpelding
# CSCI 102 – F Sections
# Assessment 10
# References:
# Time: 90 minutes

import csv

start  = []
end = []
depth = []
diff = []
form = []

with open("formations.csv", mode="r") as file:
    csvFile = csv.reader(file)
    for lines in csvFile:
        form.append(lines[1])
        for columns in lines:
                if "-" in columns: # get the float vlaues
                    num = columns.split("-") # store those values in a list
                    for row in range(len(num)):
                        float1 = float(num[0])
                        start.append(float1) # Store the start depth in a list
                        float2 = float(num[1])
                        end.append(float2) # Store the end depth in a list
                        depth.append(str(f"{num[0]}-{num[1]}")) # Store the starting depth
                        difference = float2-float1 # get the difference of the floats
                        diff.append(f"{difference:.2f}") # store the difference in a list
                        break
    form.pop(0) # Remove the header entry

with open("formations_parsed.csv", mode="w") as writeFile:
    fieldnames = ["Depth","Start Depth","End Depth","Difference in Depth","Formation","Age of Formation"] # Set the first row to which each column means
    writer = csv.DictWriter(writeFile, fieldnames) # write to the file
    writer.writeheader() # write the header

    for i in range(len(form)): # loop through all the formations
        if start[i] < 700: # Check the depth
            writer.writerow({'Depth': depth[i], 'Start Depth': start[i], 'End Depth': end[i], 'Difference in Depth': diff[i], 'Formation': form[i],'Age of Formation': 'Paleocene'}) # write the vlaues to the difference column
        else:
             writer.writerow({'Depth': depth[i], 'Start Depth': start[i], 'End Depth': end[i], 'Difference in Depth': diff[i], 'Formation': form[i],'Age of Formation': 'Cretaceous'}) # write the vlaues to the difference column