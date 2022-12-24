# Palindrome

## Description

A palindrome is a word or phrase which contains the same sequence of letters when read forwards or backwards. Characters other than letters (including spaces) are ignored and the case of letters (i.e. whether
they are upper case (A-Z) or lower case (a-z)) is irrelevant. An empty string and a string containing a single letter are both palindromes (by definition).
For example the following are palindromes:
* "rotor"
* "Radar"
* "A man, a plan, a canal, Panama!"
* "Mr. Owl ate my metal worm."
* "a. .."
* ".. ..!"

An anagram is a word or phrase formed by reordering the letters of another word or phrase. Again, characters other than letters (including spaces) are ignored and the case of letters is irrelevant.
For example:
* "stain" is an anagram of "satin"
* "I am a weakish speller!" is an anagram of "William Shakespeare"
* "Here come dots..." is an anagram of "The Morse Code"

## Objective

### 1
Write a function `reverse(str1, str2)` which copies `str1` into `str2` backwards. Here `str1` is an input parameter containing the string to be reversed, and `str2` is an output parameter containing the reversed string. So the code below should result in the string reversed having the value `imperial`:
```
char reversed[9];
reverse("lairepmi", reversed);
```
You can assume that the memory for `string2` has been allocated and is sufficient to hold the output string (including sentinel character).

### 2
Write a recursive function `compare(one, two)` which compares two strings ignoring punctuation and letter case (where one and two are the two strings being compared). If the strings are the same, the function should return 1, else it should return 0. For example, `compare("This and that", "this, and THAT!")` should return 1 while `compare("these are not the SAME", "these are the SAME")` should return 0.

### 3
Write a function `palindrome(sentence)` which returns 1 if the given sentence is a palindrome, and 0 otherwise. You may not make any assumptions about the size of the input sentence.
For example, the function call `palindrome("Madam, I’m Adam")` should return 1 while the function call `palindrome("Madam, I’m not Adam")` should return 0.

### 4
Write a function `anagram(str1, str2)` which returns 1 if string `str1` is an anagram of string `str2`, and 0 otherwise. For example, the function call `anagram("William Shakespeare", "I am a weakish speller!")` should return 1, while the call `anagram("William Shakespeare", "I am a good speller!")` should return 0.