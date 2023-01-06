#include "spell.h"
#include <array>
#include <fstream>
#include <iostream>
#include <map>

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

int d(const string &a, const string &b, const int &i, const int &j) {
    if (min(i, j) == 0) {
        return max(i, j);
    }

    // indicator value
    int ind = indicator(a[i - 1], b[j - 1]);

    // array to store distinct edit outcomes
    array<int, 4> edits = {
        d(a, b, i - 1, j) + 1,         // char deletion
        d(a, b, i, j - 1) + 1,         // char insertion
        d(a, b, i - 1, j - 1) + ind,   // char replacement or perfect match
        MAX_LENGTH                     // large placeholder value
    };

    // transposition of two successive chars
    if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
        edits[3] = d(a, b, i - 2, j - 2) + 1;
    }

    return *min_element(edits.begin(), edits.end());
}

int edit_distance(const string a, string b) {
    return d(a, b, a.length(), b.length());
}

bool spell_correct(const std::string word, char* fixed) {
    // word already in dictionary - no need for spell checker
    if (frequency(word)) {
        strcpy(fixed, word.c_str());
        return false;
    }
    return true;
}