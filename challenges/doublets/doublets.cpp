#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>

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

/* Internal helper function */
void str_to_lower(const char* str, char* output) {
   // ensure string is clear
   output[0] = '\0';

   // append lowercase characters from str to output
   while (*str != '\0') {
      *output = tolower(*str);
      output++;
      str++;
   }

   // terminate string
   *output = '\0';
}

bool display_chain(const char* chain[], ostream &output_stream) {
   // keep track of position in chain
   int position = 0;

   // string to store lowercase word
   char lower[MAX_LENGTH];

   // output each word in chain
   while (chain[position]) {
      // check health of output stream
      if (output_stream.fail()) {
         return false;
      }
      // keep uppercase if first or last position
      if (position == 0 || !chain[position+1]) {
         output_stream << chain[position];
      }
      // convert to lowercase
      else {
         str_to_lower(chain[position], lower);
         output_stream << lower;
      }
      output_stream << endl;
      position++;
   }
   return true;
}

/* Internal helper function */
bool word_exists(const char* word, vector<const char*> &words) {
   for (auto const &w: words) {
      if (!strcmp(w, word)) {
         return true;
      }
   }
   return false;
}

bool valid_chain(const char* chain[]) {
   // store words we have seen
   vector<const char*> words;

   // store position in chain
   int position = 0;

   while (chain[position]) {
      // duplicate word
      if (word_exists(chain[position], words)) {
         return false;
      }
      // add word
      words.push_back(chain[position]);

      // check validity of transition from current word to the next
      if (chain[position+1] && !valid_step(chain[position], chain[position+1])) {
         return false;
      }

      position++;
   }

   // ensure we have at least 2 words in the chain
   if (position < 2) {
      return false;
   }

   return true;
}