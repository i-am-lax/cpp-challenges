#include "playfair.h"
#include <cctype>

// TO DELETE
#include <iostream>
using namespace std;

// Position data structure to hold row and column values on encoding grid
struct Position {
    int row, col;
};

/* Use pointer arithmetic to produce an output string ('output') suitable for
 * Playfair encoding by removing non-alphanumeric characters, converting letters
 * to uppercase and adding an 'X' if the length is odd. */
void prepare(const char *input, char *output) {
    // length of string
    int length = 0;

    // add alphanumerical chars to output string
    while (*input != '\0') {
        if (isalnum(*input)) {
            *output = toupper(*input);
            output++;
            length++;
        }
        input++;
    }
    // if length is odd then append 'X'
    if (length % 2 != 0) {
        *output = 'X';
        output++;
    }
    // add sentinel character '\0'
    *output = '\0';
}

/* Return true if char 'ch' exists in 6x6 grid 'square' */
bool char_exists(const char square[6][6], const char ch) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            if (square[row][col] == ch) {
                return true;
            }
        }
    }
    return false;
}

/* Populate 6x6 Playfair encoding 'square' corresponding to a given 'codeword'
 */
void grid(const char *codeword, char square[6][6]) {
    // create full char set ordered by: codeword, letters and digits
    char charset[MAX_LENGTH];
    strcpy(charset, codeword);
    strcat(charset, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    // add char sequence to grid if it doesn't already exist
    int idx = 0;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            // increment index until we find one to add
            while (char_exists(square, charset[idx])) {
                idx++;
            }
            square[row][col] = charset[idx];
            idx++;
        }
    }
}

/* Find position on encoding grid 'square' for character 'ch' */
Position get_position(const char square[6][6], const char ch) {
    Position pos;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            if (square[row][col] == ch) {
                pos.row = row;
                pos.col = col;
            }
        }
    }
    return pos;
}

/* Encodes a single bigram ('inchar1' and 'inchar2') using the encoding
grid 'square' and outputs to 'outchar1' and 'outchar2' */
void bigram(const char square[6][6], const char inchar1, const char inchar2,
            char &outchar1, char &outchar2) {

    // find positions on grid for inchar1 and inchar2
    Position pin1 = get_position(square, inchar1);
    Position pin2 = get_position(square, inchar2);

    // construct positions for outchar1 and outchar2
    Position pout1 = {pin1.row, pin2.col}, pout2 = {pin2.row, pin1.col};

    // get corresponding char at position and write to outchar1 and outchar2
    outchar1 = square[pout1.row][pout1.col];
    outchar2 = square[pout2.row][pout2.col];
}