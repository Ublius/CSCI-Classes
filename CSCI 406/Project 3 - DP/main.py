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
