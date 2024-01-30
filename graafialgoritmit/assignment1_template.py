### Read in a graph, a set of vertices, and a pair of vertices.

import graph
import sys
from icecream import ic


## Implement your algorithm here:
def algorithm(g, B, v, w):

    start_node = v
    target_node = w

    visited_nodes = []
    queue = [start_node]

    shortest_path = []
    if start_node == target_node:
        return 0

    while queue:

        current_node = queue.pop(0)
        if current_node not in visited_nodes:
            visited_nodes.append(current_node)
            neighbours = g[current_node]
            for neighbour in neighbours:
                new_path = current_node
                new_path.append(neighbour)
                queue.append(new_path)

                if neighbour == target_node:
                    print(shortest_path)

        visited_nodes.append(node)

    nodes_of_B_on_path = 0
    for node in shortest_path:
      if node in B:
        nodes_of_B_on_path += 1

    return nodes_of_B_on_path



"""

    current_min = None
    for node in unvisited_nodes:
      if current_min == None:
        current_min = node
      elif shortest_path[node.index] < shortest_path[current_min.index]:
        current_min = node

    adj_n = g.adj[current_min]
    for a in adj_n:
      val = shortest_path[current_min] + graph[w(current_min, a)]
      if val > shortest_path[a]:
        shortest_path[a] = val
        previous_vertices[a] = current_min

    unvisited_vertices.remove(current_min)"""


### Read in a set of vertices from a file. These are just numbers separated by whitespace.
def readset(filename):
    f = open(filename, 'r')
    s = set()
    for line in f:
        for v in line.split():
            s.add(int(v))
    return s


## Read the pair, again, just two integers separated by whitespace.
def readpair(filename):
    f = open(filename, 'r')
    for line in f:
        (v, w) = line.split()
        return (int(v), int(w))


"""
### If ran from the command line:
if __name__ == "__main__":
  # Graph is the first command line argument:
  g = graph.Graph()
  g.readgraph(sys.argv[1])
  # Vertices are the second command line argument:
  B = readset(sys.argv[2])
  # Pair is the third command line argument:
  (v,w) = readpair(sys.argv[3])

  ### Call your algorithm:
  ic(B)
  ic(v)
  ic(w)

  n = algorithm(g, B, v, w)

  # Print the result:
  print(n)
  
"""

files = input("Please give the test files (specify path if not current directory): ")
try:
    print("Processing...")
    split = files.split(" ")

except:
    print("Files not found. Is the path correct?")

g = graph.Graph()
g.readgraph(split[0])
# Vertices are the second command line argument:
B = readset(split[1])
# Pair is the third command line argument:
(v, w) = readpair(split[2])

### Call your algorithm:
ic(B)
ic(v)
ic(w)

n = algorithm(g, B, v, w)

# Print the result:
print(n)
