#include "words.h"
#include <iostream>

using namespace std;

void test_compare(const char *str1, const char *str2) {
    cout << "The strings '" << str1 << "' and '" << str2 << "' are ";
    if (!compare(str1, str2))
        cout << "NOT ";
    cout << "the same" << endl << " (ignoring punctuation and case)" << endl;
}

void test_palindrome(const char *str) {
    cout << "The string '" << str << "' is ";
    if (!palindrome(str))
        cout << "NOT ";
    cout << "a palindrome." << endl;
}

void test_anagram(const char *str1, const char *str2) {
    cout << "The string '" << str1 << "' is ";
    if (!anagram(str1, str2))
        cout << "NOT ";
    cout << "an anagram of '" << str2 << "'" << endl;
}

int main() {

    /*** QUESTION 1 ***/
    char reversed[9];
    reverse("lairepmi", reversed);
    cout << "'lairepmi' reversed is '" << reversed << "'" << endl;
    reverse("desserts", reversed);
    cout << "'desserts' reversed is '" << reversed << "'" << endl << endl;

    /*** QUESTION 2 ***/
    test_compare("this, and THAT......", "THIS and THAT!!!");
    test_compare("this, and THAT", "THIS, but not that");
    cout << endl;

    /*** QUESTION 3 ***/

    test_palindrome("rotor");
    test_palindrome("Madam I'm adam");
    test_palindrome("Madam I'm not adam");
    test_palindrome("A man, a plan, a canal, Panama!");
    test_palindrome("a. ..");
    test_palindrome(".. ..!");
    test_palindrome("Mr. Owl ate my metal worm.");
    cout << endl;

    /*** QUESTION 4 ***/

    test_anagram("I am a weakish speller!", "William Shakespeare");
    test_anagram("I am a good speller!", "William Shakespeare");
    test_anagram("Here come dots...", "The Morse Code");
    test_anagram("stain", "satin");
    cout << endl;

    return 0;
}