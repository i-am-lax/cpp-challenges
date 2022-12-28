#include <cstring>
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

void text_to_binary(const char *str, char *binary) {
    if (*str == '\0') {
        *binary = '\0';
        return;
    }
    ascii_to_binary(*str, binary);
    text_to_binary(str + 1, binary + 8);
}

void binary_to_text(const char *binary, char *str) {
    if (*binary == '\0') {
        *str = '\0';
        return;
    }
    *str = binary_to_ascii(binary);
    binary_to_text(binary + 8, str + 1);
}

char parity(const char b1, const char b2, const char b3) {
    if ((b1 + b2 + b3) % 2 == 0) {
        return '0';
    }
    return '1';
}

void add_error_correction(const char *data, char *corrected) {
    // ensure input data is of correct length
    if (strlen(data) == 0 || strlen(data) % 4 != 0) {
        cerr << "[Error] Input data should be a multiple of 4 bits." << endl;
    }
    while (*data != '\0') {
        // 7-bit code word in the order: c1, c2, d1, c3, d2, d3, d4
        corrected[0] =
            parity(data[0], data[1], data[3]);   // c1 = parity(d1, d2, d4)
        corrected[1] =
            parity(data[0], data[2], data[3]);   // c2 = parity(d1, d3, d4)
        corrected[2] = data[0];
        corrected[3] =
            parity(data[1], data[2], data[3]);   // c3 = parity(d2, d3, d4)
        corrected[4] = data[1];
        corrected[5] = data[2];
        corrected[6] = data[3];

        // adjust pointers
        data += 4;
        corrected += 7;
    }
    *corrected = '\0';
}