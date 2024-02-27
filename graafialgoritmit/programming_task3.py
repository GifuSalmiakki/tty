# This is an implementation of the Task 1: Token race

# Since the token is passed randomly, the likelihood of the token
# being with any one player is higher, the more central that player is in the game
# I.e. the more edges connected to the node, the more likely it is to get a token from another node
# The points can be estimated with PageRank algorithm

import graph
import numpy as np
import sys

# Matrix where connection (edge) from node u to node v is represented by 1
# No connection is 0
def createAdjacencyMatrix(g):

  # n*n matrix initialized
  # assuming first node is 0
  A = np.zeros((g.n, g.n))

  # iterating through each cell in the matrix
  for i in range(g.n):
    for j in range(g.n):

      # if there is an egde from node i to node j, add 1/(outgoing edges) to respective cell
      if (j) in g.adj[i]:
        A[i][j] = 1/len(g.adj[i])

  # for the matrix to describe incoming links, flipping rows to columns
  A = np.transpose(A)
  return A

# probability of token going to random node = 0.01
# probability of token going to chosen neighbour, d = 0.99 (1.00 - 0.01)
def pageRank(A, random_prob = 0.01, iterations = 10):

  # at first, calculating equal odds for each node
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
  return pagerank_scores

def rankNodePoints(pagerank_scores, g):

  # we cannot sort the pagerank scores without losing which node it connects to
  # so the score and node must be connected first
  node_and_score = {}
  for node in range(g.n):
    node_and_score[node] = pagerank_scores[node]

  # sorting the dictionary in descending order by score (value)
  sorted_nodes = sorted(node_and_score.items(), key=lambda x: x[1], reverse=True)

  # sorting creates a list around the dict for some reason I'm too lazy to bother fixing
  # so here we are grabbing only the keys (nodes)
  top_nodes = []
  for i in range(9):
    if len(sorted_nodes) >= 9:
      top_nodes.append(sorted_nodes[i][0])

  return top_nodes

### If ran from the command line:
if __name__ == "__main__":
  # Graph is the first command line argument:
  g = graph.Graph()
  g.readgraph(sys.argv[1])

  A = createAdjacencyMatrix(g)
  pagerank_scores = pageRank(A)

  top_players = rankNodePoints(pagerank_scores, g)
  print(top_players)


