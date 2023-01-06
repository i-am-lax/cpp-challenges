#include "spell.h"
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

/* Internal helper function to create a map (key is word and value is frequency
 * in corpus) and write to 'm' from filename specified by WORDS. If the map is
 * created successfully then the function returns true, otherwise it returns
 * false and clears all elements in 'm' */
bool generate_map(map<string, int> &m) {
    // create input file stream
    ifstream in(WORDS);
    if (in.fail()) {
        cerr << "Error opening file: " << WORDS << endl;
        return false;
    }

    // read key (word) and value (frequency) into m
    int frequency;
    string word;
    while (!in.eof()) {
        if (in.good()) {
            in >> frequency >> word;
            m[word] = frequency;
        } else {
            m.clear();
            return false;
        }
    }
    return true;
}

/* Takes as input a word 'target' and returns the frequency (number of times the
 * word appears in the corpus (WORDS)). If the word cannot be found then the
 * function returns 0 */
int frequency(const std::string target) {
    // generate mapping from corpus
    map<string, int> mapping;
    if (!generate_map(mapping)) {
        return 0;
    }

    // identify word from mapping and get frequency
    if (mapping.count(target)) {
        return mapping.at(target);
    }
    return 0;
}

/* Internal helper function to compute the indicator such that if char inputs
 * 'x' and 'y' are equal then the function returns 0 otherwise 1 */
int indicator(const char &x, const char &y) { return x == y ? 0 : 1; }

/* Implementaton of the recursive Damerau–Levenshtein distance function for
 * input strings 'a' and 'b'. Additional parameters 'total' and 'limit' added to
 * restrict the maximum edit distance and speed up the computation */
int d(const string &a, const string &b, const int &i, const int &j, int total,
      int limit) {

    // first case
    if (min(i, j) == 0) {
        return max(i, j);
    }

    // restrict calculation
    if (total >= limit) {
        return limit;
    }

    // indicator value
    int ind = indicator(a[i - 1], b[j - 1]);

    /* array to store distinct edit outcomes:
     * char deletion
     * char insertion
     * char replacement or perfect match
     * transposition of two successive chars */
    array<int, 4> edits = {
        d(a, b, i - 1, j, total + 1, limit) + 1,
        d(a, b, i, j - 1, total + 1, limit) + 1,
        d(a, b, i - 1, j - 1, total + ind, limit) + ind,
        MAX_LENGTH   // large placeholder value
    };

    // transposition condition
    if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
        edits[3] = d(a, b, i - 2, j - 2, total + 1, limit) + 1;
    }

    return *min_element(edits.begin(), edits.end());
}

/* Returns the Damerau–Levenshtein distance between input strings 'a' and 'b'
 * constrained by 'limit' such that distances beyond this are not considered */
int edit_distance(const string a, string b, int limit) {
    return d(a, b, a.length(), b.length(), 0, limit);
}

/* Suggests a spell checked variant of the input 'word' and returns true with
 * the correction in 'fixed' if one is found, otherwise 'fixed' stores the
 * original 'word' and the function returns false. Logic:
 * - if word is in the corpus, return word
 * - if there are 1 or more words in corpus at an edit distance of 1 from w,
 * return the one that occurs most frequently
 * - if there are 1 or more words in the corpus at an edit distance of 2 from w,
return the one that occurs most frequently
 * - otherwise return the original word */
bool spell_correct(const std::string word, char *fixed) {
    // copy input word into fixed as default case
    strcpy(fixed, word.c_str());

    // get map of words
    map<string, int> words;
    generate_map(words);

    // word already in dictionary - no need for spell checker
    if (words.count(word)) {
        return false;
    }

    // limit is edit distance of 2
    int limit = 2;

    // store information about word with highest frequency
    string max_word;
    int max_frequency = 0;

    // store word and candidate lengths for initial search narrowing
    int wl = word.length(), cl;

    // find suitable candidates and compute their edit distances
    int d, lowestd = limit;
    for (auto const &w : words) {
        cl = w.first.length();
        // if the difference in lengths is > limit then don't compute distance
        if (abs(wl - cl) <= limit) {
            d = edit_distance(word, w.first, limit);
            // reached new low distance - update max word and frequency
            if (d < lowestd) {
                lowestd = d;
                max_word = w.first;
                max_frequency = w.second;
            }
            /* matched lowest distance - check if word frequency higher than
             * current winner */
            else if (d == lowestd) {
                if (w.second > max_frequency) {
                    max_word = w.first;
                    max_frequency = w.second;
                }
            }
        }
    }
    // return original word if can't find word in distance limit
    if (!max_frequency) {
        return false;
    }
    // otherwise return word with lowest distance and highest frequency
    strcpy(fixed, max_word.c_str());
    return true;
}