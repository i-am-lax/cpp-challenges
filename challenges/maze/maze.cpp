#include "maze.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

/* Allocate a dynamic 2D array with dimensions: [rows, columns] */
char **allocate_2D_array(int rows, int columns) {
    char **m = new char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new char[columns];
        assert(m[r]);
    }
    return m;
}

/* Deallocate a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
    for (int r = 0; r < rows; r++)
        delete[] m[r];
    delete[] m;
}

/* Internal helper function which gets the dimensions of a maze from a file
 * given by 'filename' and stores the values in 'height' and 'width' */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
    char line[512];

    ifstream input(filename);

    height = width = 0;

    input.getline(line, 512);
    while (input) {
        if ((int) strlen(line) > width)
            width = strlen(line);
        height++;
        input.getline(line, 512);
    }

    if (height > 0)
        return true;
    return false;
}

// Load a maze from a file given by 'filename' with dimensions [height, width]
char **load_maze(const char *filename, int &height, int &width) {

    bool success = get_maze_dimensions(filename, height, width);

    if (!success)
        return NULL;

    char **m = allocate_2D_array(height, width);

    ifstream input(filename);

    char line[512];

    for (int r = 0; r < height; r++) {
        input.getline(line, 512);
        strcpy(m[r], line);
    }

    return m;
}

// Print a maze ('m') with dimensions [height, width]
void print_maze(char **m, int height, int width) {
    cout << setw(4) << " "
         << " ";
    for (int c = 0; c < width; c++)
        if (c && (c % 10) == 0)
            cout << c / 10;
        else
            cout << " ";
    cout << endl;

    cout << setw(4) << " "
         << " ";
    for (int c = 0; c < width; c++)
        cout << (c % 10);
    cout << endl;

    for (int r = 0; r < height; r++) {
        cout << setw(4) << r << " ";
        for (int c = 0; c < width; c++)
            cout << m[r][c];
        cout << endl;
    }
}

/* Identify marker 'ch' in the input maze with dimensions 'height' x 'width' and
 * return true if it exists and overwrite 'row' and 'column' with the
 * coordinates, otherwise return false with 'row' and 'column' both set to -1 */
bool find_marker(const char ch, char **maze, const int &height,
                 const int &width, int &row, int &column) {
    // set row and column to coordinates of marker if found
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            if (maze[r][c] == ch) {
                row = r;
                column = c;
                return true;
            }
        }
    }
    // otherwise return default values
    row = -1;
    column = -1;
    return false;
}

/* Internal helper function to adjust the row or column index based on the
 * direction provided (N/E/S/W) */
void make_move(const char direction, int &row, int &col) {
    switch (direction) {
    case 'N':
        row--;
        break;
    case 'E':
        col++;
        break;
    case 'S':
        row++;
        break;
    case 'W':
        col--;
        break;
    default:
        cerr << "[Error] Invalid direction: " << direction << endl;
    }
}

/* Internal helper function to check if a given position on the input maze is
 * valid - if it is a barrier or out of range then we return false */
bool is_valid_move(char **maze, const int &height, const int &width, int &row,
                   int &col) {
    if (maze[row][col] == '|' || maze[row][col] == '+' ||
        maze[row][col] == '-') {
        return false;
    }
    if (row < 0 || row >= height) {
        return false;
    }
    if (col < 0 || col >= width) {
        return false;
    }
    return true;
}

/* Return true if a given 'path' (a series of N/S/E/W chars) through a 'height'
 * × 'width' maze leads from entrance marker '>' to exit marker 'X' without
 * moving beyond the boundaries of the maze or passing through a hedge */
bool valid_solution(const char *path, char **maze, const int &height,
                    const int &width) {
    // find coordinates for entrance and exit markers
    int row, col, end_row, end_col;
    find_marker('>', maze, height, width, row, col);
    find_marker('X', maze, height, width, end_row, end_col);

    /* start at coordinates for '>' and then for each direction, check if it is
     * a barrier or out of range */
    while (*path != '\0') {
        if (!is_valid_move(maze, height, width, row, col)) {
            return false;
        }
        // make the move and increment either the row or col
        make_move(*path, row, col);
        path++;
    }

    /* if final position is equivalent to that of the 'X' end marker then it is
     * valid */
    if (row == end_row && col == end_col) {
        return true;
    }
    return false;
}

/* Recursive auxiliary function to generate the solution for the input 'maze'
 * from 'start' and 'end' markers and write this out to 'path'. Based on the
 * current position given by 'row' and 'col we explore every possibility until
 * we eventually reach the 'end' marker, or exhaust all options */
bool find_path_aux(char **maze, const int &height, const int &width, int &row,
                   int &col, const char start, const char end, string &path) {

    // update grid for starting position
    if (maze[row][col] == start) {
        maze[row][col] = '#';
    }

    /* for each direction we make a move:
    - if the move lands on the exit then we update and return
    - otherwise if it is a valid move and we haven't visited it before we update
    the grid and make a recursive call to explore possibilities from there
    - if we hit a dead end or there is no solution then we backtrack */
    for (auto const &d : DIRECTIONS) {
        int prev_row = row, prev_col = col;
        make_move(d, row, col);
        if (is_valid_move(maze, height, width, row, col) &&
            maze[row][col] != '#') {
            if (maze[row][col] == end) {
                maze[row][col] = '#';
                path += d;
                return true;
            }
            maze[row][col] = '#';
            path += d;
            if (find_path_aux(maze, height, width, row, col, start, end,
                              path)) {
                return true;
            }
            // backtrack
            maze[row][col] = ' ';
            path.pop_back();
        }
        row = prev_row;
        col = prev_col;
    }
    return false;
}

/* Return the sequence of N/E/S/W directions if one exists to solve
 * 'maze' from the 'start' marker to the 'end' marker, otherwise return "no
 * solution" */
string find_path(char **maze, const int &height, const int &width,
                 const char start, const char end) {
    string path;

    // retrieve coordinates for 'start' position
    int row, col;
    find_marker(start, maze, height, width, row, col);

    // identify path and generate sequence
    if (!find_path_aux(maze, height, width, row, col, start, end, path)) {
        path = "no solution";
    }
    return path;
}