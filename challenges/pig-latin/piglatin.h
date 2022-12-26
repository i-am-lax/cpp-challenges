#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <set>

const std::set<char> VOWELS = {'a', 'e', 'i', 'o', 'u'};

int findFirstVowel(const char *word);
void translateWord(const char *english, char *piglatin);

#endif