def H(n, g, points, hours):
    H = [[(float('inf'), False)] * (g + 1) for _ in range(n + 1)]
    H[0][0] = (0, False)

    for i in range(1, n + 1):
        for j in range(g + 1):
            p = points[i - 1]
            h = hours[i - 1]
            if j >= p:
                a = H[i - 1][j - p][0] + h
                b = H[i - 1][j][0]
            else:
                a = h
                b = H[i - 1][j][0]
            min_val = min(a, b)
            if a == b or min_val == b:
                H[i][j] = (min_val, False)
            else:
                H[i][j] = (min_val, True)
        
    traceback = []
    i, j = n, g
    while i > 0 and j > 0:
        if H[i][j][1]:
            traceback.append(i)
            j -= points[i - 1]
        i -= 1

    traceback.reverse()
    return H[n][g][0], traceback

if __name__ == "__main__":
    # i, j = input().split()
    # test_sizes = [(5, 100), (10, 200), (15, 500), (20, 1000), (25, 2000), (30, 3000), (35, 4000), (40, 5000), (45, 6000), (50, 7000)]
    # points = list(map(int, input().split()))
    # hours = list(map(int, input().split()))

    # for (n, g) in test_sizes:
    #     start_time = time.time()
    #     min_hours = H(n, g, points, hours)
    #     end_time = time.time()

    #     if min_hours[0] != float('inf'):
    #         # print(f"n = {n}, g = {g}, result = {result}, Time taken: {end_time - start_time} seconds")
    #         print(f"{end_time-start_time}")
    #     elif sum(points) < int(g):
    #         print(f"{end_time-start_time}")
    #         # print(f"n = {n}, g = {g}, result = {-1}, Time taken: {end_time - start_time} seconds")
    #     else:
    #         print(f"{end_time-start_time}")
    #         # print(f"n = {n}, g = {g}, result = {-1}, Time taken: {end_time - start_time} seconds")

    n, g = input().split()
    g = int(g)
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    for i in range(5, 25):
        start_time = time.time()
        min_hours = H(i, g, points, hours)
        end_time = time.time()

        if min_hours[0]  != float('inf'):
            # print(f"n = {i}, result = {result}, Time taken: {end_time - start_time} seconds")
            print(f"{end_time-start_time}")
        elif sum(points) < g:
            print(f"{end_time-start_time}")
            # print(f"n = {i}, result = {-1}, Time taken: {end_time - start_time} seconds")
        else:
            print(f"{end_time-start_time}")
            # print(f"n = {i}, result = {-1}, Time taken: {end_time - start_time} seconds")



