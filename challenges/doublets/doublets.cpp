#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words.
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
    static Dictionary dictionary("words.txt");
    return dictionary.search(word);
}

bool valid_step(const char* current_word, const char* next_word) {
   int steps = 0;
   if (strlen(current_word) != strlen(next_word)) {
      return false;
   }
   if (!dictionary_search(next_word)) {
      return false;
   }

   for (int idx = 0; idx < strlen(current_word); idx++) {
      if (current_word[idx] != next_word[idx]) {
         steps++;
      }
   }
   return steps == 1;
}