#Write a function that takes in a list of integers and an operator ("+" or "*"). Then, for every number in the list, perform that operation on itself. The function must return a new list that contains the resulting numbers from each operation.

def operations(num_list, operation):
    lst = []
    for i in num_list:
        lst.append(eval(f"{i} {operation} {i}"))
    return lst
