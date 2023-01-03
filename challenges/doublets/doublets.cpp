#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* Looks up 'word' in the dictionary of approved words and returns true if
 * it is in the dictionary. Otherwise returns false */
bool dictionary_search(const char *word) {
    static Dictionary dictionary("words.txt");
    return dictionary.search(word);
}

/* Returns true if the step 'current_word' to 'next_word' represents a valid
 * step in a Doublet chain based on rules:
 * - only one letter change is allowed
 * - 'next_word' has to be a word in the dictionary
 * It is assumed that both inputs are in uppercase */
bool valid_step(const char *current_word, const char *next_word) {
    // counter for the number of changes from 'current_word' to 'next_word'
    int steps = 0;

    // ensure both words have the same length
    if (strlen(current_word) != strlen(next_word)) {
        return false;
    }

    // ensure 'next_word' is a valid word in the dictionary
    if (!dictionary_search(next_word)) {
        return false;
    }

    // iterate through and increment steps if there is a difference in letters
    for (int idx = 0; idx < strlen(current_word); idx++) {
        if (current_word[idx] != next_word[idx]) {
            steps++;
        }
    }
    // only 1 change is allowed
    return steps == 1;
}

/* Internal helper function to convert input string 'str' to lowercase and store
 * the result in 'output' */
void str_to_lower(const char *str, char *output) {
    // ensure string is clear
    output[0] = '\0';

    // append lowercase characters from str to output
    while (*str != '\0') {
        *output = tolower(*str);
        output++;
        str++;
    }

    // terminate string
    *output = '\0';
}

/* Writes input 'chain' (NULL-terminated array of uppercase C-style strings) to
 * an output stream such that the first and last words (doublet) are uppercase
 * and the remaining words (links) are lowercase. The input parameter
 * 'output_stream' can be any valid output stream, including cout. The function
 * returns true if the entire chain is successfully written to the output
 * stream, and false otherwise */
bool display_chain(const char *chain[], ostream &output_stream) {
    // keep track of position in chain
    int position = 0;

    // string to store lowercase word
    char lower[MAX_LENGTH];

    // output each word in chain
    while (chain[position]) {
        // check health of output stream
        if (output_stream.fail()) {
            return false;
        }
        // keep as uppercase if doublet
        if (position == 0 || !chain[position + 1]) {
            output_stream << chain[position];
        }
        // convert links to lowercase
        else {
            str_to_lower(chain[position], lower);
            output_stream << lower;
        }
        output_stream << endl;
        position++;
    }
    return true;
}

/* Internal helper function which returns true if the input 'word' is present in
 * the vector 'words' otherwise returns false */
bool word_exists(const char *word, vector<const char *> &words) {
    for (auto const &w : words) {
        // check if the words are equal
        if (!strcmp(w, word)) {
            return true;
        }
    }
    return false;
}

/* Returns true if and only if the given 'chain' (NULL-terminated array of
 * uppercase C-style strings) is a valid Doublets chain according to all four
 * rules:
 * - there are at least 2 words which constitute the doublet
 * - each word must be formed from the preceding word by changing one letter in
 * it only
 * - chain must not contain the same word twice
 * - each word must be a word in the dictionary */
bool valid_chain(const char *chain[]) {
    // unique words we have seen
    vector<const char *> words;

    // store position in chain
    int position = 0;

    while (chain[position]) {
        // check if the word is already in the chain
        if (word_exists(chain[position], words)) {
            return false;
        }
        // add word to set of unique words
        words.push_back(chain[position]);

        // check validity of transition from current word to the next
        if (chain[position + 1] &&
            !valid_step(chain[position], chain[position + 1])) {
            return false;
        }
        position++;
    }

    // ensure we have at least 2 words in the chain
    return position >= 2;
}

/* Internal helper function to calculate the length of the input 'chain' which
 * is a NULL-terminated array of uppercase C-style strings */
int length_chain(const char *chain[]) {
    int position = 0;
    while (chain[position]) {
        position += 1;
    }
    return position;
}

/* Internal helper function to count how many characters are different between
 * strings 'one' and 'two' */
int char_difference(const char *one, const char *two) {
    int count = 0;
    while (*one != '\0' || *two != '\0') {
        if (*one != *two) {
            count += 1;
        }
        one++;
        two++;
    }
    return count;
}

bool word_in_chain(const char *word, const char *chain[]) {
    int position = 0;
    while (chain[position]) {
        // check if the words are equal
        if (!strcmp(chain[position], word)) {
            return true;
        }
        position++;
    }
    return false;
}

bool aux(const char *word, const char *target, const char *chain[],
         int max_steps) {
    // chain length exceeds step constraint - terminate
    if (length_chain(chain) - 1 > max_steps) {
        cout << "chain too long!" << endl;
        display_chain(chain, cout);
        return false;
    }
    // chain is valid - terminate
    if (valid_chain(chain)) {
        return true;
    }
    char w[MAX_LENGTH];
    strcpy(w, word);
    for (int idx = 0; idx < strlen(word); idx++) {
        for (char c = 'A'; c <= 'Z'; c++) {
            w[idx] = c;
            if (valid_step(word, w) && !word_in_chain(w, chain)) {
                // update
                int length = length_chain(chain);
                chain[length - 1] = w;
                chain[length] = target;
                chain[length + 1] = NULL;
                if (aux(w, target, chain, max_steps)) {
                    return true;
                }
                // backtrack
                chain[length - 1] = target;
                chain[length] = NULL;
            }
        }
    }
    return false;
}

/* Attempts to find a valid chain beginning with 'startword' and ending with
 * 'target_word' in up to max_steps steps. If a valid chain can be found, output
 * parameter 'answer_chain' contains the found chain and the function returns
 * true. Otherwise the function returns false */
bool find_chain(const char *start_word, const char *target_word,
                const char *answer_chain[], int max_steps) {
    // ensure both words are valid words in the dictionary
    if (!dictionary_search(start_word) || !dictionary_search(target_word)) {
        return false;
    }
    // ensure both words are the same length
    if (strlen(start_word) != strlen(target_word)) {
        return false;
    }
    /* ensure count of different characters is <= 'max_steps' otherwise not
     * possible */
    if (char_difference(start_word, target_word) > max_steps) {
        return false;
    }
    // initialise chain
    for (int i = 0; i <= max_steps + 1; i++) {
        answer_chain[i] = new char[MAX_LENGTH];
    }
    answer_chain[0] = start_word;
    answer_chain[1] = target_word;
    answer_chain[2] = NULL;

    // run recursive search
    if (aux(start_word, target_word, answer_chain, max_steps)) {
        return true;
    }
    return false;
}