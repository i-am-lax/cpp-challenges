#ifndef WORDS_H
#define WORDS_H

// Copy contents of 'str1' into 'str2' backwards
void reverse(const char *str1, char *str2);

/* Compare strings 'one' and 'two' (ignoring characters that are not letters and
 * also ignoring the case) and return 1 if they're equal else 0 */
int compare(const char *one, const char *two);

// Returns 1 if 'sentence' is a palindrome, and 0 otherwise
int palindrome(const char *sentence);

// Returns 1 if string 'str1' is an anagram of string 'str2', and 0 otherwise
int anagram(const char *str1, const char *str2);

#endif