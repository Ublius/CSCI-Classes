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

