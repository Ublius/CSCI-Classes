import sys

def minimum_hours(n, g, points, hours):
    # Initialize DP table with "infinity" for values not achievable
    H = [[float('inf')] * (g + 1) for _ in range(n + 1)]
    
    # Base case: 0 points require 0 hours for any number of tasks
    for i in range(n + 1):
        H[i][0] = 0
    
    # Fill DP table according to the recurrence relation
    for i in range(1, n + 1):
        pi = points[i - 1]
        hi = hours[i - 1]
        for j in range(g + 1):
            if j >= pi:
                H[i][j] = min(H[i - 1][j - pi] + hi, H[i - 1][j])
            else:
                H[i][j] = min(hi, H[i - 1][j])
    
    # Find the minimum hours to achieve at least 'g' points
    min_hours = min(H[n][j] for j in range(g, g + 1))
    
    # Return -1 if achieving the desired points is impossible
    return min_hours if min_hours < float('inf') else -1

# Sample Input
n, g = map(int, input().split())
points = list(map(int, input().split()))
hours = list(map(int, input().split()))

# Output the result
print(minimum_hours(n, g, points, hours))
