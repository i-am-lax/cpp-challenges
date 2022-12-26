#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <iostream>
#include <set>

// maximum length of string
const int MAX_LENGTH = 100;

// set of vowels
const std::set<char> VOWELS = {'a', 'e', 'i', 'o', 'u'};

/* Returns the position of the first "vowel" in the input 'word'. If the word
 * does not contain a vowel then the function returns -1. */
int findFirstVowel(const char *word);

/* Produces a Pig Latin translation for a given English word ('input') and
 * stores output in 'piglatin' */
void translateWord(const char *english, char *piglatin);

/* Takes words from an input stream (e.g. cin or a file input stream) and writes
 * a corresponding Pig Latin translation to an output stream */
void translateStream(std::istream &input, std::ostream &cout);

#endif