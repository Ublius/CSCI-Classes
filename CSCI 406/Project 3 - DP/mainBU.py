import sys

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
    n, g = map(int, input().split())
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    min_hours = H(n, g, points, hours)
    if min_hours != float('inf'):
        print(min_hours)
    elif sum(points) < g:
        print(-1)
    else:
        print(-1)