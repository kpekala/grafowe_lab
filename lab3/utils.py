def matrix_graph(V, L, undirected):
    g = [[0 for j in range(V)] for i in range(V)]
    for (x, y, c) in L:
        g[x - 1][y - 1] = c
        if undirected:
            g[y - 1][x - 1] = c
    return g


def adjacency_list(n, edges):
    g = [[] for _ in range(n)]
    for (x, y, c) in edges:
        g[x - 1].append((y - 1, c))
    return g
