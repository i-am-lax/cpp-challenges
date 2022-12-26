#ifndef MAZE_H
#define MAZE_H

/* Allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* Load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* Print a maze */
void print_maze(char **m, int height, int width);

bool find_marker(const char ch, char **maze, const int &height,
                 const int &width, int &row, int &column);

#endif