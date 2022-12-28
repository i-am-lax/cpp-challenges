#ifndef CORRECT_H
#define CORRECT_H

/* Takes in a character ('ch') and returns (in the output parameter 'binary') a
 * string representation of the 8-bit binary number corresponding to the ASCII
 * code of 'ch' */
void ascii_to_binary(char letter, char *output);

/* Converts a string representing a binary number (in the input parameter
 * 'binary') to an ASCII character */
char binary_to_ascii(const char *binary);

/* Converts a string of characters (the input parameter 'str') into a
 * corresponding stream of bits (the output string parameter 'binary') */
void text_to_binary(const char *str, char *binary);

/* Converts a stream of bits (the input string parameter 'binary') into a
 * corresponding character string (the output parameter 'str') */
void binary_to_text(const char *binary, char *str);

/* Outputs (in the output string parameter 'corrected') an error-corrected
version of a data bit stream (the input string parameter 'data'). There are 7
bits for every 4 bits of data. */
void add_error_correction(const char *data, char *corrected);

/* Outputs (in the output string parameter 'decoded') an error-corrected version
 * of a received bit stream (the input string parameter 'received'). The
 * function returns the number of errors corrected */
int decode(const char *received, char *decoded);

int binary_to_decimal(const char* binary);

#endif