#ifndef MULTITAP_H
#define MULTITAP_H

#include <map>
#include <vector>
#include <fstream>
#include <iostream>

const std::map<char, std::vector<char>> KEYS = {
    {'0', {' '}},           {'1', {'.', ',', '!', '!'}},
    {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
    {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
    {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
    {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}},
};

int encode_character(const char ch, char *multitap);

void encode(const char* plaintext, char* multitap);

void decode(std::istream &input, std::ostream &output);

#endif