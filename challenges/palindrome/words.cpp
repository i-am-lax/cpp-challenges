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
    char rev[strlen(sentence) + 1];

    // reverse the string
    reverse(sentence, rev);

    // then compare them and return result
    return compare(sentence, rev);
}

void sort(const char *str, char *output) {
    // copy str into output
    strcpy(output, str);

    int min;
    char tmp;
    for (int i = 0; output[i] != '\0'; i++) {
        min = i;
        for (int j = i + 1; output[j] != '\0'; j++) {
            if (tolower(output[j]) < tolower(output[min])) {
                min = j;
            }
        }
        if (min != i) {
            char tmp = output[i];
            output[i] = output[min];
            output[min] = tmp;
        }
    }
}

int anagram(const char *str1, const char *str2) {
    // initialise strings to hold sorted values
    char sorted1[strlen(str1) + 1], sorted2[strlen(str2) + 1];

    // sort strings
    sort(str1, sorted1);
    // cout << "Sorted value for '" << str1 << "' is: " << sorted1 << endl;
    sort(str2, sorted2);
    // cout << "Sorted value for '" << str2 << "' is: " << sorted2 << endl;

    // once sorted, if equal we know it is an anagram
    return compare(sorted1, sorted2);
}