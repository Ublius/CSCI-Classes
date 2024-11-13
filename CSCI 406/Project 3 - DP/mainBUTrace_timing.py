import time

def H(n, g, points, hours):
    H = [[(float('inf'), ())] * (g + 1) for _ in range(n + 1)]

    for i in range(n + 1):
        H[i][0] = (0, ())
    
    for i in range(1, n + 1):
        p = points[i - 1]
        h = hours[i - 1]
        for j in range(1, g + 1):
            H[i][j] = (float('inf'), ())

            if j >= p:
                min_value = min(H[i-1][j-p][0] + h, H[i-1][j][0])
                H[i][j] = (min_value, H[i][j][1])

                if H[i-1][j-p][0] + h == H[i-1][j][0]:
                    if len(H[i - 1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i][j][1] + H[i - 1][j][1])
                elif H[i][j][0] == H[i-1][j-p][0] + h:
                    if len(H[i - 1][j - p]) > 1:
                        H[i][j] = (H[i][j][0], H[i][j][1] + H[i - 1][j - p][1])
                    H[i][j] = (H[i][j][0], H[i][j][1] + (i,))
                else:
                    if len(H[i-1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i][j][1] + H[i-1][j][1])
            else:
                H[i][j] = (min(h, H[i-1][j][0]), H[i][j][1])

                if H[i][j][0] == h:
                    H[i][j] = (H[i][j][0], H[i][j][1] + (i,))
                else:
                    if len(H[i-1][j]) > 1:
                        H[i][j] = (H[i][j][0], H[i][j][1] + H[i-1][j][1])

    return H[n][g]

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



