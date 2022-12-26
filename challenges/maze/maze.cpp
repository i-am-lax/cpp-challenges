#include <cassert>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

/* Allocate a dynamic 2D array */
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

/* Internal helper function which gets the dimensions of a maze */
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

/* load a maze from a file*/
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

/* Print a maze */
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

bool find_marker(const char ch, char **maze, const int &height,
                 const int &width, int &row, int &column) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            if (maze[r][c] == ch) {
                row = r;
                column = c;
                return true;
            }
        }
    }
    row = -1;
    column = -1;
    return false;
}

/* Internal helper function to adjust the row or column index based on the
 * direction provided (N/S/E/W) */
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

bool valid_solution(const char *path, char **maze, const int &height,
                    const int &width) {
    // find coordinates for entrance and exit markers
    int row, col, end_row, end_col;
    find_marker('>', maze, height, width, row, col);
    find_marker('X', maze, height, width, end_row, end_col);

    /* start at coordinates for '>' and then for each direction, check if it is
     * a barrier ('|', '+' or '-') */
    while (*path != '\0') {
        if (maze[row][col] == '|' || maze[row][col] == '+' ||
            maze[row][col] == '-') {
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