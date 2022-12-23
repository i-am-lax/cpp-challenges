#include "playfair.h"
#include <cctype>

// TO DELETE
#include <iostream>
using namespace std;

/* Use pointer arithmetic to produce an output string ('output') suitable for
 * Playfair encoding by removing non-alphanumeric characters, converting letters
 * to uppercase and adding an 'X' if the length is odd.  */
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

void grid(const char *codeword, char square[6][6]) {
    /* generate grid sequence:
     * - we know our sequence has a max length of 36 (25 letters + 10 digits)
     * - length(set) >= length(sequence)
     * - add chars from set to sequence if they're not already in sequence to
     * get unique values */
    char set[MAX_LENGTH], sequence[37];

    // ordering comprises of codeword, letters and then digits
    strcpy(set, codeword);
    strcat(set, "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

    // if char from set not in sequence then append
    int seqidx = 0;
    for (int setidx = 0; set[setidx] != '\0'; setidx++) {
        if (!strchr(sequence, set[setidx])) {
            sequence[seqidx] = set[setidx];
            sequence[seqidx+1] = '\0';
            seqidx++;
        }
    }

    // add char sequence to grid
    seqidx = 0;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            square[row][col] = sequence[seqidx];
            seqidx++;
        }
    }
}