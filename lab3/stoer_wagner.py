from queue import PriorityQueue
import math

class Node:
    def __init__(self):
        self.edges = {}
        self.disabled = False
        self.processed = False

    def addEdge(self, to, weight):
        self.edges[to] = self.edges.get(to, 0) + weight

    def delEdge(self, to):
        del self.edges[to]

    def disable(self):
        self.disabled = True
        self.edges = {}

def notDisabled(graph):
    return [v for v in graph if not v.disabled]



def nodesFromEdges(edges, n):
    nodes = [Node() for i in range(n)]
    for (x, y, c) in edges:
        nodes[x-1].addEdge(y-1, c)
        nodes[y-1].addEdge(x-1, c)
    return nodes

def cleanGraph(graph):
  for u in graph:
    u.processed = False

def minimumCutPhase(graph):
    cleanGraph(graph)
    n = len(notDisabled(graph))
    S = []
    weights = [0] * len(graph)
    queue = PriorityQueue()
    queue.put((0,0))
    while len(S) < n: #and not queue.empty():
      c, v = queue.get()
      if graph[v].processed or graph[v].disabled:
        continue
      S.append(v)
      graph[v].processed = True
      #aktualizacja sum wag
      for u in list(graph[v].edges):
        weight = graph[v].edges.get(u)
        weights[u] += weight
        queue.put((-weights[u], u))

    if len(S) >= 2:
      mergeVertices(graph, S[-2], S[-1])
    return weights[S[-1]]

def stoerWagner(graph):
  res = math.inf
  while len(notDisabled(graph)) > 2:
    res = min(res, minimumCutPhase(graph))
  return res



def mergeVertices(graph, x, y):
    for v in list(graph[y].edges):
        if v == x:
            graph[x].delEdge(y)
        else:
            weight = graph[y].edges[v]
            graph[x].addEdge(v, weight)
            graph[v].addEdge(x, weight)
    graph[y].disable()
