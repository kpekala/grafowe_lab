# https://faliszew.github.io/algograf/lab3


from dimacs import loadWeightedGraph
from queue import PriorityQueue
from flow_method import *
from utils import *
from stoer_wagner import *

(V,L) = loadWeightedGraph("graphs/clique200")
graph = nodesFromEdges(L, V)
print(stoerWagner(graph))
