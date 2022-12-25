#include "braille.h"
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

/* Produces the Braille encoding for 'ch' and stores in 'braille'. Raised dots
 * are encoded as 'O' and unraised dots as '.'. The length of the encoding is
 * returned. Logic:
 * - Digits are preceded with the number sign and then mapped to 'a' to 'j'
 * - Letters are preceded with the capital sign if uppercase and then mapped to
 * lowercase encodings
 * - Punctuation is mapped to their encoding if it exists otherwise given a
 * default value  */
int encode_character(const char ch, char *braille) {
    // ensure braille is clear
    *braille = '\0';

    /* for digits add number sign (given by '#' in mapping) and then letter
     * corresponding to digit (a -> 1, ..., i -> 9, j -> 0) */
    if (isdigit(ch)) {
        strcpy(braille, MAPPING.at('#'));
        if (ch == '0') {
            strcat(braille, MAPPING.at('j'));
        } else {
            strcat(braille, MAPPING.at(ch + '0'));
        }
    }
    /* for letters add capital sign (given by '^' in mapping) for uppercase
     * letters and then retrieve lowercase value */
    else if (isalpha(ch)) {
        if (isupper(ch)) {
            strcpy(braille, MAPPING.at('^'));
        }
        strcat(braille, MAPPING.at(tolower(ch)));
    }
    // for punctuation retrieve mapping if it exists
    else if (MAPPING.count(ch)) {
        strcpy(braille, MAPPING.at(ch));
    }
    // otherwise add default encoding (given by '~' in mapping)
    else {
        strcpy(braille, MAPPING.at('~'));
    }
    return strlen(braille);
}

/* Recursively encodes the characters given in 'plaintext' and stores in
 * 'braille' */
void encode(const char *plaintext, char *braille) {
    // base case: terminate once we reach end of plaintext
    if (*plaintext == '\0') {
        return;
    }
    // shift the braille pointer by however many chars required for encoding
    int size = encode_character(*plaintext, braille);
    encode(plaintext + 1, braille + size);
}

/* Writes the corresponding sequence of 3x2 Braille cells for 'plaintext' to an
 * output stream 'output' */
void print_braille(const char *plaintext, ostream &output) {
    // get encoding for plaintext
    char braille[MAX_LENGTH];
    encode(plaintext, braille);

    /* for each row of 3x2 grid we get column values we need:
    - row 1 -> idx 0 and 3
    - row 2 -> idx 1 and 4
    - row 3 -> idx 2 and 5 */
    for (int offset = 0; offset < 3; offset++) {
        for (int i = 0; braille[i] != '\0'; i++) {
            if (i % 6 == offset) {
                output << braille[i] << braille[i + 3] << " ";
            }
        }
        output << endl;
    }
    /* align the plaintext chars making sure additional spaces accounted for if
     * char is uppercase or a digit as the encoding would have 2 3x2
     * grids associated to it */
    for (int i = 0; plaintext[i] != '\0'; i++) {
        if (isupper(plaintext[i]) || isdigit(plaintext[i])) {
            // pad with 3 additional spaces
            output << "   ";
        }
        cout << plaintext[i] << "  ";
    }
    output << endl;
}