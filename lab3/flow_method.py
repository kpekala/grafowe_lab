import math
from copy import deepcopy


def bfs(s,t, graph, parent):
    n = len(graph)
    visited = [False] * n
    queue = [s]
    visited[s] = True

    while queue:
        u = queue.pop(0)
        for v, val in enumerate(graph[u]):
            if not visited[v] and val > 0:
                visited[v] = True
                parent[v] = u
                if v == t:
                    return True
                queue.append(v)

    return visited[t]


def max_flow(graph, s, t):
    n = len(graph)
    parent = [-1] * n
    max_flow = 0
    while bfs(s,t, graph, parent):
        max_flow += 1
        itr = t
        while itr != s:
            u = parent[itr]
            graph[u][itr] = 0
            graph[itr][u] = 0
            itr = parent[itr]
        parent = [-1] * n
    return max_flow

#graph is represented by a matrix
def connectivity(graph):
    n = len(graph)
    min_val = math.inf
    for s in range(n-1):
        min_val = min(min_val, max_flow(deepcopy(graph), s, n-1))
    return min_val
