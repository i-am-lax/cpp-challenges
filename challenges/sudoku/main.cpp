#include "sudoku.h"
#include <cstdio>
#include <iostream>

using namespace std;

// Testing function for is_complete
void test_is_complete(const char *filename) {
    char board[9][9];
    load_board(filename, board);

    cout << "Board is ";
    if (!is_complete(board)) {
        cout << "NOT ";
    }
    cout << "complete.\n\n";
}

// Testing function for make_move
void test_make_move(const char *position, char digit, char board[9][9]) {
    cout << "Putting '" << digit << "' into " << position << " is ";
    if (!make_move(position, digit, board)) {
        cout << "NOT ";
    }
    cout << "a valid move. The board is:\n";
    display_board(board);
}

// Testing function for save_board
void test_save_board(const char *file_in, const char *file_out) {
    char board[9][9];
    load_board(file_in, board);

    if (save_board(file_out, board)) {
        cout << "Save board to '" << file_out << "' successful.\n";
    } else {
        cout << "Save board failed.\n";
    }
    cout << '\n';
}

// Testing function for solve_board
void test_solve_board(const char *filename) {
    char board[9][9];
    load_board(filename, board);
    display_board(board);

    if (solve_board(board)) {
        cout << "The '" << filename << "' board has a solution:\n";
    } else {
        cout << "A solution cannot be found.\n";
    }
    display_board(board);
    cout << '\n';
}

int main() {

    char board[9][9];

    cout << "============= Pre-supplied functions =============\n\n";

    cout << "Calling load_board():\n";
    load_board("easy.dat", board);

    cout << '\n';
    cout << "Displaying Sudoku board with display_board():\n";
    display_board(board);
    cout << "Done!\n\n";

    cout << "=================== Question 1 ===================\n\n";

    // check easy.dat board is incomplete
    test_is_complete("easy.dat");

    // check easy-solution.dat board is complete
    test_is_complete("easy-solution.dat");

    cout << "=================== Question 2 ===================\n\n";

    load_board("easy.dat", board);

    // test a valid move
    test_make_move("I8", '1', board);

    // test for invalid length
    test_make_move("A12", '2', board);

    // test for invalid row
    test_make_move("J8", '1', board);

    // test for invalid column
    test_make_move("C0", '3', board);

    // test for invalid digit
    test_make_move("E1", '!', board);

    // test for adding digit when one exists in row
    test_make_move("B3", '4', board);

    // test for adding digit when one exists in column
    test_make_move("B3", '8', board);

    // test for adding digit when one exists in 3x3 block
    test_make_move("B7", '6', board);

    cout << "=================== Question 3 ===================\n\n";

    // check we can save contents of easy.dat to easy-copy.dat
    test_save_board("easy.dat", "easy-copy.dat");

    cout << "=================== Question 4 ===================\n\n";

    // attempt to solve the easy and medium boards
    test_solve_board("easy.dat");
    test_solve_board("medium.dat");

    cout << "=================== Question 5 ===================\n\n";

    // attempt to solve 3 mystery boards
    test_solve_board("mystery1.dat");
    test_solve_board("mystery2.dat");
    test_solve_board("mystery3.dat");

    return 0;
}
