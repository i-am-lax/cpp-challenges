#include "soundex.h"
#include <cctype>

// TO DELETE
#include <iostream>
using namespace std;

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