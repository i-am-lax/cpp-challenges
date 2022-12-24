#include "words.h"
#include <cctype>
#include <cstring>

// TO DELETE
#include <iostream>
using namespace std;

void reverse(const char *str1, char *str2) {
    // iterate from the end of str1 and append to str2
    for (int idx = strlen(str1) - 1; idx >= 0; idx--) {
        *str2 = str1[idx];
        str2++;
    }
    *str2 = '\0';
}

/*
void prepare_string(const char* str, char* output) {
    while (*str != '\0') {
        if (isalpha(*str)) {
            *output = *str;
            output++;
        }
        str++;
    }
    *output = '\0';
}
*/

int compare(const char *one, const char *two) {
    // if the chars are not letters then increment until they are
    while (!isalpha(*one) && *one != '\0') {
        one++;
    }
    while (!isalpha(*two) && *two != '\0') {
        two++;
    }
    // once we've reached end of string we know they're equal
    if (*one == '\0' && *two == '\0') {
        return 1;
    }
    // if chars match (irrespective of case) then compare next ones
    if (tolower(*one) == tolower(*two)) {
        return compare(one + 1, two + 1);
    }
    return 0;
}

int palindrome(const char *sentence) {
    // initialise string to hold reverse of sentence
    char rev[strlen(sentence)];

    // reverse the string
    reverse(sentence, rev);

    // then compare them and return result
    return compare(sentence, rev);
}