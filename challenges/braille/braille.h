#ifndef BRAILLE_H
#define BRAILLE_H

#include <map>

const int MAX_LENGTH = 512;

// Braille letter encodings
const std::map<char, const char *> MAPPING = {
    {'~', "......"}, {'^', ".....0"}, {'#', "..0000"}, {'.', ".0..00"},
    {',', ".0...."}, {';', ".00..."}, {'-', "..0..0"}, {'!', ".00.0."},
    {'?', ".00..0"}, {'(', ".00.00"}, {')', ".00.00"}, {'a', "0....."},
    {'b', "00...."}, {'c', "0..0.."}, {'d', "0..00."}, {'e', "0...0."},
    {'f', "00.0.."}, {'g', "00.00."}, {'h', "00..0."}, {'i', ".0.0.."},
    {'j', ".0.00."}, {'k', "0.0..."}, {'l', "000..."}, {'m', "0.00.."},
    {'n', "0.000."}, {'o', "0.0.0."}, {'p', "0000.."}, {'q', "00000."},
    {'r', "000.0."}, {'s', ".000.."}, {'t', ".0000."}, {'u', "0.0..0"},
    {'v', "000..0"}, {'w', ".0.000"}, {'x', "0.00.0"}, {'y', "0.0000"},
    {'z', "0.0.00"},
};

/* Produces a string (given by 'braille') that represents the Braille encoding
 * of a single input character (given by 'ch'). In the string encoding, a
 * Braille cell is represented as 6 consecutive characters, one for each dot
 * position. Raised dots are encoded as 'O' and unraised dots as '.'. The length
 * of 'braille' is returned */
int encode_character(const char ch, char *braille);

// Produces the Braille encoding of 'plaintext' and stores in 'braille'
void encode(const char *plaintext, char *braille);

/* Writes the corresponding sequence of 3x2 Braille cells for 'plaintext' to an
 * output stream 'output' (such as cout or a file) */
void print_braille(const char *plaintext, std::ostream &output);

#endif