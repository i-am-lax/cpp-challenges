#include <iostream>

using namespace std;

/* Converts a character 'ch' into a binary string representation ('binary') */
void ascii_to_binary(char ch, char *binary) {
    for (int n = 128; n; n >>= 1)
        *binary++ = (ch & n) ? '1' : '0';
    *binary = '\0';
}

/* Converts a binary string representation ('binary') into a character */
char binary_to_ascii(const char *binary) {
    int ch = 0;
    for (int n = 0, slide = 128; n < 8; n++, slide >>= 1) {
        if (binary[n] == '1')
            ch = ch | slide;
    }
    return ch;
}

void text_to_binary(const char* str, char* binary) {
    if (*str == '\0') {
        *binary = '\0';
        return;
    }
    ascii_to_binary(*str, binary);
    text_to_binary(str + 1, binary + 8);
}

void binary_to_text(const char* binary, char* str) {
    if (*binary == '\0') {
        *str = '\0';
        return;
    }
    *str = binary_to_ascii(binary);
    binary_to_text(binary + 8, str + 1);
}