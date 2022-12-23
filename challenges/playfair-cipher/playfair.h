#ifndef PLAYFAIR_H
#define PLAYFAIR_H

const int MAX_LENGTH = 100;

// Produce output string ('output') suitable for Playfair encoding
void prepare(const char *input, char *output);

// Populate 6x6 Playfair encoding 'square' corresponding to a given 'codeword'
void grid(const char *codeword, char square[6][6]);

/* Encodes a single bigram (given by 'inchar1' and 'inchar2') using the encoding
grid 'square' and outputs to 'outchar1' and 'outchar2' */
void bigram(const char square[6][6], const char inchar1, const char inchar2,
            char &outchar1, char &outchar2);

#endif