# Ex4
## How it works?
* The program reads from the client
* a corresponding message is sent from the server
* The program reads the input of the graph
* it goes through the matrix with the matching algorithm and given 
  entry and exit points
* if succeced, the result is sent from the server. else, corresponding error code is printed

## Serach Statuses:
0. **PATH_FOUND** -  no error.
1. **PATH_NOT_FOUND** - thrown if the algorithm couldn't find a path with given matrix(due to b values for example).
2. **MISSING_BREAK_LINE** - there is no double breakline at the end(corrupted data).
3. **UNKNOWN_ALGORITHM** - there isn't an algorithm matching to the one given by the client.
4. **WRONG_MATRIX_DIMENSIONS_FORMAT** - the height, width line idn't in the right format.
5. **WRONG_NUMBER_OF_LINES** - the number of lines is'nt matching the line.
6. **WRONG_LINE_FORMAT** - one of the lines isn't in the right format (numbers seperated by commas, no spaces).
7. **WRONG_NUMBER_OF_CELLS_IN_LINE** - the length of a line doesn't match with the width of the matrix.
8. **WRONG_START_CELL_FORMAT** - the start line isn't in a right format (two integers seperated by a comma)
9. **WRONG_END_CELL_FORMAT** - the end line isn't in a right format (two integers seperated by a comma).
