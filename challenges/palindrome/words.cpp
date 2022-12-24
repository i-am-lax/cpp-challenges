#include "words.h"
#include <cctype>
#include <cstring>

// Copy contents of 'str1' into 'str2' backwards
void reverse(const char *str1, char *str2) {
    // iterate from the end of str1 and append to str2
    for (int idx = strlen(str1) - 1; idx >= 0; idx--) {
        *str2 = str1[idx];
        str2++;
    }
    *str2 = '\0';
}

/* Recursively compare strings 'one' and 'two' (ignoring chars that are not
 * letters and also ignoring the case) and return 1 if they're equal else 0 */
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

/* Returns 1 if 'sentence' is a palindrome, and 0 otherwise. Determined by
 * reversing the string and checking if they're equal in terms of letters */
int palindrome(const char *sentence) {
    // initialise string to hold reverse of sentence
    char rev[strlen(sentence) + 1];

    // reverse the string
    reverse(sentence, rev);

    // then compare them and return result
    return compare(sentence, rev);
}

// Swap two characters given by reference parameters 'first' and 'second'
void swap(char &first, char &second) {
    char tmp = first;
    first = second;
    second = tmp;
}

/* Return the index for the smallest (lowercase) char from 'str' from position
 * 'idx' */
int minimum(char *str, int idx) {
    int min = idx;
    for (int j = idx + 1; str[j] != '\0'; j++) {
        if (tolower(str[j]) < tolower(str[min])) {
            min = j;
        }
    }
    return min;
}

/* Sort characters in 'str' (using selection sort) and return result in
 * 'output'. Ensure lowercase characters are compared. */
void sort(const char *str, char *output) {
    // copy str into output
    strcpy(output, str);

    // if there is a char smaller than that at position 'i' then swap
    int min;
    for (int i = 0; output[i] != '\0'; i++) {
        min = minimum(output, i);
        if (min != i) {
            swap(output[i], output[min]);
        }
    }
}

/* Returns 1 if string 'str1' is an anagram of string 'str2', and 0 otherwise.
 * Determined by sorting both strings and checking if they're equal in terms of
 * letters */
int anagram(const char *str1, const char *str2) {
    // initialise strings to hold sorted values
    char sorted1[strlen(str1) + 1], sorted2[strlen(str2) + 1];

    // sort strings
    sort(str1, sorted1);
    sort(str2, sorted2);

    // once sorted, if equal we know it is an anagram
    return compare(sorted1, sorted2);
}