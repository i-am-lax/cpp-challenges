#ifndef PLAYFAIR_H
#define PLAYFAIR_H

const int MAX_LENGTH = 100;

void prepare(const char *input, char *output);
void grid(const char *codeword, char square[6][6]);

#endif