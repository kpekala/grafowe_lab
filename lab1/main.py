from dimacs import *
from find_union import *
from sys import argv

def sortEdges(edges):
    edges.sort(key=lambda tup: tup[2], reverse=True)

def findUnionSolution(edges, n):
    sortEdges(L)
    v = [Element(i) for i in range(n)]

    result = 0
    for edge in edges:
        if find(v[0]) != find(v[1]):
            union(v[edge[0]-1], find(v[edge[1]-1]))
            result = edge[2]
    return result

def adjacencyList(L, n):
    adj_list = [[] for _ in range(n)]
    for edge in L:
        adj_list[edge[0]].append((edge[1], edge[2]))
        adj_list[edge[1]].append((edge[0], edge[2]))
    return adj_list


if __name__ == "__main__":
    name = "test_graphs/"+argv[1]
    (V, L) = loadWeightedGraph(name)
    print("test :",int(readSolution(name)) == findUnionSolution(L,V))
