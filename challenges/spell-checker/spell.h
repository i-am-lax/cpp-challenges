#ifndef SPELL_H
#define SPELL_H

#include <string>

// Assume maximum word length of 512 characters
const int MAX_LENGTH = 512;

// File containing words and frequencies
const std::string WORDS = "words.dat";

/* Takes as input a word 'target' and returns the number of times the word
 * appears in the corpus (WORDS). If the word cannot be found then the function
 * returns 0 */
int frequency(const std::string target);

/* Returns the Damerau–Levenshtein distance between input strings 'a' and 'b' */
int edit_distance(const std::string a, const std::string b,
                  int limit = MAX_LENGTH);

/* Suggests a spell checked variant of the input 'word' and returns true with
 * the correction in 'fixed' if one is found, otherwise 'fixed' stores the
 * original 'word' and the function returns false */
bool spell_correct(const std::string word, char *fixed);

#endif