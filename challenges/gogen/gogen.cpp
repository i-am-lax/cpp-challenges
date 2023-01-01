#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>

#include <vector>

#include "common.h"
#include "gogen.h"

using namespace std;

// Internal helper function which allocates a dynamic 2D array
char **allocate_2D_array(int rows, int columns) {
    char **m = new char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new char[columns];
        assert(m[r]);
    }
    return m;
}

// Internal helper function which deallocates a dynamic 2D array
void deallocate_2D_array(char **m, int rows) {
    for (int r = 0; r < rows; r++)
        delete[] m[r];
    delete[] m;
}

/* Internal helper function which removes unprintable characters like carriage
 * returns and newlines from strings */
void filter(char *line) {
    while (*line) {
        if (!isprint(*line))
            *line = '\0';
        line++;
    }
}

/* loads a Gogen board from a file */
char **load_board(const char *filename) {
    char **board = allocate_2D_array(5, 6);
    ifstream input(filename);
    assert(input);
    char buffer[512];
    int lines = 0;
    input.getline(buffer, 512);
    while (input && lines < HEIGHT) {
        filter(buffer);
        if (strlen(buffer) != WIDTH)
            cout << "WARNING bad input = [" << buffer << "]" << endl;
        assert(strlen(buffer) == WIDTH);
        strcpy(board[lines], buffer);
        input.getline(buffer, 512);
        lines++;
    }
    input.close();
    return board;
}

/* saves a Gogen board to a file */
bool save_board(char **board, const char *filename) {
    ofstream out(filename);
    if (!out)
        return false;
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            out << board[r][c];
        }
        out << endl;
    }
    bool result = out.good();
    out.close();
    return result;
}

/* internal helper function for counting number of words in a file */
int count_words(const char *filename) {
    char word[512];
    int count = 0;
    ifstream in(filename);
    while (in >> word)
        count++;
    in.close();
    return count;
}

/* loads a word list from a file into a NULL-terminated array of char *'s */
char **load_words(const char *filename) {
    int count = count_words(filename);
    ifstream in(filename);
    assert(in);
    int n = 0;
    char **buffer = new char *[count + 1];   // +1 because we NULL terminate
    char word[512];
    for (; (in >> word) && n < count; n++) {
        buffer[n] = new char[strlen(word) + 1];
        strcpy(buffer[n], word);
    }
    buffer[n] = NULL;
    in.close();
    return buffer;
}

/* prints a Gogen board in appropriate format */
void print_board(char **board) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            cout << "[" << board[r][c] << "]";
            if (c < WIDTH - 1)
                cout << "--";
        }
        cout << endl;
        if (r < HEIGHT - 1) {
            cout << " | \\/ | \\/ | \\/ | \\/ |" << endl;
            cout << " | /\\ | /\\ | /\\ | /\\ |" << endl;
        }
    }
}

/* prints a NULL-terminated list of words */
void print_words(char **words) {
    for (int n = 0; words[n]; n++)
        cout << words[n] << endl;
}

/* frees up the memory allocated in load_board(...) */
void delete_board(char **board) { deallocate_2D_array(board, HEIGHT); }

/* frees up the memory allocated in load_words(...) */
void delete_words(char **words) {
    int count = 0;
    for (; words[count]; count++)
        ;
    deallocate_2D_array(words, count);
}

/* add your own function definitions here */
bool get_position(char **board, const char ch, int &row, int &column) {
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (board[r][c] == ch) {
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

int count_character(char **board, const char ch) {
    int count = 0;
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (board[r][c] == ch) {
                count += 1;
            }
        }
    }
    return count;
}

bool is_complete(char **board) {
    vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                            'S', 'T', 'U', 'V', 'W', 'X', 'Y'};
    for (auto const &l : letters) {
        if (count_character(board, l) != 1) {
            return false;
        }
    }
    return true;
}

bool is_valid_indices(int row, int col) {
    if (row < 0 || row > HEIGHT - 1) {
        return false;
    }
    if (col < 0 || col > WIDTH - 1) {
        return false;
    }
    return true;
}

// bool adjacent_letter(char **board, int row, int col, char *word) {
//     if (*word == '\0') {
//         return true;
//     }
//     if (board[row][col] != *word) {
//         return false;
//     }

//     for (int r = row - 1; r <= row + 1; r++) {
//         for (int c = col - 1; col <= col + 1; c++) {
//             if (r == row && c == col) {
//                 continue;
//             }
//             if (!is_valid_indices(r, c)) {
//                 continue;
//             }
//             if (adjacent_letter(board, r, c, word + 1)) {
//                 cout << "Letter " << *(word+1) << "found at [" << r << ", "
//                 << c << endl; return true;
//             }
//         }
//     }
//     return false;
// }

bool is_adjacent(int r1, int c1, int r2, int c2) {
    int rdiff = abs(r1 - r2), cdiff = abs(c1 - c2);
    if (rdiff == 0 && cdiff == 0) {
        return false;
    }
    if (rdiff > 1 || cdiff > 1) {
        return false;
    }
    return true;
}

bool word_on_board(char **board, char *word) {
    // get position of the first letter
    int row, col, next_row, next_col;
    get_position(board, *word, row, col);

    while (*(word + 1) != '\0') {
        get_position(board, *(word + 1), next_row, next_col);
        if (!is_adjacent(row, col, next_row, next_col)) {
            return false;
        }
        word++;
        row = next_row;
        col = next_col;
    }
    return true;
}

/* Returns true if the given 'board' represents a solution to the Gogen puzzle
 * with the given list of 'words'. The first check performed is to ensure that
 * all letters from A-Y are present (and only once). Next each word in 'words'
 * is checked to see if it is represented on the board with letters in any of
 * the adjacent positions  */
bool valid_solution(char **board, char **words) {
    // need to make sure that all letters of alphabet (except Z) are present
    if (!is_complete(board)) {
        return false;
    }
    // need to check if the words themselves are present on the board
    for (int n = 0; words[n]; n++) {
        if (!word_on_board(board, words[n])) {
            return false;
        }
    }
    return true;
}

void update(char **board, const char ch, Mask &mask) {
    int row, col;
    // location is fixed on the board
    if (get_position(board, ch, row, col)) {
        mask.set_all(false);
        mask.set_element(row, col, true);
        return;
    }
    // free positions on the board
    for (int r = 0; r < HEIGHT; r++) {
        for (int c = 0; c < WIDTH; c++) {
            if (isalpha(board[r][c])) {
                mask.set_element(r, c, false);
            }
        }
    }
    // confirmed letter position - add to board
    if (mask.count() == 1) {
        mask.get_position(true, row, col);
        board[row][col] = ch;
    }
}

void neighbourhood_intersect(Mask &one, Mask &two) {
    Mask onbr = one.neighbourhood();
    Mask tnbr = two.neighbourhood();

    one.intersect_with(tnbr);
    two.intersect_with(onbr);
}

bool aux(char **board, char **words) {
    // board is complete - terminate
    if (valid_solution(board, words)) {
        return true;
    }
}

bool solve_board(char **board, char **words) {
    vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                            'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                            'S', 'T', 'U', 'V', 'W', 'X', 'Y'};
    vector<Mask> masks;
    for (auto const &l : letters) {
        Mask m;
        update(board, l, m);
        masks.push_back(m);
    }
}