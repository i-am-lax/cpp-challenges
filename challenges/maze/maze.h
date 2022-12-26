#ifndef MAZE_H
#define MAZE_H

#include <list>

// maximum character constraint
const int MAX_LENGTH = 512;

// set of possible directions
const std::list<char> DIRECTIONS = {'N', 'E', 'S', 'W'};

// Allocate and deallocate dynamic arrays
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);
void deallocate_char_array(char *ptr);

/* Load a maze from a file given by 'filename' with dimensions 'height' x
 * 'width' */
char **load_maze(const char *filename, int &height, int &width);

// Print a maze ('m') with dimensions 'height' x 'width'
void print_maze(char **m, int height, int width);

/* Find the coordinates of marker character 'ch' in the 2D array of characters
 * 'maze' of dimension 'height' × 'width'. When the maze contains 'ch', 'row'
 * and 'column' is set to the row and column coordinates of the marker character
 * respectively, and the function returns true. Otherwise 'row' and 'column' are
 * both set to -1, and the function returns false */
bool find_marker(const char ch, char **maze, const int &height,
                 const int &width, int &row, int &column);

/* Determine if a given 'path' through a 'height' × 'width' maze leads from the
 * entrance marker '>' to the exit marker 'X' without moving outside the
 * boundaries of the maze or passing through a hedge. The parameter path is a
 * string of uppercase characters, each of which is in the set {N, S, E, W} */
bool valid_solution(const char *path, char **maze, const int &height,
                    const int &width);

/* Finds a solution through a 'height' × 'width' maze beginning at marker
 * character 'start' and terminating at marker character 'end'. The path is
 * marked on the maze using '#' characters, and the function returns a string of
 * direction movements. If there is no path, the function returns the string "no
 * solution". */
char *find_path(char **maze, const int &height, const int &width,
                const char start, const char end);

#endif