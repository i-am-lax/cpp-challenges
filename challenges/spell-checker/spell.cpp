#include "spell.h"
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

/* Internal helper function */
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
            return false;
        }
    }
    return true;
}

int frequency(const std::string target) {
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

int indicator(const char &x, const char &y) { return x == y ? 0 : 1; }

int d(const string &a, const string &b, const int &i, const int &j, int total,
      int limit) {
    if (min(i, j) == 0) {
        return max(i, j);
    }

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

int edit_distance(const string a, string b, int limit) {
    return d(a, b, a.length(), b.length(), 0, limit);
}

bool spell_correct(const std::string word, char *fixed) {
    // word already in dictionary - no need for spell checker
    if (frequency(word)) {
        strcpy(fixed, word.c_str());
        return false;
    }
    // limit is edit distance of 2
    int limit = 2;

    // store word and candidate lengths for initial search narrowing
    int wl = word.length(), cl;

    // get map of words
    map<string, int> words;
    generate_map(words);

    // find suitable candidates and compute their edit distances
    map<int, vector<string>> distances;
    int d;
    for (auto const &w : words) {
        cl = w.first.length();
        if (abs(wl - cl) <= limit) {
            d = edit_distance(word, w.first, limit);
            if (d <= limit) {
                if (distances.count(d)) {
                    distances.at(d).push_back(w.first);
                } else {
                    distances[d] = {w.first};
                }
            }
        }
    }

    // store information about word with highest frequency
    string max_word;
    int max_frequency = 0;

    // words with edit distance of 1 exist - retrieve highest frequency
    if (distances.count(1) && distances.at(1).size() > 0) {
        for (auto const &w : distances.at(1)) {
            int f = words.at(w);
            if (f > max_frequency) {
                max_frequency = f;
                max_word = w;
            }
        }
    }
    // otherwise move on to words with edit distance of 2
    else if (distances.count(2) && distances.at(2).size() > 0) {
        for (auto const &w : distances.at(2)) {
            int f = words.at(w);
            if (f > max_frequency) {
                max_frequency = f;
                max_word = w;
            }
        }
    }
    // any distance beyond that, return original word
    else {
        strcpy(fixed, word.c_str());
        return false;
    }

    strcpy(fixed, max_word.c_str());
    return true;
}