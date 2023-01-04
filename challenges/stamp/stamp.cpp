#include "stamp.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <openssl/sha.h>
#include <cctype>

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
		if (!isxdigit(*digest)) {
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

bool file_to_SHA1_digest(const char* filename, char* digest) {
	ifstream in(filename);
	if (in.fail()) {
		strcpy(digest, "error");
		return false;
	}
	int idx = 0;
	char ch, contents[MAX_LENGTH];
	in.get(ch);
	while (!in.eof()) {
		contents[idx] = ch;
		idx++;
		in.get(ch);
	}
	in.close();
	contents[idx] = '\0';
	text_to_SHA1_digest(contents, digest);
	return true;
}

// Internal helper function
void build_header(const char* recipient, const char* digest, int counter, char* header) {
	// clear contents of header
	strcpy(header, "");

	// append contents in order
	strcpy(header, recipient);
    strcat(header, ":");
    strcat(header, digest);
    strcat(header, ":");

	strcat(header, to_string(counter).c_str());
}

bool make_header(const char* recipient, const char* filename, char* header){
	char digest[41], header_digest[41];
	if (!file_to_SHA1_digest(filename, digest)) {
		return false;
	}
	int max_counter = 10000000, counter = 0;
	while (counter <=  max_counter) {
		// construct header
		build_header(recipient, digest, counter, header);
				
		// compute hash of header
		text_to_SHA1_digest(header, header_digest);

		// check leading zeros
		if (leading_zeros(header_digest) == 5) {
			return true;
		}
		counter++;
	}
	return false;
}

MessageStatus check_header(const char* email_address, const char* header, const char* filename) {
	// check if the input email address exists in the header
	if (!strstr(header, email_address)) {
		return WRONG_RECIPIENT;
	}
	
	// check if the computed SHA1 hash exists in the header
	char digest[41];
	if (!file_to_SHA1_digest(filename, digest)) {
		return INVALID_MESSAGE_DIGEST;
	}
	if (!strstr(header, digest)) {
		return INVALID_MESSAGE_DIGEST;
	}

	// check if hashing the header produces 5 leading zeros
	char header_digest[41];
	text_to_SHA1_digest(header, header_digest);
	if (leading_zeros(header_digest) != 5) {
		return INVALID_HEADER_DIGEST;
	}
	 	
	// final check by comparing the entire header against constructed header
	char header_to_check[128];
	if (!make_header(email_address, filename, header_to_check)) {
		return INVALID_HEADER;
	}
	if (strcmp(header, header_to_check)) {
		return INVALID_HEADER;	
	}

	return VALID_EMAIL;
}

/* add your function definitions here */
