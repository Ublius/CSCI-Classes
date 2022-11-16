#   Tristen Erpelding
#  ​ CSCI 102 – Section F 
#   Assessment 04A
#   References: 
#   Time: 20 minutes

# Get input on company name/city/state and the cashier name
CompName = str(input("Enter company name: "))
CompCS = str(input("Enter company city/state: "))
CashName = str(input("Enter cashier name: "))

# Get input on purchased items
Item1Name = str(input("Purchased item 1 name: "))
Item1Price = float(input("Purchased item 1 price: "))
Item2Name = str(input("Purchased item 2 name: "))
Item2Price = float(input("Purchased item 2 price: "))
Item3Name = str(input("Purchased item 3 name: "))
Item3Price = float(input("Purchased item 3 price: "))

# Get favorite closing message
ClosingMessage = str(input("Enter your favorite ending message: "))

# Calculate Total Cost
TotalCost = (Item1Price + Item2Price + Item3Price)

# Output
print("")
print("         RECEIPT GENERATOR")
print("-----------------------------------")
print(f"      {CompName}")
print(f"      {CompCS}")
print("===================================")
print(f"      Your cashier was: {CashName}")
print("      Welcome Valued Customer")
print("===================================")
print("      Item Name       Item Price")
print(f"      {Item1Name}       ${Item1Price:.2f}")
print(f"      {Item2Name}       ${Item2Price:.2f}")
print(f"      {Item3Name}       ${Item3Price:.2f}")
print("===================================")
print(f"      Total Cost of Order: {TotalCost:.2f}")
print("===================================")
print(f"      {ClosingMessage}")
print("-----------------------------------")