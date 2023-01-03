#ifndef DOUBLETS_H
#define DOUBLETS_H

// Constraint on maximum string length
const int MAX_LENGTH = 30;

/* Pre-supplied function looks up a given word in the dictionary of approved
 * words. It is assumed that the input 'word' is in uppercase. The function
 * returns true if the 'word' is in the dictionary. Otherwise returns false */
bool dictionary_search(const char *word);

/* Returns true if the step 'current_word' to 'next_word' represents a valid
 * step in a Doublet chain. It is assumed that both inputs are in uppercase */
bool valid_step(const char *current_word, const char *next_word);

/* Writes input 'chain' to an output stream such that the first and last words
 * (doublet) are uppercase and the remaining words (links) are lowercase. The
 * input parameter 'output_stream' can be any valid output stream, including
 * cout. The function returns true if the entire chain is successfully written
 * to the output stream, and false otherwise */
bool display_chain(const char *chain[], std::ostream &output_stream);

/* Returns true if and only if the given 'chain' is a valid Doublets chain
 * according to all four rules:
 * - there are at least 2 words which constitute the doublet
 * - each word must be formed from the preceding word by changing one letter in
 * it only
 * - chain must not contain the same word twice
 * - each word must be a word in the dictionary */
bool valid_chain(const char *chain[]);

/* Attempts to find a valid chain beginning with 'startword' and ending with
 * 'target_word' in up to max_steps steps. If a valid chain can be found, output
 * parameter 'answer_chain' contains the found chain and the function returns
 * true. Otherwise the function returns false */
bool find_chain(const char *start_word, const char *target_word,
                const char *answer_chain[], int max_steps);

#endif