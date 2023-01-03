#include "multitap.h"

#include <cctype>
#include <cstring>

using namespace std;

// ignoring case
int encode_character(const char ch, char *multitap) {
    // output number of keystrokes
    int size = 0;

    // ensure multitap is clear
    multitap[0] = '\0';

    // add * before digit
    if (isdigit(ch)) {
        multitap[0] = '*';
        multitap[1] = ch;
        multitap[2] = '\0';
        size = strlen(multitap);
        return size;
    }

    map<char, vector<char>> keys = {
        {'0', {' '}},           {'1', {'.', ',', '!', '!'}},
        {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
        {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
        {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
        {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}},
    };

    // identify letters
    for (map<char, vector<char>>::iterator it = keys.begin(); it != keys.end();
         it++) {
        for (int idx = 0; idx < it->second.size(); idx++) {
            if (it->second[idx] == tolower(ch)) {
                while (idx >= 0) {
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

// iterative
void encode(const char *plaintext, char *multitap) {
    // clear string
    multitap[0] = '\0';

    // string to store individual character encoding
    char m[10];

    bool prevcase = false, currentcase = false;

    while(*plaintext != '\0') {
        if (isupper(*plaintext)) {
            currentcase = true;
        }
        else {
            currentcase = false;
        }

        if (currentcase != prevcase) {
            strcat(multitap, "#");
            prevcase = currentcase;
        }

        encode_character(*plaintext, m);

        // add pause
        if (multitap[strlen(multitap) - 1] == m[0]) {
            strcat(multitap, "|");
        }

        strcat(multitap, m);
        plaintext++;
    }
}

char decode_char(string str) {
    map<char, vector<char>> keys = {
        {'0', {' '}},           {'1', {'.', ',', '!', '!'}},
        {'2', {'a', 'b', 'c'}}, {'3', {'d', 'e', 'f'}},
        {'4', {'g', 'h', 'i'}}, {'5', {'j', 'k', 'l'}},
        {'6', {'m', 'n', 'o'}}, {'7', {'p', 'q', 'r', 's'}},
        {'8', {'t', 'u', 'v'}}, {'9', {'w', 'x', 'y', 'z'}},
    };
    return keys.at(str[0])[str.length() - 1];
}


void decode(istream &input, ostream &output) {
    bool upper = false;
    string decoded;
    char ch, outch;

    input.get(ch);
    while(!input.eof()) {
        if (ch == '#') {
            upper = !upper;
        }
        else if (ch == '*') {
            input.get(ch);
            output << ch;
        }
        else if (isdigit(ch)) {
            decoded += ch;
        }
        if (decoded.length() > 0 && ch != input.peek()) {
            outch = decode_char(decoded);
            if (upper) {
                outch = toupper(outch);
            }
            output << outch;
            decoded = "";
        }
        input.get(ch);
    }
}