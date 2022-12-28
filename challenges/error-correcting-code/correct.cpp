#include <cmath>
#include <cstring>

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

/* Recursively converts a string of characters ('str') into a corresponding
 * stream of bits ('binary') */
void text_to_binary(const char *str, char *binary) {
    if (*str == '\0') {
        *binary = '\0';
        return;
    }
    ascii_to_binary(*str, binary);
    text_to_binary(str + 1, binary + 8);
}

/* Recursively converts a stream of bits ('binary') into a corresponding
 * character string ('str') */
void binary_to_text(const char *binary, char *str) {
    if (*binary == '\0') {
        *str = '\0';
        return;
    }
    *str = binary_to_ascii(binary);
    binary_to_text(binary + 8, str + 1);
}

/* Internal helper function to recursively convert a stream of bits ('binary')
 * into its decimal value */
int binary_to_decimal(const char *binary) {
    if (*binary == '\0') {
        return 0;
    }
    return (*binary - '0') * pow(2, strlen(binary) - 1) +
           binary_to_decimal(binary + 1);
}

/* Internal helper function to compute the parity (as a char) for 3 input
 * values:
 * - Return '0' if (a + b + c) is even
 * - Return '1' if (a + b + c) is odd */
char parity(const char b1, const char b2, const char b3) {
    return ((b1 + b2 + b3) % 2 == 0) ? '0' : '1';
}

/* Internal helper function which has been overloaded to compute the parity (as
 * a char) for 4 input values */
char parity(const char b1, const char b2, const char b3, const char b4) {
    return ((b1 + b2 + b3 + b4) % 2 == 0) ? '0' : '1';
}

/* Internal helper function to flip the input char 'bit' */
void flip(char &bit) { bit == '0' ? bit = '1' : bit = '0'; }

/* Recursively output (in the output string parameter 'corrected') an
error-corrected version of a data bit stream (the input string parameter
'data'). There are 7 bits (c1, c2, d1, c3, d2, d3, d4) per 4 bits which includes
3 check bits (c1, c2, c3) and 4 data bits (d1, d2, d3, d4) */
void add_error_correction(const char *data, char *corrected) {
    if (*data == '\0') {
        *corrected = '\0';
        return;
    }
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

    // recursive call with adjusted pointers
    add_error_correction(data + 4, corrected + 7);
}

/* Recursively output (in output string 'decoded') an error-corrected version
 * of a received bit stream (the input string 'received'), and return return the
 * number of errors corrected */
int decode(const char *received, char *decoded) {
    // base case
    if (*received == '\0') {
        *decoded = '\0';
        return 0;
    }

    /* create array of the error-corrected stream of 7 bits from 'received' so
     * we can flip the bit incase of corruption without modifying original */
    char stream[8];
    strncpy(stream, received, 7);

    // compute three parity check values
    char p1 = parity(stream[3], stream[4], stream[5],
                     stream[6]);   // p1 = parity(b4, b5, b6, b7)
    char p2 = parity(stream[1], stream[2], stream[5],
                     stream[6]);   // p2 = parity(b2, b3, b6, b7)
    char p3 = parity(stream[0], stream[2], stream[4],
                     stream[6]);   // p3 = parity(b1, b3, b5, b7)

    // if one or more of p1, p2, p3 are 1, there has been an error
    bool error = (p1 == '1') || (p2 == '1') || (p3 == '1');

    if (error) {
        /* convert parity values to decimal equivalent and flip bit at that
         * position */
        char binary[] = {p1, p2, p3, '\0'};
        int decimal = binary_to_decimal(binary);
        flip(stream[decimal - 1]);
    }

    // original 4-bit data stream recovered as: b3, b5, b6, b7
    decoded[0] = stream[2];
    decoded[1] = stream[4];
    decoded[2] = stream[5];
    decoded[3] = stream[6];

    // recursive call with adjusted pointers
    return error + decode(received + 7, decoded + 4);
}