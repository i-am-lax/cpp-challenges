#include "spell.h"
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

int frequency(const string &target) {
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

bool indicator(string &a, string &b, const int &i, const int &j) {
    return a[i - 1] != b[j - 1];
}

int d(string &a, string &b, const int &i, const int &j) {
    if (min(i, j) == 0) {
        return max(i, j);
    }

    vector<int> edits;
    // character deletion
    edits.push_back(d(a, b, i - 1, j) + 1);
    // character insertion
    edits.push_back(d(a, b, i, j - 1) + 1);
    // character replacement or perfect match
    edits.push_back(d(a, b, i - 1, j - 1) + indicator(a, b, i, j));

    if (i > 1 && j > 1 && (a[i - 1] == b[j - 2]) && (a[i - 2] == b[j - 1])) {
        // transposition of two successive characters
        edits.push_back(d(a, b, i - 2, j - 2) + 1);
    }
    return *min_element(edits.begin(), edits.end());
}

int edit_distance(string a, string b) {
    return d(a, b, a.length(), b.length());
}