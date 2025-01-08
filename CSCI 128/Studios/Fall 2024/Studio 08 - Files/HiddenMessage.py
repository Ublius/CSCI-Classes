my_list = []
message = []
with open("secret_message.txt", "r") as file:
    for line in file:
        for char in line:
           my_list.append(char)
           
for i in range(0, len(my_list), 2):
    message.append(my_list[i])
    
print("".join(message))
