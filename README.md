# Ex4
## How it works?
* The program reads from the client
* a corresponding message is sent from the server
* The program reads the input of the graph
* it goes through the matrix with the matching algorithm and given 
  entry and exit points
* if succeced, the result is sent from the server. else, corresponding error code is printed

## Error Codes:
0. Path found, no error.
1. Path not found, thrown if the algorithm couldn't find a path with given matrix.
2. out of bounds. thrown if the given entry or exit points are out of bounds. 