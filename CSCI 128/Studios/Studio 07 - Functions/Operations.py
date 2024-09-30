def operations(num_list, operation):
    lst = []
    for i in num_list:
        lst.append(eval(f"{i} {operation} {i}"))
    return lst
