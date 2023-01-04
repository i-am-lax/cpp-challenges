#include "stamp.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>

using namespace std;

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
    char append[16];
    strcpy(output, "");
    for (int n = 0; n < hash_length; n++) {
        sprintf(append, "%02x", str[n]);
        strcat(output, append);
    }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((const unsigned char *) text, strlen(text), hash);
    convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

int leading_zeros(const char* digest) {
	// counter for leading zeros
	int count = 0;
	bool front = true;
	while(*digest != '\0') {
		if (!isalnum(*digest) && !islower(*digest)) {
			return -1;
		}
		if (*digest == '0' && front) {
			count++;
		}
		else {
			front = false;
		}
		digest++;
	}
	return count;
}
/* add your function definitions here */
