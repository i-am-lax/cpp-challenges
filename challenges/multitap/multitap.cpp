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

    // identify letters
    for (char key = '0'; key <= '9'; key++) {
        for (int idx = 0; idx < KEYS.at(key).size(); idx++) {
            if (KEYS.at(key)[idx] == tolower(ch)) {
                while (idx >= 0) {
                    *multitap = key;
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

void decode(istream &input, ostream &output) {
    // keep track of case
    bool upper = false;

    // string to hold digit set for decoding
    string str;
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
            str += ch;
        }
        if (str.length() > 0 && ch != input.peek()) {
            outch = KEYS.at(str[0])[str.length() - 1];
            if (upper) {
                outch = toupper(outch);
            }
            output << outch;
            // reset string
            str = "";
        }
        input.get(ch);
    }
}