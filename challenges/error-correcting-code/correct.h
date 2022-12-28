#ifndef CORRECT_H
#define CORRECT_H

/* Takes in a character ('ch') and returns (in the output parameter 'binary') a
 * string representation of the 8-bit binary number corresponding to the ASCII
 * code of 'ch' */
void ascii_to_binary(char letter, char *output);

/* Converts a string representing a binary number (in the input parameter
 * 'binary') to an ASCII character */
char binary_to_ascii(const char *binary);

void text_to_binary(const char* str, char* binary);

void binary_to_text(const char* binary, char* str);

char parity(const char b1, const char b2, const char b3);

void add_error_correction(const char* data, char* corrected);

#endif