#include "soundex.h"
#include <cctype>
#include <cstring>
#include <sstream>

/* Generates the Soundex encoding corresponding to a given 'surname' and writes
 * result to 'soundex':
 - letter at the start of the Soundex code is the first letter of the surname
 - remaining digits looked up from 'CODES' map (adjacent letters with same code
 treated as one)
 - empty slots padded with zeros */
void encode(const char *surname, char *soundex) {
    // initialise variables for code lookups
    char code, prev = '0';

    // pad soundex code with zeros
    strcpy(soundex, "0000");

    // index for soundex
    int idx = 0;
    while (*surname != '\0' && soundex[idx] != '\0') {
        // add first letter of surname
        if (idx == 0) {
            soundex[idx] = toupper(*surname);
            idx++;
        }
        // lookup code and and ensure it is not equal to the previous one
        else if (CODES.count(*surname)) {
            code = CODES.at(*surname);
            if (code != prev) {
                soundex[idx] = code;
                prev = code;
                idx++;
            }
        }
        surname++;
    }
}

/* Recursively compare two Soundex codes ('one' and 'two') and return 1 if the
 * codes are the same otherwise return 0 */
int compare(const char *one, const char *two) {
    // if we have reached the ends of the strings then they're equal
    if (*one == '\0' && *two == '\0') {
        return 1;
    }
    // if the characters are equal we can compare the next one
    if (*one == *two) {
        return compare(one + 1, two + 1);
    }
    return 0;
}

/* Returns the number of words in 'sentence' that have the same Soundex encoding
 * as 'surname' */
int count(const char *surname, const char *sentence) {
    // count for output
    int count = 0;

    // declare strings to hold sentence copy and soundex codes
    char sentence_copy[strlen(sentence) + 1], soundex[5], word_soundex[5];

    // create copy of sentence
    strcpy(sentence_copy, sentence);

    // get soundex code for surname
    encode(surname, soundex);

    // tokenise sentence on space, comma or fullstop
    char delimiter[] = " ,.";
    char *word = strtok(sentence_copy, delimiter);
    while (word) {
        // get encoding for word
        encode(word, word_soundex);
        // increment count if encodings equal
        if (compare(word_soundex, soundex)) {
            count++;
        }
        // get the next token
        word = strtok(NULL, delimiter);
    }
    return count;
}