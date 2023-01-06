#include <iostream>
#include <fstream>
#include <map>
#include "spell.h"

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
    while(!in.eof()) {
        if (in.good()) {
            in >> frequency >> word;
            m[word] = frequency;
        }
        else {
            return false;
        }
    }
    return true;
}

int frequency(string target) {
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