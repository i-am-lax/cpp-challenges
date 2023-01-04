#include "stamp.h"
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>

using namespace std;

/* Pre-supplied internal helper function which transforms raw binary hash value
 * 'str' into human-friendly hexademical form and writes to 'output' */
void convert_hash(const unsigned char *str, char *output, int hash_length) {
    char append[16];
    strcpy(output, "");
    for (int n = 0; n < hash_length; n++) {
        sprintf(append, "%02x", str[n]);
        strcat(output, append);
    }
}

/* Pre-supplied helper function to generate the SHA1 hash of input string 'text'
 * into output parameter 'digest' */
void text_to_SHA1_digest(const char *text, char *digest) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char *) text, strlen(text), hash);
    convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* Returns the number of leading zeros in the hexadecimal string (representing a
 * hash) 'digest' or -1 if the characters that are not valid hexadecimal digits
 * ('0' to '9' and 'a' to 'f') */
int leading_zeros(const char *digest) {
    // counter for zeros
    int count = 0;
    bool front = true;

    while (*digest != '\0') {
        // check if the character is a valid hexadecimal digit
        if (!isxdigit(*digest)) {
            return -1;
        }
        // increment if sequence of zeros continues
        if (*digest == '0' && front) {
            count++;
        }
        // another non-zero character encountered means stop counting
        else {
            front = false;
        }
        digest++;
    }
    return count;
}

/* Writes the SHA1 digest of the contents of 'filename' and returns true. If the
 * file cannot be opened then the function returns false with digest set to
 * "error" */
bool file_to_SHA1_digest(const char *filename, char *digest) {
    // instantiate input file stream
    ifstream in(filename);
    // check if the file opened successfully
    if (in.fail()) {
        strcpy(digest, "error");
        return false;
    }
    // declare variables for keeping track of file contents
    int idx = 0;
    char ch, contents[MAX_LENGTH];

    // retrieve each character from file stream and write to 'contents'
    in.get(ch);
    while (!in.eof()) {
        contents[idx] = ch;
        in.get(ch);
        idx++;
    }
    in.close();
    contents[idx] = '\0';

    // generate SHA1 digest
    text_to_SHA1_digest(contents, digest);

    return true;
}

/* Internal helper function to construct a valid header from the email address
 * ('recipient'), the SHA1 digest ('digest') and the 'counter', all separated
 * with colons. The output is written to 'header' in the format:
 * <recipient email address>:<SHA1 digest of message body>:<counter> */
void build_header(const char *recipient, const char *digest, const int &counter,
                  char *header) {
    // clear contents of header
    strcpy(header, "");

    // append contents in order
    strcpy(header, recipient);
    strcat(header, ":");
    strcat(header, digest);
    strcat(header, ":");
    strcat(header, to_string(counter).c_str());
}

/* Internal helper function to generate the SHA1 digest of input 'header' and
 * write to 'digest' and check if it has the required 5 leading zeros */
bool valid_header_digest(const char *header, char *digest) {
    // clear contents of digest
    strcpy(digest, "");

    // generate hash for header
    text_to_SHA1_digest(header, digest);

    // check if the SHA1 digest contains 5 leading zeros
    return leading_zeros(digest) == 5;
}

/* Prepares a header for a given email message with contents in 'filename' to be
 * sent to the email address 'recipient'. The output is written to 'header' in
 * form: <recipient email address>:<SHA1 digest of message body>:<counter>
 * The function returns false if the file cannot be read, or if a valid counter
 * is not found after 10 million attempts */
bool make_header(const char *recipient, const char *filename, char *header) {
    // strings to hold message and header digests
    char digest[DIGEST_LENGTH], header_digest[DIGEST_LENGTH];

    // generate the SHA1 digest of the message in 'filename'
    if (!file_to_SHA1_digest(filename, digest)) {
        return false;
    }

    // set initial counter and maximum limit
    int max_counter = 10000000, counter = 0;
    while (counter <= max_counter) {
        // construct header
        build_header(recipient, digest, counter, header);

        // get the header whose SHA1 digest contains 5 leading zeros
        if (valid_header_digest(header, header_digest)) {
            return true;
        }
        counter++;
    }
    return false;
}

/* Validates the input 'header' based on the recipient 'email_address' and
 * message stored in 'filename'. The return value is a status code of type
 * MessageStatus */
MessageStatus check_header(const char *email_address, const char *header,
                           const char *filename) {
    // check if 'email_address' matches the recipient in the header
    if (!strstr(header, email_address)) {
        return WRONG_RECIPIENT;
    }

    /* check if the computed SHA1 digest for message in 'filename' matches the
     * one in the header */
    char digest[DIGEST_LENGTH];
    if (!file_to_SHA1_digest(filename, digest) || !strstr(header, digest)) {
        return INVALID_MESSAGE_DIGEST;
    }

    // check if hashing the header produces 5 leading zeros
    char header_digest[DIGEST_LENGTH];
    if (!valid_header_digest(header, header_digest)) {
        return INVALID_HEADER_DIGEST;
    }

    // reconstruct header and check if it is equal and hence the correct format
    char header_remake[HEADER_LENGTH];
    if (!make_header(email_address, filename, header_remake) ||
        strcmp(header, header_remake)) {
        return INVALID_HEADER;
    }

    return VALID_EMAIL;
}