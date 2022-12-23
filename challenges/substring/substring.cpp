#include <cstring>
#include <iostream>

using namespace std;

// Iterative version to check if s1 is a prefix of s2
// bool is_prefix(const char *s1, const char *s2) {
//     while ((*s1 != '\0') && (*s2 != '\0')) {
//         if (*s1 != *s2) {
//             return false;
//         }
//         s1++;
//         s2++;
//     }
//     return true;
// }

// Recursive version to check if s1 is a prefix of s2
bool is_prefix(const char *s1, const char *s2) {
    if (*s1 == '\0') {
        return true;
    }
    if (*s1 == *s2) {
        return is_prefix(++s1, ++s2);
    }
    return false;
}

/* Recursive function to search for s1 as a substring in s2 and return position
 * of first instance otherwise -1 if not found */
int substring_position(const char *s1, const char *s2) {
    static int position = 0;
    int output;

    if (is_prefix(s1, s2)) {
        // store position in variable output and clear position
        output = position;
        position = 0;
        return output;
    }
    if (*s2 == '\0') {
        position = 0;
        return -1;
    } else {
        position++;
        return substring_position(s1, ++s2);
    }
}

// Alternative version to substring_position() which uses strstr()
int substring_position2(const char *s1, const char *s2) {
    const char *p = strstr(s2, s1);
    return (p) ? p - s2 : -1;
}