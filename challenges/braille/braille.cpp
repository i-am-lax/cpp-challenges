#include "braille.h"

// TO DELETE
#include <iostream>
using namespace std;

int encode_character(const char ch, char *braille) {
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

void encode(const char *plaintext, char *braille) {
    // base case: terminate once we reach end of plaintext
    if (*plaintext == '\0') {
        return;
    }
    // shift the braille pointer by however many chars required for encoding
    int size = encode_character(*plaintext, braille);
    encode(plaintext + 1, braille + size);
}