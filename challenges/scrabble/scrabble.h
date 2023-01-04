#ifndef SCRABBLE_H
#define SCRABBLE_H

#include <map>

// Maximum length of a word
const int MAX_LENGTH = 30;

// File name containing dictionary of words
const char WORDS[] = "words.txt";

// Set of score modifiers
enum ScoreModifier {
    NONE,
    DOUBLE_LETTER_SCORE,
    TRIPLE_LETTER_SCORE,
    DOUBLE_WORD_SCORE,
    TRIPLE_WORD_SCORE
};

/* Mapping containing individual tile scores where ' ' and '?' both represent a
 * blank tile */
const std::map<char, int> TILE_SCORES = {
    {' ', 0}, {'?', 0}, {'A', 1}, {'B', 3}, {'C', 3},  {'D', 2}, {'E', 1},
    {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8},  {'K', 5}, {'L', 1},
    {'M', 3}, {'N', 1}, {'O', 1}, {'P', 3}, {'Q', 10}, {'R', 1}, {'S', 1},
    {'T', 1}, {'U', 1}, {'V', 4}, {'W', 4}, {'X', 8},  {'Y', 4}, {'Z', 10},
};

/* Returns the tile score for a given input 'tile'. If 'tile' is a letter
 * (whether uppercase or lowercase) then the function returns the tile score for
 * that letter. If 'tile' is ' ' or '?' (both of which can be used to represent
 * the blank tile), the function returns 0. Otherwise it returns -1 */
int tile_score(const char &tile);

/* Returns true if the target 'word' can be constructed from the letters in
 * 'tiles' and the sequence written to 'played_tiles'. Otherwise the function
 * returns false */
bool can_form_word_from_tiles(const char *word, std::string tiles,
                              char *played_tiles);

/* Returns the word score given a 'played_tiles' string and an array of
 * 'score_modifiers' */
int compute_score(const char *played_tiles,
                  const ScoreModifier score_modifiers[]);

/* Returns the highest word score that can be achieved given a particular
 * collection of tiles and score modifiers, using any of the words in the
 * supplied dictionary. 'tiles' and 'score_modifiers' are inputs of type string
 * and ScoreModifier[] respectively. If it not possible to make any word in the
 * supplied dictionary from the 'tiles' then the function returns -1. Otherwise
 * output parameter 'word' contains the word attaining the highest word score,
 * and the function returns the score */
int highest_scoring_word_from_tiles(const std::string tiles,
                                    const ScoreModifier score_modifiers[],
                                    char *word);

#endif
