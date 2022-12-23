# Substring Position

## Objective
The aim is to write a two argument, integer-valued recursive function `substring_position(...)`
which searches to see if its first (string) argument is a substring of its second (string) argument.
If it finds an instance of the substring, then it returns the start position of the first instance
of the substring in the string, otherwise it returns -1.

## Examples
```
substring position("this", "this is a simple exercise") // 0
substring position("is", "this is a simple exercise") // 2
substring position("is a", "this is a simple exercise") // 5
substring position("is an", "this is a simple exercise") // -1
substring position("exercise", "this is a simple exercise") // 17
substring position("simple exercise", "this is a simple") // -1
substring position("", "this is a simple exercise") // 0
substring position("", "") // 0
```
