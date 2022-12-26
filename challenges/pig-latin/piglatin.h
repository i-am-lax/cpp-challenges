#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <iostream>
#include <set>

const int MAX_LENGTH = 100;

const std::set<char> VOWELS = {'a', 'e', 'i', 'o', 'u'};

int findFirstVowel(const char *word);
void translateWord(const char *english, char *piglatin);
void translateStream(std::istream &input, std::ostream &cout);

#endif