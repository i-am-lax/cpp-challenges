#include "piglatin.h"
#include <cctype>
#include <iostream>

using namespace std;

int findFirstVowel(const char *word) {
    for (int idx = 0; word[idx] != '\0'; idx++) {
        if (VOWELS.count(tolower(word[idx]))) {
            return idx;
        }
        // 'y' considered vowel if it is not first or last letter
        else if (idx > 0 && idx < strlen(word) - 1 &&
                 tolower(word[idx]) == 'y') {
            return idx;
        }
    }
    return -1;
}

void translateWord(const char *english, char *piglatin) {
    // ensure char array is clear
    *piglatin = '\0';

    // copy word
    strcpy(piglatin, english);

    // if word begins with digit then leave as is
    if (isdigit(*english)) {
        return;
    }
    // find first vowel in word
    int first = findFirstVowel(english);

    // if word contains no vowels, we simply append "ay"
    if (first == -1) {
        strcat(piglatin, "ay");
    }
    // if first letter is vowel, add "way" to it
    else if (first == 0) {
        strcat(piglatin, "way");
    }
    // if vowel is not the first letter
    else {
        // check if word is capitalised
        bool isCapital = isupper(*english);

        // move all the characters before the vowel to the end of the word
        strcpy(piglatin, (piglatin + first));
        strncat(piglatin, english, first);
        strcat(piglatin, "ay");

        /* ensure letters are lowercase except for first letter if initially
         * capitalised */
        if (isCapital) {
            *piglatin = toupper(*piglatin);
        }
        while (*piglatin != '\0') {
            piglatin++;
            *piglatin = tolower(*piglatin);
        }
    }
}

void translateStream(istream &input, ostream &cout) {
    // declare variables
    char ch, word[MAX_LENGTH], piglatin[MAX_LENGTH];

    // base case: return if end of stream reached
    input.get(ch);
    if (!input) {
        return;
    }

    // if punctuation or new line then output right away
    if (ispunct(ch) || ch == '\n') {
        cout << ch;
    }
    /* if we encounter a letter then keep incrementing until we capture the word
     * and convert to piglatin */
    else if (isalnum(ch)) {
        int idx = 0;
        while (isalnum(ch)) {
            word[idx] = ch;
            idx++;
            input.get(ch);
        }
        word[idx] = '\0';
        translateWord(word, piglatin);
        cout << piglatin << ch;
    }
    translateStream(input, cout);
}