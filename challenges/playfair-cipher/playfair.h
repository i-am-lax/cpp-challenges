#ifndef PLAYFAIR_H
#define PLAYFAIR_H

const int MAX_LENGTH = 100;

// Produce output string ('output') suitable for Playfair encoding
void prepare(const char *input, char *output);

// Populate 6x6 Playfair encoding 'square' corresponding to a given 'codeword'
void grid(const char *codeword, char square[6][6]);

#endif