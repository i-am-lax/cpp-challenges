#ifndef MULTITAP_H
#define MULTITAP_H

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

// Constraint on maximum string length
const int MAX_LENGTH = 10;

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

/* Produces the multitap encoding of the input string 'plaintext' to be encoded.
 * The second parameter ('multitap') contains the corresponding multitap-encoded
 * string (taking letter case and pauses into account) */
void encode(const char *plaintext, char *multitap);

/* Takes a multitap-encoded input stream and writes a decoded plaintext version
 * to an output stream. The first parameter ('input') is the input stream (e.g.
 * cin or a file input stream) and the second parameter is the output stream
 * (e.g. cout or a file output stream) */
void decode(std::istream &input, std::ostream &output);

#endif