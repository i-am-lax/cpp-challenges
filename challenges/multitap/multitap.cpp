#include "multitap.h"

#include <cctype>
#include <cstring>

using namespace std;

/* Internal helper function to repeat a character ('ch') 'n' times and write to
 * string 'out' */
void repeat_char(int n, const char &ch, char *out) {
    while (n >= 0) {
        *out = ch;
        out++;
        n--;
    }
    *out = '\0';
}

/* Produces the multitap encoding of a single input character 'ch' (ignoring
case) using the KEYS map and writes it to 'multitap'. The number of keystrokes
required to encode 'ch' is returned which is the length of 'multitap' */
int encode_character(const char &ch, char *multitap) {
    // output number of keystrokes
    int size = 0;

    // ensure multitap is clear
    *multitap = '\0';

    // case when ch is a digit - add ch itself preceded with *
    if (isdigit(ch)) {
        char digit[] = {'*', ch, '\0'};
        strcpy(multitap, digit);
        size = strlen(multitap);
    }

    // otherwise identify letters
    else {
        char converted[MAX_LENGTH];
        // iterate through digits in KEYS map
        for (map<char, vector<char>>::const_iterator it = KEYS.begin();
             it != KEYS.end(); it++) {
            for (int idx = 0; idx < it->second.size(); idx++) {
                if (it->second[idx] == tolower(ch)) {
                    // repeat digit n times based on position in vector
                    repeat_char(idx, it->first, converted);
                    strcpy(multitap, converted);
                    size += idx + 1;
                    break;
                }
            }
        }
    }
    return size;
}

/* Iteratively produces the multitap encoding of the input string 'plaintext'.
 * The encoding is written to 'multitap' (taking letter case '#' and pauses '|'
 * into account) */
void encode(const char *plaintext, char *multitap) {
    // clear string
    multitap[0] = '\0';

    // string to store individual character encoding
    char encoding[MAX_LENGTH];

    //
    bool prevcase = false, currentcase = false;

    while (*plaintext != '\0') {
        if (isupper(*plaintext)) {
            currentcase = true;
        } else {
            currentcase = false;
        }

        if (currentcase != prevcase) {
            strcat(multitap, "#");
            prevcase = currentcase;
        }

        // generate character encoding
        encode_character(*plaintext, encoding);

        /* add pause if last digit of multitap and first digit of new encoding
         * are equal */
        if (multitap[strlen(multitap) - 1] == encoding[0]) {
            strcat(multitap, "|");
        }

        // append character encoding
        strcat(multitap, encoding);
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
    while (!input.eof()) {
        if (ch == '#') {
            upper = !upper;
        } else if (ch == '*') {
            input.get(ch);
            output << ch;
        } else if (isdigit(ch)) {
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