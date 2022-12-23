#ifndef SUBSTRING_H
#define SUBSTRING_H

/* Returns true if its first string argument is a prefix of its second string
 * argument */
bool is_prefix(const char *s1, const char *s2);

/* Return the start position of the first instance of the substring 's1' in the
 * string 's2' else -1 */
int substring_position(const char *s1, const char *s2);

/* Second version which makes use of the strstr function in <cstring> */
int substring_position2(const char *s1, const char *s2);

#endif