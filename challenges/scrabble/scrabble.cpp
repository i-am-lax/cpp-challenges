#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"
#include <string>

using namespace std;

int tile_score(const char tile) {
    if (TILE_SCORES.count(tile)) {
        return TILE_SCORES.at(tile);
    }
    return -1;
}

bool letter_in_tiles(const char ch, const string tiles) {
    for (int i = 0; i < tiles.length(); i++) {
        if (tiles[i] == ch) {
            return true;
        }
    }
    return false;
}

bool can_form_word_from_tiles(const char *word, string tiles, char* played_tiles) {
    if (*word == '\0') {
        *played_tiles = '\0';
        return true;
    }
    if (letter_in_tiles(*word, tiles)) {
        int position = tiles.find_first_of(*word);
        *played_tiles = *word;
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word+1, tiles, played_tiles);
    }
    else if (letter_in_tiles(' ', tiles)) {
        int position = tiles.find_first_of(' ');
        *played_tiles = ' ';
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word+1, tiles, played_tiles);
    }
    else if (letter_in_tiles('?', tiles)) {
        int position = tiles.find_first_of('?');
        *played_tiles = '?';
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word+1, tiles, played_tiles);
    }
    else {
        return false;
    }
}