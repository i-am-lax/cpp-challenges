#include "playfair.h"
#include <cctype>

// TO DELETE
#include <iostream>
using namespace std;

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
bool char_exists(char square[6][6], const char ch) {
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            if (square[row][col] == ch) {
                return true;
            }
        }
    }
    return false;
}

/* Populate 6x6 Playfair encoding 'square' corresponding to a given 'codeword' */
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