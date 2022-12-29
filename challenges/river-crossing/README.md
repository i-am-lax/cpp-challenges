# River-Crossing Puzzle

## Description

The challenge is to code a classical river-crossing puzzle dating from the end of the 19th century. The puzzle concerns 3 missionaries and 3 cannibals who must cross a river from the left bank to the right bank using a boat which carries at most 2 people. The boat cannot cross the river by itself with noone on board. Swimming is impossible since the river contains piranhas and sharks.
Importantly, for both banks, if there are missionaries present on a bank, then there must not be more cannibals than missionaries on that bank (since then the cannibals eat the missionaries).

```
+-----------------+                                  +-----------------+
|  Missionaries   |                                  |  Missionaries   |
|  _              |                                  |                 |
| (_)             |              \ | /               |                 |
|/ + \            |            '- .-. -'             |                 |
|\\ //            |           -==(   )==-            |                 |
|/   \            |            .- '-' -.             |                 |
|.___.            |              / | \               |                 |
|  1     2     3  |                                  |  1     2     3  |
+-----------------+                                  +-----------------+
|    Cannibals    |                                  |    Cannibals    |
|                 |                      _     _     |                 |
| \V/             |                     (_)   (_)    | \V/   \V/       |
| (_)O            |                    / + \ / + \   | (_)O  (_)O      |
|/| |+            |                    \\ // \\ //   |/| |+ /| |+      |
| /r\|            |                    /   \ /   \   | /r\|  /r\|      |
| " "'            |                    .___. .___.   | " "'  " "'      |
|  1     2     3  |                 /"'~~~~~.~~~~~'"\|  1     2     3  |
+-----------------+                 \_______________/+-----------------+
|                 |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                 |
|      Bank       |         ...\,    <>< __          |      Bank       |
|                 |      >='   (O>      /o \/        |                 |
|                 |         ''/''       >__/\   ><>  |                 |
+-----------------+                                  +-----------------+
```

## Objective

### 1
Write a function `make_river_scene(left, boat)` which takes two character strings: `left` describing the contents of the left river bank, and `boat` describing the contents of the boat, and which returns a corresponding ASCII-art scene.
The character string for the left bank may be up to 7 letters long and may contain three different kinds of characters:
* M denoting the presence of a missionary
* C denoting the presence of a cannibal
* B denoting that the boat is at the left bank
The character string for the boat may be up to 2 letters long and may contain only the characters M and C. Note both strings may also be empty and the order of the characters is irrelevant.
For example, the code:
```
scene = make_river_scene("CM", "MM");
print_scene(scene);
```
should display output similar to that shown in the description. The initial state of the puzzle (with all missionaries and cannibals and the boat at the left bank) has left bank set to `"BCCCMMM"` and boat set to `""` while the goal state of the puzzle has left bank set to `""` and boat set to `""`.

### 2
Write a function `perform_crossing(left, targets)` which performs a crossing of the river by one or two target entities from one bank of the river to the other using the boat. The parameter `left` is both an input and an output parameter: it initially describes the contents of the left river bank, but is modified to reflect the result of the move (including an update on the boat’s position).
The parameter `targets` is a string with one or two characters drawn from the letters M and C according to the entities performing the crossing (N.B. performing a "crossing" without anyone on the boat is not allowed).

The function assumes an initially empty boat, and performs the move in three phases, each of which should be displayed on the console as an ASCII-art scene:
* Loading the boat: The targets are transferred from the bank where the boat is, onto the boat.
* Crossing the river: The boat crosses the river to the other side.
* Unloading the boat: The targets are transferred from the boat onto the bank where the boat is now.

The return value should be a status code reflecting the outcome of the crossing operation e.g. `ERROR_INVALID_MOVE` in the case of an invalid targets string, `ERROR_MISSIONARIES_EATEN` in the case of the unfortunate missionaries being eaten by cannibals, or `VALID_NON_GOAL_STATE` in the case of a valid but non-goal state.
For example, the code:
```
char left[10] = "MMMB";
int result = perform_crossing(left, "MM");
cout << status_description(result) << endl;
```
should result in the output shown below, with left set to "M" and result set to `ERROR_MISSIONARIES_EATEN` (since the two missionaries arriving from the left bank will be outnumbered by the cannibals on the right bank).

```
+-----------------+                                  +-----------------+
|  Missionaries   |                                  |  Missionaries   |
|  _              |                                  |                 |
| (_)             |              \ | /               |                 |
|/ + \            |            '- .-. -'             |                 |
|\\ //            |           -==(   )==-            |                 |
|/   \            |            .- '-' -.             |                 |
|.___.            |              / | \               |                 |
|  1     2     3  |                                  |  1     2     3  |
+-----------------+                                  +-----------------+
|    Cannibals    |                                  |    Cannibals    |
|                 |     _     _                      |                 |
|                 |    (_)   (_)                     | \V/   \V/   \V/ |
|                 |   / + \ / + \                    | (_)O  (_)O  (_)O|
|                 |   \\ // \\ //                    |/| |+ /| |+ /| |+|
|                 |   /   \ /   \                    | /r\|  /r\|  /r\||
|                 |   .___. .___.                    | " "'  " "'  " "'|
|  1     2     3  |/"'~~~~~.~~~~~'"\                 |  1     2     3  |
+-----------------+\_______________/                 +-----------------+
|                 |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                 |
|      Bank       |         ...\,    <>< __          |      Bank       |
|                 |      >='   (O>      /o \/        |                 |
|                 |         ''/''       >__/\   ><>  |                 |
+-----------------+                                  +-----------------+
+-----------------+                                  +-----------------+
|  Missionaries   |                                  |  Missionaries   |
|  _              |                                  |                 |
| (_)             |              \ | /               |                 |
|/ + \            |            '- .-. -'             |                 |
|\\ //            |           -==(   )==-            |                 |
|/   \            |            .- '-' -.             |                 |
|.___.            |              / | \               |                 |
|  1     2     3  |                                  |  1     2     3  |
+-----------------+                                  +-----------------+
|    Cannibals    |                                  |    Cannibals    |
|                 |                      _     _     |                 |
|                 |                     (_)   (_)    | \V/   \V/   \V/ |
|                 |                    / + \ / + \   | (_)O  (_)O  (_)O|
|                 |                    \\ // \\ //   |/| |+ /| |+ /| |+|
|                 |                    /   \ /   \   | /r\|  /r\|  /r\||
|                 |                    .___. .___.   | " "'  " "'  " "'|
|  1     2     3  |                 /"'~~~~~.~~~~~'"\|  1     2     3  |
+-----------------+                 \_______________/+-----------------+
|                 |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                 |
|      Bank       |         ...\,    <>< __          |      Bank       |
|                 |      >='   (O>      /o \/        |                 |
|                 |         ''/''       >__/\   ><>  |                 |
+-----------------+                                  +-----------------+
+-----------------+                                  +-----------------+
|  Missionaries   |                                  |  Missionaries   |
|  _              |                                  |  _     _        |
| (_)             |              \ | /               | (_)   (_)       |
|/ + \            |            '- .-. -'             |/ + \ / + \      |
|\\ //            |           -==(   )==-            |\\ // \\ //      |
|/   \            |            .- '-' -.             |/   \ /   \      |
|.___.            |              / | \               |.___. .___.      |
|  1     2     3  |                                  |  1     2     3  |
+-----------------+                                  +-----------------+
|    Cannibals    |                                  |    Cannibals    |
|                 |                                  |                 |
|                 |                                  | \V/   \V/   \V/ |
|                 |                                  | (_)O  (_)O  (_)O|
|                 |                                  |/| |+ /| |+ /| |+|
|                 |                                  | /r\|  /r\|  /r\||
|                 |                                  | " "'  " "'  " "'|
|  1     2     3  |                 /"'~~~~~.~~~~~'"\|  1     2     3  |
+-----------------+                 \_______________/+-----------------+
|                 |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                 |
|      Bank       |         ...\,    <>< __          |      Bank       |
|                 |      >='   (O>      /o \/        |                 |
|                 |         ''/''       >__/\   ><>  |                 |
+-----------------+                                  +-----------------+
The missionaries have been eaten! Oh dear!
```

### 3
Write a function `play_game()` which allows a user to play the game of missionaries and cannibals by suggesting boat crossings via the keyboard. The return value of the function should be an appropriate status code e.g. `ERROR_MISSIONARIES_EATEN` or `VALID_GOAL_STATE` (i.e. everyone crossed successfully).