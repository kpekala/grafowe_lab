class Node:
    def __init__(self):
        self.edges = {}
        self.disabled = False

    def addEdge(self, to, weight):
        self.edges[to] = self.edges.get(to, 0) + weight

    def delEdge(self, to):
        del self.edges[to]

    def disable(self):
        self.disabled = True
        self.edges = []


def nodesFromEdges(edges, n):
    nodes = [Node() for i in range(n)]
    for (x, y, c) in edges:
        nodes[x].addEdge(y, c)
        nodes[y].addEdge(x, c)


def mergeVertices(G, x, y):
    for v in list(G[y].edges):
        if v == x:
            G[x].delEdge(y)
        else:
            weight = G[y].edges[v]
            G[x].addEdge(v, weight)
            G[v].addEdge(x, weight)
    G[y].disable()
