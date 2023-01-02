#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

int tile_score(const char tile) {
    if (TILE_SCORES.count(tile)) {
        return TILE_SCORES.at(tile);
    }
    return -1;
}