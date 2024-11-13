def H(n, g, points, hours):
    # Initialize DP table where each cell stores a tuple: (time, parent_index)
    H = [[(float('inf'), -1)] * (g + 1) for _ in range(n + 1)]

    # Base case: zero points require zero hours and no tasks
    for i in range(n + 1):
        H[i][0] = (0, 0)
    
    # Fill the DP table based on the algorithm you provided
    for i in range(1, n + 1):
        p = points[i - 1]
        h = hours[i - 1]
        for j in range(1, g + 1):
            # Start with excluding the current task
            H[i][j] = (float('inf'), -1)

            if j >= p:
                min_value = min(H[i-1][j-p][0] + h, H[i-1][j][0])
                H[i][j] = (min_value, H[i][j][1])

                # If the result is equal, choose the path with the parent
                if H[i-1][j-p][0] + h == H[i-1][j][0]:
                    if len(H[i - 1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i - 1][j][1])
                elif H[i][j][0] == H[i-1][j-p][0] + h:
                    if len(H[i - 1][j - p]) > 1:
                        H[i][j] = (H[i][j][0], H[i - 1][j - p][1])
                    H[i][j] = (H[i][j][0], i)
                else:
                    if len(H[i-1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i-1][j][1])
            else:
                H[i][j] = (min(h, H[i-1][j][0]), H[i][j][1])

                if H[i][j][0] == h:
                    H[i][j] = (H[i][j][0], i)
                else:
                    if len(H[i-1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i-1][j][1])


    # # Perform traceback to get the selected tasks (parents)
    parents = []
    i, j = n, g
    while j > 0 and i > 0:
        parent = H[i][j][1]
        print(f"Current (i={i}, j={j}) => Parent: {parent}")
        if parent != -1:  # If a parent exists
            if parent != i - 1:  # Task i was selected
                parents.append(parent)  # Append the parent number
                print(f"Selected Parent: {parent}")
                j -= points[parent - 1]  # Subtract the points of the task we just included
                # print(points[parent - 1])
                # print(j)
            else:
                i -= 1  # Move to the parent if no task was selected
        else:
            break  # No parent found, end of traceback

    parents.reverse()  # Reverse the parent list to get them in the correct order
    
    return H[n][g][0], parents


if __name__ == "__main__":
    n, g = map(int, input().split())
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    print(H(n, g, points, hours))
    # min_hours = H(n, g, points, hours)
    # if min_hours[0] != float('inf'):
    #     print(min_hours[0])
    #     print(len(min_hours[1]))
    #     print(" ".join(map(str, min_hours[1])))
    # elif sum(points) < g:
    #     print(-1)
    # else:
    #     print(-1)

