#include "scrabble.h"
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
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

bool can_form_word_from_tiles(const char *word, string tiles,
                              char *played_tiles) {
    if (*word == '\0') {
        *played_tiles = '\0';
        return true;
    }
    if (letter_in_tiles(*word, tiles)) {
        int position = tiles.find_first_of(*word);
        *played_tiles = *word;
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word + 1, tiles, played_tiles);
    } else if (letter_in_tiles(' ', tiles)) {
        int position = tiles.find_first_of(' ');
        *played_tiles = ' ';
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word + 1, tiles, played_tiles);
    } else if (letter_in_tiles('?', tiles)) {
        int position = tiles.find_first_of('?');
        *played_tiles = '?';
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word + 1, tiles, played_tiles);
    } else {
        return false;
    }
}

void apply_modifier(ScoreModifier sm, int &tile_score, int &dw, int &tw) {
    switch (sm) {
    case NONE:
        break;
    case DOUBLE_LETTER_SCORE:
        tile_score *= 2;
        break;
    case TRIPLE_LETTER_SCORE:
        tile_score *= 3;
        break;
    case DOUBLE_WORD_SCORE:
        dw++;
        break;
    case TRIPLE_WORD_SCORE:
        tw++;
        break;
    default:
        cerr << "Invalid score modifier." << endl;
        break;
    }
}

int compute_score(const char *played_tiles, ScoreModifier score_modifiers[]) {
    int score = 0, dw = 0, tw = 0;

    // gather scores for individual tiles and see if double or triple word hit
    for (int idx = 0; idx < strlen(played_tiles); idx++) {
        // get score of individual tile
        int tile_score = TILE_SCORES.at(played_tiles[idx]);

        //  apply modifier
        apply_modifier(score_modifiers[idx], tile_score, dw, tw);

        // add tile score to overall score
        score += tile_score;
    }

    // apply double or triple word
    if (dw) {
        score *= (2 * dw);
    }
    if (tw) {
        score *= (3 * tw);
    }

    // add bonus points if all 7 tiles used
    if (strlen(played_tiles) == 7) {
        score += 50;
    }

    return score;
}