# This is an implementation of the Task 1: Token race

# Since the token is passed randomly, the likelihood of the token
# being with any player is higher, the more central that player is in the game
# I.e. the more edges connected to the node, the more likely it is to get a token from another node

import graph
import numpy as np
import sys

# Matrix where connection (edge) from node u to node v is represented by 1
# No connection is 0
def adjacencyMatrix(g):

  # n*n matrix initialized
  # in programming index starts at 0 but the first node is 1
  # so indices are fixed to start at 1 throughout
  A = np.zeros((g.n-1, g.n-1))

  # iterating through each cell in the matrix
  for i in range(g.n-1):
    for j in range(g.n-1):

      # if there is an egde from node i to node j, add 1 to respective cell
      if (j+1) in g.adj[i+1]:
        A[i][j] = 1
      else:
        A[i][j] = 0
  print(A)
  return A

# probability of token going to random node = 0.01
# probability of token going to chosen neighbour, d = 0.99 (1.00 - 0.01)
def pageRank(A, random_prob = 0.01, iterations = 10):

  n = A.shape[0]
  pagerank_scores = np.ones(n) / n

  for i in range(iterations):
    # matrix-vector multiplication
    new_scores = A.dot(pagerank_scores)

    # counting in the probability of choosing random  node
    new_scores = random_prob + (1 - random_prob)*new_scores

    # if scores are converging, stop
    if np.allclose(pagerank_scores, new_scores):
      break

  pagerank_scores = new_scores
  print(pagerank_scores)
  return pagerank_scores

def rankNodePoints(pagerank_scores, A):

  top_nodes = []


  return top_nodes

### If ran from the command line:
if __name__ == "__main__":
  # Graph is the first command line argument:
  g = graph.Graph()
  g.readgraph(sys.argv[1])
  #file = "~/tty/graafialgoritmit/testgraph_1"
  #g.readgraph(file)
  A = adjacencyMatrix(g)
  pageRank(A)

