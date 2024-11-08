import time

def H(n, g, points, hours):
    H = [[float('inf')] * (g + 1) for _ in range(n + 1)]
    
    for i in range(n + 1):
        H[i][0] = 0
    
    for i in range(1, n + 1):
        p = points[i - 1]
        h = hours[i - 1]
        for j in range(g + 1):
            if j >= p:
                H[i][j] = min(H[i - 1][j - p] + h, H[i - 1][j])
            else:
                H[i][j] = min(h, H[i - 1][j])
    
    min_hours = min(H[n][j] for j in range(g, g + 1))
    
    return min_hours

if __name__ == "__main__":
    i, j = input().split()
    # j = int(j)
    test_sizes = [(5, 100), (10, 200), (15, 500), (20, 1000), (25, 2000), (30, 3000), (35, 4000), (40, 5000), (45, 6000), (50, 7000)]
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    for (n, g) in test_sizes:
        start_time = time.time()
        min_hours = H(n, g, points, hours)
        end_time = time.time()

        if min_hours != float('inf'):
            # print(f"n = {n}, g = {g}, result = {result}, Time taken: {end_time - start_time} seconds")
            print(f"{end_time-start_time}")
        elif sum(points) < int(g):
            print(f"{end_time-start_time}")
            # print(f"n = {n}, g = {g}, result = {-1}, Time taken: {end_time - start_time} seconds")
        else:
            print(f"{end_time-start_time}")
            # print(f"n = {n}, g = {g}, result = {-1}, Time taken: {end_time - start_time} seconds")
