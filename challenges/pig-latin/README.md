# Pig Latin

## Description
Pig Latin is an elegant but archaic pseudo-language that was first spoken in the Middle Ages. Invented by commoners so that they could sound as fancy as their lords, it is based on manipulating the letters of English words so that they sound like Latin words in plural feminine form (i.e. they end in an "ay" sound).
It is a little known fact that the Magna Carta was first written in Pig Latin and only later translated into lesser tongues. Today, Pig Latin is used mostly for amusement although it also has more serious applications such as obfuscating song titles to evade copyright restrictions on music web sites.

Any English word may be changed into its Pig Latin equivalent as follows:
1. If the word begins with a vowel, add "way" to it. For example, Pig Latin for apple is appleway.
2. If the word begins with a letter that is not a vowel, find the first occurence of a vowel, move all the characters before the vowel to the end of the word, and add "ay". For example, grape becomes apegray and strong becomes ongstray.
3. If the word contains no vowels just add "ay" to it.
4. For the purposes of this exercise the vowels are a, e, i, o, u and y; but y is only considered a vowel if it is not the first or last letter of the word. So yeti becomes etiyay, my becomes myay and crying becomes yingcray.
5. If the word begins with a character that is a digit, leave the word as is. For example, 300 remains 300.
6. If the word begins with an initial upper case (captial) letter then so should the corresponding Pig Latin word. For example, Banana becomes Ananabay. You do not need to handle other capitalization patterns (e.g. all uppercase words).

## Objective

### 1
Write a function `findFirstVowel(word)` which returns the position of the first "vowel" in the given `word` (see the Pig Latin rules on the previous page for the precise definition of "vowel"). If the word does not contain a vowel then the function should return `-1`. The first parameter to the function (`word`) is a read-only string containing a single English word.

For example, the code below should result in the integer vowel having the value 1.
```
int vowel;
vowel = findFirstVowel("passionfruit");
```

### 2
Write a function `translateWord(english, piglatin)` which produces a Pig Latin translation for a given English word. The first parameter to the function (`english`) is an input string containing the English word. The second parameter (`piglatin`) is an output parameter which should contain the corresponding Pig Latin translation.

For example, the code should result in the string translated having the value `Ananabay`.
```
char translated[100];
translateWord("Banana", translated);
```

### 3
Write a function `translateStream(inputStream, outputStream)` which takes words from an input stream and writes a corresponding Pig Latin translation to an output stream. The first parameter (`inputStream`) is the input stream (e.g. `cin` or a file input stream) and the second parameter is the output stream. You may
assume that both the input and output streams have been initialised/opened appropriately before the function is called, and that no word in the file is longer than 64 characters.