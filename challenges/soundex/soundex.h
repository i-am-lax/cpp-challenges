#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <map>

// Mapping for digits of the Soundex code
const std::map<char, char> CODES = {
    {'b', '1'}, {'f', '1'}, {'p', '1'}, {'v', '1'}, {'c', '2'}, {'g', '2'},
    {'j', '2'}, {'k', '2'}, {'q', '2'}, {'s', '2'}, {'x', '2'}, {'z', '2'},
    {'d', '3'}, {'t', '3'}, {'l', '4'}, {'m', '5'}, {'n', '5'}, {'r', '6'},
};

/* Generates the Soundex encoding corresponding to a given 'surname' and writes
 * result to 'soundex' */
void encode(const char *surname, char *soundex);

/* Compare two Soundex codes ('one' and 'two') and return 1 if the codes are the
 * same otherwise return 0 */
int compare(const char *one, const char *two);

#endif