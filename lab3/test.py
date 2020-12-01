from copy import deepcopy
graph = [
    [1,2,3],
    [4,5,6],
    [7,8,9]
]

graph1 = deepcopy(graph)
graph1[0][0] = 9
print(graph)