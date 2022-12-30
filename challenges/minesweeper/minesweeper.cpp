#include "minesweeper.h"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

    cout << "Loading board from file '" << filename << "'... ";

    ifstream in(filename);
    if (!in)
        cout << "Failed!" << endl;
    assert(in);

    char buffer[512];

    int row = 0;
    in.getline(buffer, 512);
    while (in && row < 9) {
        for (int n = 0; n < 9; n++) {
            assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' ||
                   buffer[n] == '?' || isdigit(buffer[n]));
            board[row][n] = buffer[n];
        }
        row++;
        in.getline(buffer, 512);
    }

    cout << ((row == 9) ? "Success!" : "Failed!") << endl;
    assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
    cout << (char) ('A' + row) << "|";
    for (int i = 0; i < 9; i++)
        cout << ((data[i] == '.') ? ' ' : data[i]);
    cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
    cout << "  ";
    for (int r = 0; r < 9; r++)
        cout << (char) ('1' + r);
    cout << endl;
    cout << " +---------+" << endl;
    for (int r = 0; r < 9; r++)
        print_row(board[r], r);
    cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */
void initialise_board(char board[9][9]) {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            board[r][c] = '?';
}

bool is_complete(const char mines[9][9], const char revealed[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (revealed[row][col] == '?' && mines[row][col] != '*') {
                return false;
            }
        }
    }
    return true;
}

int count_mines(const char *position, const char mines[9][9]) {
    if (!position || strlen(position) != 2) {
        throw invalid_argument("[Invalid Argument Error] Position must be a "
                               "string of 2 characters.");
    }
    int row = position[0] - 'A', col = position[1] - '1';
    cout << "Position: " << position << " is " << row << ", " << col << endl;

    // there are a maximum of 8 checks
    int count = 0;
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (r == row && c == col) {
                continue;
            }
            if (r < 0 || r > 8) {
                continue;
            }
            if (c < 0 || c > 8) {
                continue;
            }
            if (mines[r][c] == '*') {
                count += 1;
            }
        }
    }
    return count;
}