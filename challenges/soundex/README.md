# Soundex Coding System

## Description

The Soundex coding system represents surnames according to the way they sound rather than the way they are spelt. Surnames that sound the same but are spelt differently (e.g. PAYNE and PAINE) have the same Soundex code.
All Soundex codes take the form of an upper case letter followed by three digits (e.g. `B652`, `E255` or `M263`). The rules for mapping a surname onto its corresponding Soundex code are as follows:
* The letter at the start of the Soundex code is always the first letter of the surname
* The digits of the Soundex code are assigned using the remaining letters of the surname according to the mapping below. All occurrences of the letters a, e, h, i, o, u, w and y are ignored. If there are not enough digits to make up a four character code, zeroes are added to the end. Additional letters are disregarded.
```
b, f, p, v → 1
l → 4
c, g, j, k, q, s, x, z → 2
m, n → 5
d, t → 3
r → 6

Example:
- Washington is coded W252 (W, 2 for the S, 5 for the N, 2 for the G, remaining letters disregarded
- Lee is coded L000 (L, 000 added)
```
* If the surname has two or more adjacent letters that have the same number in the Soundex coding, they should be treated as one letter. For example, Jackson is coded as J250 (J, 2 for the C, K ignored, S ignored, 5 for the N, 0 added). Note that this rule does not imply that it is impossible for adjacent repeated digits to occur in a Soundex coding.

## Objective

### 1
Write a function `encode(surname, soundex)` which produces the Soundex encoding corresponding to a given surname. The first parameter to the function (`surname`) is an input string containing the surname that should be encoded. The second parameter (`soundex`) is an output string into which the Soundex encoding for the string should be placed.

For example, the code should result in the string soundex having the value `J250`:
```
char soundex[5];
encode("Jackson", soundex)
```

### 2
Write a recursive function `compare(one, two)` which compares two Soundex codes (where one and two are strings representing the Soundex codes being compared). If the Soundex codes are the same, the function should return 1, else it should return 0.
For example, `compare("S250","S255")` should return `0` while `compare("W252","W252")`
should return `1`.

### 3
Write a function `count(surname, sentence)` which returns the number of words in a given sentence that have the same Soundex encoding as the given surname.
For example, the function call `count("Leeson", "Linnings, Leasonne, Lesson and Lemon.")` should return the value `2`.