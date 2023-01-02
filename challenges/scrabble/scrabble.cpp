#include "scrabble.h"
#include <cassert>
#include <cctype>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

/* If 'tile' is a letter (upper or lowercase) then the function returns the tile
 * score for that letter from TILE_SCORES mapping. If 'tile' is ' ' or '?'
 * (a blank tile), the function returns 0. Otherwise it returns -1 */
int tile_score(const char &tile) {
    if (TILE_SCORES.count(toupper(tile))) {
        return TILE_SCORES.at(toupper(tile));
    }
    return -1;
}

/* Internal helper function which returns true if the input character 'ch' is
 * present in the string 'tiles' and false otherwise */
bool letter_in_tiles(const char &ch, const string &tiles) {
    for (int i = 0; i < tiles.length(); i++) {
        if (tiles[i] == toupper(ch)) {
            return true;
        }
    }
    return false;
}

/* Recursive function which determines if the target 'word' can be constructed
 * from the letters in 'tiles' and if so, the function returns true and the
 * sequence is written to 'played_tiles'. Otherwise it returns false */
bool can_form_word_from_tiles(const char *word, string tiles,
                              char *played_tiles) {
    // reached end of the target word - terminate
    if (*word == '\0') {
        *played_tiles = '\0';
        return true;
    }
    // if the letter exists in the tile set then we play it and remove from
    // tiles
    if (letter_in_tiles(*word, tiles)) {
        int position = tiles.find_first_of(*word);
        *played_tiles = *word;
        played_tiles++;
        tiles.erase(position, 1);
        return can_form_word_from_tiles(word + 1, tiles, played_tiles);
        // if the letter does not exist we see if a blank tile exists instead
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
        // otherwise the word cannot be constructed and we return false
    } else {
        played_tiles[0] = '\0';
        return false;
    }
}

/* Internal helper function which applies a given ScoreModifier 'sm' and updates
 * the individual tile score 'tile_score' if modifier is DOUBLE_LETTER_SCORE
 / TRIPLE_LETTER_SCORE, or increments the count of double word ('dw') and triple
 word ('tw') modifiers if modifier is DOUBLE_WORD_SCORE / TRIPLE_WORD_SCORE */
void apply_modifier(const ScoreModifier sm, int &tile_score, int &dw, int &tw) {
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

/* Returns the word score given a 'played_tiles' string and an array of
 * 'score_modifiers'. The score for each individual tile is retrieved based on
 * the tile score itself and the corresponding score modifier. If the double /
 * triple word modifiers are present then these are applied after the tile
 * scores are tallied up. If the final word has a length of 7 then a bonus of 50
 * points is added. Note we assume that a score modifier array can comprise of
 * both double and triple word modifiers which can also appear more than once */
int compute_score(const char *played_tiles,
                  const ScoreModifier score_modifiers[]) {
    /* instantiate variables to hold overall score, and count of double/triple
     * word modifiers */
    int score = 0, dw = 0, tw = 0;

    // gather scores for individual tiles and see if double or triple word hit
    for (int idx = 0; idx < strlen(played_tiles); idx++) {
        // get score of individual tile
        int ts = tile_score(played_tiles[idx]);

        //  apply modifier
        apply_modifier(score_modifiers[idx], ts, dw, tw);

        // add tile score to overall score
        score += ts;
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

/* Returns the highest word score that can be achieved given a particular
 * collection of 'tiles' and 'score_modifiers', using any of the words in the
 * supplied dictionary. If it not possible to make any word from the 'tiles'
 * then the function returns -1. Otherwise output parameter 'word' contains the
 * word attaining the highest word score, and the function returns the score.
 * The function iterates through the dictionary and checks if the word can be
 * made from the 'tiles' and if so, computes the score and records it.
 * Subsequent words are then compared to this score and updated accordingly */
int highest_scoring_word_from_tiles(const string tiles,
                                    const ScoreModifier score_modifiers[],
                                    char *word) {
    /* variables to hold results and the word we are currently checking from
     * dictionary */
    int score, max_score = 0;
    char word_to_check[MAX_LENGTH], current_word[MAX_LENGTH];

    // create input filestream
    ifstream in;
    in.open(WORDS);

    /* for each word in WORDS file check if it is a possibility and compare
     * scores */
    while (!in.eof()) {
        // reset score
        score = 0;

        // read in word
        in >> word_to_check;

        // calculate score if word can be formed from tiles
        if (can_form_word_from_tiles(word_to_check, tiles, current_word)) {
            score = compute_score(current_word, score_modifiers);
        }

        // overwrite current maximum score if the new score exceeds it
        if (score > max_score) {
            max_score = score;
            strcpy(word, current_word);
        }
    }

    // if a word can be generated we return the highest score
    if (max_score > 0) {
        return max_score;
    }
    return -1;
}