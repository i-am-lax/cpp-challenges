#ifndef MULTITAP_H
#define MULTITAP_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

/* Mapping from digits to set of characters in their correct positions for
 * multitap encoding */
const std::map<char, std::vector<char>> KEYS = {
    {'0', {' '}},           {'1', {'.', ',', '!', '!'}},
    {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
    {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
    {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
    {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}},
};

/* Produces the multitap encoding of a single input character 'ch' (ignoring
case) and writes it to 'multitap'. The number of keystrokes required to
encode 'ch' is returned which is the length of the multitap encoding */
int encode_character(const char &ch, char *multitap);

void encode(const char *plaintext, char *multitap);

void decode(std::istream &input, std::ostream &output);

#endif