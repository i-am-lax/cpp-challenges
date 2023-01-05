#include "sudoku.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* pre-supplied function to load a Sudoku board from a file */
void load_board(const char *filename, char board[9][9]) {

    cout << "Loading Sudoku board from file '" << filename << "'... ";

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

/* internal helper function */
void print_frame(int row) {
    if (!(row % 3)) {
        cout << "  +===========+===========+===========+\n";
    } else {
        cout << "  +---+---+---+---+---+---+---+---+---+\n";
    }
}

/* internal helper function */
void print_row(const char *data, int row) {
    cout << (char) ('A' + row) << " ";
    for (int i = 0; i < 9; i++) {
        cout << ((i % 3) ? ':' : '|') << " ";
        cout << ((data[i] == '.') ? ' ' : data[i]) << " ";
    }
    cout << "|\n";
}

/* pre-supplied function to display a Sudoku board */
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

bool is_complete(const char board[9][9]) {
	for (int r = 0; r < 9; r++) {
		for (int c = 0; c < 9; c++) {
			if (!isdigit(board[r][c])) {
				return false;
			}
		}
	}
	return true;
}

bool valid_indices(const int &row, const int &col) {
	if (row < 0 || row > 8) {
		return false;
	}
	if (col < 0 || col > 8) {
		return false;
	}
	return true;
}

bool digit_in_row(const char digit, const int &row, const char board[9][9]) {
	for (int col = 0; col < 9; col++) {
		if (board[row][col] == digit) {
			return true;
		}
	}
	return false;
}

bool digit_in_col(const char digit, const int &col, const char board[9][9]) {
    for (int row = 0; row < 9; row++) {
        if (board[row][col] == digit) {
            return true;
        }
    }
    return false;
}

bool digit_in_square(const char digit, const int &row, const int &col, const char board[9][9]) {
	for (int r = (row / 3) * 3; r < ((row / 3) * 3) + 3; r++) {
		for (int c = (col / 3) * 3; c < ((col / 3) * 3) + 3; c++) {
			if (board[r][c] == digit) {
				return true;
			}
		}
	}
	return false;
}

bool make_move(const char* position, const char digit, char board[9][9]) {
	// check string is valid
	if (!position || strlen(position) != 2) {
		return false;
	}
	// extract row and column
	int row = position[0] - 'A', col = position[1] - '1';
	
	// check row and column within range on board
	if (!valid_indices(row, col)) {
		return false;
	}

	// check digit is valid
	if (digit < '1' || digit > '9') {
		return false;
	}

	// check digit does not already exist in same row, column and square	
	if (digit_in_row(digit, row, board) || digit_in_col(digit, col, board) 
		|| digit_in_square(digit, row, col, board)) {
			return false;
	}
	
	// update board
	board[row][col] = digit;	

	return true;
}	
