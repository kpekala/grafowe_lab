# https://faliszew.github.io/algograf/lab3

from dimacs import loadWeightedGraph
from queue import PriorityQueue
from flow_method import *
from utils import *

(V,L) = loadWeightedGraph("graphs/grid100x100")
graph = matrix_graph(V, L, True)

print(connectivity(graph))