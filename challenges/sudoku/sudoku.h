#ifndef SUDOKU_H
#define SUDOKU_H

/* Pre-supplied function to load a Sudoku board from 'filename' into 9x9 array
 * 'board' */
void load_board(const char *filename, char board[9][9]);

// Pre-supplied function to print out 'board' in appropriate format
void display_board(const char board[9][9]);

/* Takes as input 'board' which is a 9 × 9 array of characters representing a
 * Sudoku board and returns true if all board positions are occupied by digits,
 * and false otherwise */
bool is_complete(const char board[9][9]);

/* Attempts to place 'digit' (1-9) onto a Sudoku 'board' at the input
 * 'position'. If 'digit' and 'position' are valid inputs and 'digit' can be
 * placed without violating the rules then the board is updated and the function
 * returns true, otherwise it returns false */
bool make_move(const char *position, const char &digit, char board[9][9]);

/* Outputs the 2D 9x9 character array 'board' to a file with name 'filename'.
 * The return value is true if the file was successfully written, and false
 * otherwise */
bool save_board(const char *filename, const char board[9][9]);

/* Attempts to solve the Sudoku puzzle in input/output parameter 'board'. The
 * return value of the function is true if a solution is found, in which case
 * 'board' contains the solution found. In the case that a solution does not
 * exist the return value is false and 'board' contains the original board */
bool solve_board(char board[9][9]);

#endif
