#include "minesweeper.h"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

#include <vector>

using namespace std;

// Pre-supplied function to load mine positions from a file into 'board'
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

// Internal helper function to print row of board */
void print_row(const char *data, int row) {
    cout << (char) ('A' + row) << "|";
    for (int i = 0; i < 9; i++)
        cout << ((data[i] == '.') ? ' ' : data[i]);
    cout << "|" << endl;
}

// Pre-supplied function to display a minesweeper board
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

// Pre-supplied function to initialise playing board
void initialise_board(char board[9][9]) {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            board[r][c] = '?';
}

/* Returns true if the current board 'revealed' is complete based on the board
 * representing mine locations 'mines'. Complete is defined by all non-mine
 * squares in the playing board having been uncovered. Logic used - if the
 * square is unrevealed ('?') or flagged ('*') and the actual location is not a
 * mine, then we return false because there are non-mine squares active */
bool is_complete(const char mines[9][9], const char revealed[9][9]) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if ((revealed[row][col] == '?' || revealed[row][col] == '*') &&
                mines[row][col] != '*') {
                return false;
            }
        }
    }
    return true;
}

/* Internal helper function to check that the 'row' and 'column' values are
 * within range in relation to the 9x9 board */
bool valid_indices(const int &row, const int &col) {
    return row >= 0 && row < 9 && col >= 0 && col < 9;
}

/* Internal helper function to retrieve row and column indices from 'position'
 * and store in input parameters 'row' and 'col' respectively. Throw error if
 * 'position' is null or if length is not at least 2 */
void str_to_indices(const char *position, int &row, int &col) {
    // check validity of input position
    if (!position || strlen(position) < 2) {
        throw invalid_argument("[Invalid Argument Error] Position must be a "
                               "string with structure [(A-I), (1-9)]");
    }
    row = position[0] - 'A';
    col = position[1] - '1';
}

/* Internal helper function to generate string of 2 characters [(A-I),
 * (1-9)] from 'row' and 'col' inputs and store in 'position' */
void indices_to_str(const int &row, const int &col, char *position) {
    char output[] = {static_cast<char>(row + 'A'), static_cast<char>(col + '1'),
                     '\0'};
    strcpy(position, output);
}

/* Returns the number of mines around a particular square (8 adjacent squares
 * are checked). 'position' is a two-character string denoting row and column
 * board coordinates (e.g. "I8") and 'mines' is a 2D character array of mine
 * locations */
int count_mines(const char *position, const char mines[9][9]) {
    // deduce row and columns from position string
    int row, col;
    str_to_indices(position, row, col);

    // there are a maximum of 8 possibilities for adjacent squares
    int count = 0;
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            // do not check current position
            if (r == row && c == col) {
                continue;
            }
            // ensure they're in range
            if (!valid_indices(r, c)) {
                continue;
            }
            // increment count if a mine is present on an adjacent square
            if (mines[r][c] == '*') {
                count += 1;
            }
        }
    }
    return count;
}

/* Internal helper function to return the char equivalent of an input integer
 * 'i' between 0 to 9 e.g. 5 -> '5' */
char int_to_char(const int &i) {
    if (i < 0 || i > 9) {
        throw invalid_argument(
            "[Invalid Argument Error] 'i' must be between 0 to 9");
    }
    return static_cast<char>(i + '0');
}

/* Internal helper function to populate the 'square' with a blank if the count
 * of mines 'count' is zero otherwise the count itself as a character */
void add_count_to_board(char &square, const int &count) {
    if (count > 0) {
        square = int_to_char(count);
        return;
    }
    square = ' ';
}

/* Internal helper function to recursively uncover adjacent squares if the
 * current square is revealed to be blank. Base cases are:
 * - if the indices are out of range
 * - if the square has already been flagged or uncovered
 * - if a mine is present
 * - if the count of mines > 0 then we add the count to the board and stop */
void uncover(const char mines[9][9], char revealed[9][9], const int row,
             const int col) {
    // indices out of range - terminate
    if (!valid_indices(row, col)) {
        return;
    }

    // square already revealed - terminate
    if (revealed[row][col] != '?') {
        return;
    }

    // presence of mine - terminate
    if (mines[row][col] == '*') {
        return;
    }

    // generate position string
    char position[3];
    indices_to_str(row, col, position);

    // count number of mines in adjacent squares to position
    int count = count_mines(position, mines);

    // add count to the board
    add_count_to_board(revealed[row][col], count);

    // count mines > 0 - terminate
    if (count > 0) {
        return;
    }

    /* if the count is zero make recursive call for adjacent squares - note we
     * cannot just do flood-fill because a square might be only reachable
     * diagonally from a blank square (e.g. C8 can only be uncovered from D7) */
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            uncover(mines, revealed, r, c);
        }
    }
}

/* Uncovers or flags a square on the current board 'revealed' at the given
 * 'position'. If the third character of 'position' is '*' the move is to
 * flag the square in 'revealed', otherwise we uncover the square. Various
 * checks are performed:
 * - ensure 'position' is of the correct length and if length is 3, then 3rd
 * character is '*' only
 * - validate row and column indices and make sure they're actual positions
 * on the board
 * - ensure square has not already been uncovered / flagged */
MoveResult make_move(const char *position, const char mines[9][9],
                     char revealed[9][9]) {
    bool flag = false;
    int length = strlen(position);

    // if the position is an incorrect length then it is invalid
    if (length < 2 || length > 3) {
        return INVALID_MOVE;
    }

    // if 3rd character of position is '*' then move is to flag
    if (length == 3) {
        if (position[2] == '*') {
            flag = true;
        }
        // invalid if any other character present
        else {
            return INVALID_MOVE;
        }
    }

    // get row and column indices
    int row, col;
    str_to_indices(position, row, col);

    // check if row or column indices out of range
    if (!valid_indices(row, col)) {
        return INVALID_MOVE;
    }

    // redundant if square has already been uncovered / flagged
    if (revealed[row][col] != '?') {
        return REDUNDANT_MOVE;
    }

    // flag the square if it is set
    if (flag) {
        revealed[row][col] = '*';
        return VALID_MOVE;
    }

    // check if the underlying square is a mine
    if (mines[row][col] == '*') {
        return BLOWN_UP;
    }

    // count the number of mines
    int count = count_mines(position, mines);
    if (count == 0) {
        // recursive uncovering of adjacent squares
        uncover(mines, revealed, row, col);
    } else {
        revealed[row][col] = int_to_char(count);
    }

    // board is solved if all non-mine squares have been uncovered
    if (is_complete(mines, revealed)) {
        return SOLVED_BOARD;
    }

    return VALID_MOVE;
}

/* Internal helper function to copy the contents of 'board' into 'copy' */
void copy_board(const char board[9][9], char copy[9][9]) {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            copy[r][c] = board[r][c];
        }
    }
}

void update(char board[9][9], char *move, int row, int col) {
    // translate number of mines at [row, col] back to integer value
    int mines = board[row][col] - '0';

    // counters to hold number of flags / unrevealed in adjacent squares
    int unrevealed = 0;
    int flags = 0;

    // store positions
    char position[3];
    vector<char *> positions;

    // check adjacent squares
    for (int r = row - 1; r <= row + 1; r++) {
        for (int c = col - 1; c <= col + 1; c++) {
            if (r == row && c == col) {
                continue;
            }
            if (!valid_indices(r, c)) {
                continue;
            }
            if (board[r][c] == '*') {
                flags++;
            }
            if (board[r][c] == '?') {
                unrevealed++;
                indices_to_str(r, c, position);
                // TODO: figure out why duplicates are being pushed
                if (find(positions.begin(), positions.end(), position) ==
                    positions.end()) {
                    positions.push_back(position);
                }
            }
        }
    }

    /* Print positions */
    // issues from: [0, 6] A7 - positions vector = A8, A8
    // issues from: [6, 0] - positions vector = H2, H2
    // issues from [7, 0] - positions vector = I2, I2

    /* if the number of flags equals the count of mines then any question
     * marks can be turned over */
    if ((mines - flags == 0) && unrevealed > 0) {
        for (auto const &p : positions) {
            strcat(move, p);
            strcat(move, " ");
            // only use information we know
            int m = count_mines(p, board);
            int r, c;
            str_to_indices(p, r, c);
            board[r][c] = int_to_char(m);
        }
    } else if ((mines - flags == unrevealed) && unrevealed > 0) {
        for (auto const &p : positions) {
            strcat(move, p);
            strcat(move, "* ");
            int r, c;
            str_to_indices(p, r, c);
            board[r][c] = '*';
        }
    }
}

/* Determines if a safe move (no guess-work) is available starting from the
 * current playing board 'revealed'. The return value of the function is
 * true if a safe move(s) is available, and the output string 'move'
 * contains the sequence. Otherwise the return value of the function is
 * false and 'move' is empty. Logic:
 * - create a copy of the board to mark moves as we iterate through
 * - if number of flags equals count of mines then any '?' squares can be
 * revealed
 * - if (mines - flags) equals '?' squares then those squares can be
 * revealed */
bool find_safe_move(const char revealed[9][9], char *move) {

    // copy current playing board 'revealed'
    char copy[9][9];
    copy_board(revealed, copy);

    // ensure move is empty to begin with
    *move = '\0';

    // iterate through board and update as safe moves are found
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            update(copy, move, r, c);
        }
    }

    cout << "Safe moves: " << move << endl;

    if (strlen(move) > 0) {
        return true;
    }
    return false;
}