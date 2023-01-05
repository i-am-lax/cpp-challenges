#include "sudoku.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

// Pre-supplied function to load a Sudoku board from a file
void load_board(const char *filename, char board[9][9]) {
    cout << "Loading Sudoku board from file '" << filename << "'... ";

    // check input filestream health
    ifstream in(filename);
    if (!in) {
        cout << "Failed!\n";
    }
    assert(in);

    char buffer[512];

    int row = 0;
    in.getline(buffer, 512);
    while (in && row < 9) {
        for (int n = 0; n < 9; n++) {
            assert(buffer[n] == '.' || isdigit(buffer[n]));
            board[row][n] = buffer[n];
        }
        row++;
        in.getline(buffer, 512);
    }

    cout << ((row == 9) ? "Success!" : "Failed!") << '\n';
    assert(row == 9);
}

// Internal helper function to print frame of board
void print_frame(const int &row) {
    if (!(row % 3)) {
        cout << "  +===========+===========+===========+\n";
    } else {
        cout << "  +---+---+---+---+---+---+---+---+---+\n";
    }
}

// Internal helper function to print a given 'row'
void print_row(const char *data, const int &row) {
    cout << (char) ('A' + row) << " ";
    for (int i = 0; i < 9; i++) {
        cout << ((i % 3) ? ':' : '|') << " ";
        cout << ((data[i] == '.') ? ' ' : data[i]) << " ";
    }
    cout << "|\n";
}

// Pre-supplied function to display a Sudoku board
void display_board(const char board[9][9]) {
    cout << "    ";
    for (int r = 0; r < 9; r++) {
        cout << (char) ('1' + r) << "   ";
    }
    cout << '\n';
    for (int r = 0; r < 9; r++) {
        print_frame(r);
        print_row(board[r], r);
    }
    print_frame(9);
}

// Internal helper function to check if 'entry' in board is empty or not
bool entry_is_empty(const char &entry) { return entry == '.'; }

/* Internal helper function to ensure that digit has a valid value between 1 to
 * 9 */
bool valid_digit(const char &digit) { return (digit >= '1' && digit <= '9'); }

/* Function which takes a 9 × 9 array of characters ('board') representing a
 * Sudoku board and returns true if all board positions are occupied by digits,
 * and false otherwise */
bool is_complete(const char board[9][9]) {
    // iterate through each entry
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            // return false if entry is an invalid digit
            if (!valid_digit(board[r][c]))
                return false;
        }
    }
    return true;
}

/* Function which outputs the 2-D character array board to a file with name
 * given by 'filename'. The return value is true if the file was successfully
 * written. */
bool save_board(const char *filename, const char board[9][9]) {
    // create output file stream
    ofstream out(filename);
    if (out.fail()) {
        cerr << "Error opening file: " << filename << endl;
        return false;
    }
    // write out board while ensuring output stream is healthy
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (out.good()) {
                out.put(board[r][c]);
            } else {
                return false;
            }
        }
        if (out.good()) {
            out.put('\n');
        } else {
            return false;
        }
    }
    out.close();
    return true;
}

/* Internal helper function to check if it is valid to place 'digit' in a given
 * 'row'. Each row must contain numbers from 1 to 9 once. */
bool valid_row(const char &digit, const int &row, const char board[9][9]) {
    for (int col = 0; col < 9; col++) {
        if (board[row][col] == digit)
            return false;
    }
    return true;
}

/* Internal helper function to check if it is valid to place 'digit' in a given
 * column, 'col'. Each column can only contain numbers from 1 to 9 once. */
bool valid_col(const char &digit, const int &col, const char board[9][9]) {
    for (int row = 0; row < 9; row++) {
        if (board[row][col] == digit)
            return false;
    }
    return true;
}

/* Internal helper function to check if it is valid to place 'digit' in its
 * respective 3x3 block. Each 3x3 square must contain numbers from 1 to 9 once.
 */
bool valid_square(const char &digit, const int &row, const int &col,
                  const char board[9][9]) {
    // get starting indices for 3x3 block corresponding to row and column
    int row_start = (row / 3) * 3, col_start = (col / 3) * 3;

    for (int r = row_start; r < row_start + 3; r++) {
        for (int c = col_start; c < col_start + 3; c++) {
            if (board[r][c] == digit)
                return false;
        }
    }
    return true;
}

/* Internal helper function to see if a 'digit' entry is valid based on row,
 * column and 3x3 square rules */
bool entry_is_valid(const char &digit, const int &row, const int &col,
                    const char board[9][9]) {
    return valid_row(digit, row, board) && valid_col(digit, col, board) &&
           valid_square(digit, row, col, board);
}

/* Function which attempts to place a digit onto a Sudoku board at a given
 * position. Returns true if successfully updated position. */
bool make_move(const char *position, const char &digit, char board[9][9]) {
    // ensure length of position is 2
    if (!position || strlen(position) != 2)
        return false;

    // check if digit out of range
    if (!valid_digit(digit))
        return false;

    // get integer row and column indices
    int row = position[0] - 'A', col = position[1] - '1';

    // check if row or column indices out of range
    if ((row < 0 || row >= 9) || (col < 0 || col >= 9))
        return false;

    // check if adding entry satisfies row, column and 3x3 block rules
    if (!entry_is_valid(digit, row, col, board))
        return false;

    // add digit if checks pass
    board[row][col] = digit;
    return true;
}

/* Function which attempts to solve the Sudoku puzzle in input/output parameter
 * 'board'. Return value is true if a solution is found, in which case 'board'
 * contains the solution found. Return value is false if a solution does not
 * exist and 'board' is unchanged. */
bool solve_board(char board[9][9]) {
    // board complete - terminate
    if (is_complete(board)) {
        return true;
    }

    // iterate through board
    for (char r = 'A'; r <= 'I'; r++) {
        for (char c = '1'; c <= '9'; c++) {

            // get integer row and column indices
            int row = r - 'A', col = c - '1';

            // if no entry exists then explore digits to add
            if (entry_is_empty(board[row][col])) {

                // try each possible digit from 1-9
                for (char digit = '1'; digit <= '9'; digit++) {
                    char position[3] = {r, c};

                    // update board if move possible
                    if (make_move(position, digit, board)) {

                        // recursive call
                        if (solve_board(board))
                            return true;

                        /* backtrack - clear position if not possible to add
                         * digit */
                        board[row][col] = '.';
                    }
                }
                return false;
            }
        }
    }
    return false;
}
