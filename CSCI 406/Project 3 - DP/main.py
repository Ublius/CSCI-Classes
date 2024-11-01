def H(n, g, points, hours):
    # Base Cases
    if g <= 0:
        return 0
    if n == 0:
        return float('inf')  # Return infinity if no tasks are left and g > 0

    # Check if the current task can be included or not
    if points[n - 1] <= g:
        return min(H(n - 1, g - points[n - 1], points, hours) + hours[n - 1],
                   H(n - 1, g, points, hours))
    else:
        return H(n - 1, g, points, hours)


# Input Handling
if __name__ == "__main__":
    n, g = map(int, input().split())
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    min_hours = H(n, g, points, hours)
    if min_hours != float('inf'):
        print(min_hours)
    else:
        print(-1)
