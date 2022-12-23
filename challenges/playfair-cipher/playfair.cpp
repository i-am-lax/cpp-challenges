#include "playfair.h"
#include <cctype>

// TO DELETE
#include <iostream>
using namespace std;

void prepare(const char* input, char* output) {
    // length of string
    int length = 0;

    // add alphanumerical chars to output string
    while (*input != '\0') {
        if (isalnum(*input)) {
            *output = toupper(*input);
            output++;
            length++;
        }
        input++;
    }
    // if length is odd then append 'X'
    if (length % 2 != 0) {
        *output = 'X';
        output++;
    }
    // add sentinel character '\0'
    *output = '\0';
}