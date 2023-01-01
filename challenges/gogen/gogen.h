#include "mask.h"
#include <vector>

// Set of letters on Gogen board
const std::vector<char> LETTERS = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                   'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                                   'S', 'T', 'U', 'V', 'W', 'X', 'Y'};

/* Reads a Gogen board from a file ('filename') and returns a 2D array of
 * characters. Memory for board is allocated on the heap */
char **load_board(const char *filename);

// Outputs a Gogen board ('board') to a file ('filename')
bool save_board(char **board, const char *filename);

/* Prints the contents of the 2D array of characters describing the Gogen board
 * ('board') in an appropriate format */
void print_board(char **board);

/* Reads a word list from a file ('filename') and returns a NULL-terminated
 * array of character pointers, each of which points to one word. Memory for the
 * words is allocated on the heap */
char **load_words(const char *filename);

// Prints out the word list given by 'words'
void print_words(char **words);

// Deallocates the memory on heap for 'board'
void delete_board(char **board);

// Deallocates the memory on heap for 'words'
void delete_words(char **words);

/* Searches for the first occurrence of character 'ch' in 'board' when searched
 * in a row-by-row fashion. If the character is found, the function returns true
 * and sets the output parameters 'row' and 'column' to the position of 'ch'. If
 * the character cannot be found, the function returns false and 'row' and
 * 'column' are both set to -1 */
bool get_position(char **board, const char ch, int &row, int &column);

/* Returns true if the given 'board' represents a solution to the Gogen puzzle
 * with the given list of 'words' */
bool valid_solution(char **board, char **words);

/* Mutually updates the 'board' and a 'mask' for letter 'ch' based on:
 * - fixed location - if 'ch' is already on the board then all mask entries are
 * false except for at the position of 'ch'
 * - free locations - mark letters that are not 'ch' as false and the remaining
 * are true to represent possible positions of 'ch'
 * - confirmed location - if mask contains only one cell that is true then the
 * board is updated with 'ch' at that position */
void update(char **board, const char ch, Mask &mask);

/* Modifies masks 'one' and 'two' by intersecting each with the 1-neighbourhood
 * of the other */
void neighbourhood_intersect(Mask &one, Mask &two);

/* Attempts to find a solution to a given Gogen puzzle. If a solution can be
 * found, the function returns true and the parameter 'board' should contain the
 * completed board. Otherwise the function returns false */
bool solve_board(char **board, char **words);