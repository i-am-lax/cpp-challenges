#ifndef STAMP_H
#define STAMP_H

// Constraint on maximum string length to be hashed
const int MAX_LENGTH = 25000;

// Length of SHA1 digest
const int DIGEST_LENGTH = 41;

// Maximum header length
const int HEADER_LENGTH = 512;

// Set of status codes for validity of header
enum MessageStatus {
    INVALID_HEADER = -1,
    WRONG_RECIPIENT = -2,
    INVALID_MESSAGE_DIGEST = -3,
    INVALID_HEADER_DIGEST = -4,
    VALID_EMAIL = 1
};

/* Pre-supplied helper function which generates the SHA1 hash of input string
 * 'text' into output parameter 'digest' */
void text_to_SHA1_digest(const char *text, char *digest);

/* Takes as its parameter a hexadecimal string representing a hash 'digest' and
 * returns the number of leading zeros it begins with. If the string contains
 * characters that are not valid hexadecimal digits ('0' to '9' and 'a' to 'f')
 * then the function returns -1 */
int leading_zeros(const char *digest);

/* Returns in 'digest' the SHA1 digest of the contents of the file with name
 * 'filename'. If the file cannot be opened then the function returns false with
 * digest set to "error", otherwise it returns true */
bool file_to_SHA1_digest(const char *filename, char *digest);

/* Prepares a header for a given email message with contents in 'filename' to be
 * sent to the email address 'recipient'. The output is written to 'header' in
 * form: <recipient email address>:<SHA1 digest of message body>:<counter>
 * The return value is false if the file containing the message body cannot be
 * read, or if a valid counter is not found after 10 million attempts, otherwise
 * it returns true */
bool make_header(const char *recipient, const char *filename, char *header);

/* Validates the input 'header' based on the recipient 'email_address' and
 * message stored in 'filename'. The return value is a status code of type
 * MessageStatus */
MessageStatus check_header(const char *email_address, const char *header,
                           const char *filename);

#endif
