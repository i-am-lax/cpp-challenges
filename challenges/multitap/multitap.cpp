#include "multitap.h"

#include <cctype>
#include <cstring>

using namespace std;

int encode_character(const char ch, char* multitap) {
    // output number of keystrokes
    int size = 0;

    // add * before digit
    if (isdigit(ch)) {
        multitap[0] = '*';
        multitap[1] = ch;
        multitap[2] = '\0';
        size = strlen(multitap);
        return size;
    }

    // uppercase letter - add #
    if (isupper(ch)) {
        *multitap = '#';
        multitap++;
        size++;
    }

    map<char, vector<char>> keys = {
        {'0', {' '}},           {'1', {'.', ',', '!', '!'}},
        {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
        {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
        {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
        {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}},
    };

    // identify letters
    for (map<char, vector<char>>::iterator it=keys.begin(); it!=keys.end(); it++) {
        for (int idx = 0; idx < it->second.size(); idx++) {
            if (it->second[idx] == tolower(ch)) {
                while(idx >= 0) {
                    *multitap = it->first;
                    multitap++;
                    size++;
                    idx--;
                }
                break;
            }
        }
    }
    *multitap = '\0';
    return size;
}