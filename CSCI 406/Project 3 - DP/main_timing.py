import time

def H(n, g, points, hours):
    if g <= 0:
        return 0
    if n == 0 and g > 0:
        return float('inf') 

    if points[n - 1] <= g:
        return min(H(n - 1, g - points[n - 1], points, hours) + hours[n - 1], H(n - 1, g, points, hours))
    else:
        return min(hours[n - 1], H(n - 1, g, points, hours))

if __name__ == "__main__":
    n, g = input().split()
    g = int(g)
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    for i in range(5, 25):
        start_time = time.time()
        min_hours = H(i, g, points, hours)
        end_time = time.time()

        if min_hours != float('inf'):
            # print(f"n = {i}, result = {result}, Time taken: {end_time - start_time} seconds")
            print(f"{end_time-start_time}")
        elif sum(points) < g:
            print(f"{end_time-start_time}")
            # print(f"n = {i}, result = {-1}, Time taken: {end_time - start_time} seconds")
        else:
            print(f"{end_time-start_time}")
            # print(f"n = {i}, result = {-1}, Time taken: {end_time - start_time} seconds")
