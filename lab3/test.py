arr = []
while True:
    line = input()
    if line:
        arr.append(int(line))
    else:
        break

n = len(arr)
for i in range(n-2):
    for j in range(i, n-1):
        for k in range(j, n):
            if arr[i] + arr[j] + arr[k] == 2020:
                print(arr[i] * arr[j] * arr[k])