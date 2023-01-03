bool find_chain(const char *start_word, const char *target_word,
                const char *answer_chain[], int max_steps) {

    if (answer_chain[0] == NULL) {
    }   // This is to zero out incoming answer_chain
    else if (!valid_step(start_word, answer_chain[0])) {
        delete[] answer_chain;
        for (unsigned int i = 0;
             i < sizeof(answer_chain) / sizeof(answer_chain[0]); ++i) {
            answer_chain[i] = NULL;
        }
    }

    int filled_words =
        find_sz_of_char_arrays(answer_chain);   // Looks for null-terminator

    string previous_word, try_new_word;
    bool is_solved = false;

    if (valid_chain(answer_chain) &&
        valid_step(answer_chain[filled_words - 1], target_word)) {
        is_solved = true;
    }

    if (is_solved) {
        return true;
    }
    if (max_steps == 0 && !is_solved) {
        return false;
    }

    if (filled_words > 0) {
        previous_word = answer_chain[filled_words - 1];
    } else {
        previous_word = start_word;
    }

    for (unsigned int j = 0; j < strlen(start_word); ++j) {
        try_new_word = previous_word;
        for (unsigned int i = 0; i < 26; ++i) {
            char new_char = i + 'A';
            if (try_new_word.at(j) !=
                new_char) {   // has to be different from character already in
                              // place
                try_new_word.at(j) = new_char;

                if (valid_step(previous_word.c_str(), try_new_word.c_str()) &&
                    !is_word_already_in_chain(try_new_word.c_str(),
                                              answer_chain)) {

                    const char **answer_chain_temp = new const char
                        *[15];   // append 'try' word to answer array

                    for (int k = 0; k < filled_words; ++k) {
                        answer_chain_temp[k] = answer_chain[k];
                    }
                    answer_chain_temp[filled_words] = try_new_word.c_str();
                    answer_chain_temp[filled_words + 1] = NULL;

                    if (find_chain(start_word, target_word, answer_chain_temp,
                                   max_steps - 1)) {
                        delete[] answer_chain;
                        answer_chain = new const char *[15];

                        for (int kk = 0; kk < 15; ++kk) {
                            if (answer_chain_temp[kk] != NULL) {
                                answer_chain[kk] = answer_chain_temp[kk];
                            }
                        }

                        delete[] answer_chain_temp;

                        return true;
                    }   // if successful - append the word
                }       // if valid step
            }           // if letter is differerent
        }               // for i
    }                   // for j

    return false;
}