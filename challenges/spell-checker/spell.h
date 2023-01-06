#ifndef SPELL_H
#define SPELL_H

#include <string>

// Assume maximum word length of 512 characters
const int MAX_LENGTH = 512;

// File containing words and frequencies
const std::string WORDS = "words.dat";

int frequency(const std::string target);
int edit_distance(const std::string a, const std::string b, int limit = MAX_LENGTH);
bool spell_correct(const std::string word, char* fixed);

#endif