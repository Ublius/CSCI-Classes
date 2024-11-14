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
    n, g = map(int, input().split())
    points = list(map(int, input().split()))
    hours = list(map(int, input().split()))

    min_hours = H(n, g, points, hours)
    if min_hours[0] != float('inf'):
        print(min_hours[0])
        print(len(min_hours[1]))
        print(" ".join(map(str, min_hours[1])))
    elif sum(points) < g:
        print(-1)
    else:
        print(-1)
