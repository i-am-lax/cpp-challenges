#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include "river.h"

// Default constructor for position
Position::Position() : row(-1), col(-1){};

Position::Position(int _row, int _col) {
    if (_row < 0 || _row > SCENE_HEIGHT) {
        throw invalid_argument("[Invalid Argument Error]: row is out of range");
    }
    if (_col < 0 || _col > SCENE_WIDTH) {
        throw invalid_argument("[Invalid Argument Error]: col is out of range");
    }
    row = _row;
    col = _col;
};

Position ElementPositions::get_bank_position(Element e, Direction d,
                                                  int index) {
    if (index < 0 || index > 2) {
        throw invalid_argument("[Invalid Argument Error]: index is out of "
                               "range - must be 0, 1 or 2.");
    }
    Position pos;
    switch (e) {
    case MISSIONARY:
        pos = missionary[d];
        break;
    case CANNIBAL:
        pos = cannibal[d];
        break;
    default:
        cerr << "Invalid element." << endl;
        return pos;
    }
    // construct position (1, 2, 3) on bank using offset
    pos.col += (index * offset);
    return pos;
}

Position ElementPositions::get_boat_position(Direction d, int index) {
    if (index < 0 || index > 1) {
        throw invalid_argument("[Invalid Argument Error]: index is out of "
                               "range - must be 0 or 1.");
    }
    Position pos = boat[d];
    pos.row = cannibal[d].row;
    pos.col += 3 + (index * offset);
    return pos;
}

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
    char **m = new (nothrow) char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new (nothrow) char[columns];
        assert(m[r]);
    }
    return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
    for (int r = 0; r < rows; r++)
        delete[] m[r];
    delete[] m;
}

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
    char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

    for (int i = 0; i < SCENE_HEIGHT; i++)
        for (int j = 0; j < SCENE_WIDTH; j++)
            scene[i][j] = ' ';

    return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene
 */
void destroy_scene(char **scene) { deallocate_2D_array(scene, SCENE_HEIGHT); }

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++)
            cout << scene[i][j];
        cout << endl;
    }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
    while (*line) {
        if (*line >= ' ')
            line++;
        else
            *line = '\0';
    }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

    ifstream in(filename);
    if (!in)
        return false;

    int start_col = col;
    char line[512];
    in.getline(line, 512);
    filter(line);
    while (!in.fail()) {
        for (int n = 0; n < strlen(line); n++) {
            if (row >= SCENE_HEIGHT)
                return false;
            if (col >= SCENE_WIDTH)
                break;
            scene[row][col++] = line[n];
        }
        row++;
        col = start_col;
        in.getline(line, 512);
        filter(line);
    }
    return true;
}

/* pre-supplied helper function to report the status codes encountered in
 * Question 3 */
const char *status_description(int code) {
    switch (code) {
    case ERROR_INVALID_CANNIBAL_COUNT:
        return "Invalid cannibal count";
    case ERROR_INVALID_MISSIONARY_COUNT:
        return "Invalid missionary count";
    case ERROR_INVALID_MOVE:
        return "Invalid move";
    case ERROR_MISSIONARIES_EATEN:
        return "The missionaries have been eaten! Oh dear!";
    case ERROR_INPUT_STREAM_ERROR:
        return "Unexpected input stream error";
    case ERROR_BONUS_NO_SOLUTION:
        return "No solution";
    case VALID_GOAL_STATE:
        return "Problem solved!";
    case VALID_NONGOAL_STATE:
        return "Valid state, everything is OK!";
    }
    return "Unknown error";
}

/* insert your functions here */
char **make_river_scene(const char *left, const char *boat) {
    // ensure the inputs are of the correct length (length <= 7 and boat <= 2)
    if (strlen(left) > 7 || strlen(boat) > 2) {
        throw invalid_argument(
            "[Invalid Argument Error]: 'left' and 'boat' should be a "
            "maximum of 7 and 2 characters respectively");
    }

    // get element positions
    ElementPositions rs;
    Position pos;

    // create 2-D array to represent scene
    char **scene = create_scene();

    // initial state of scene with riverbanks, sun and river
    add_to_scene(scene, rs.bank[L].row, rs.bank[L].col, filename.at(BANK));
    add_to_scene(scene, rs.bank[R].row, rs.bank[R].col, filename.at(BANK));
    add_to_scene(scene, rs.sun.row, rs.sun.col, filename.at(SUN));
    add_to_scene(scene, rs.river.row, rs.river.col, filename.at(RIVER));

    /* Sequence for left river bank:
    - M denoting the presence of a missionary
    - C denoting the presence of a cannibal
    - B denoting that the boat is at the left bank */
    bool boat_on_right = true;
    int missionaries = 0, cannibals = 0;

    while (*left != '\0') {
        if (*left == 'B') {
            add_to_scene(scene, rs.boat[L].row, rs.boat[L].col, filename.at(BOAT));
            boat_on_right = false;
        }
        else if (*left == 'M') {
            pos = rs.get_bank_position(MISSIONARY, L, missionaries);
            add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
            missionaries += 1;
        }
        else if (*left == 'C') {
            pos = rs.get_bank_position(CANNIBAL, L, cannibals);
            add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
            cannibals += 1;
        }
        left++;
    }

    // Add boat to right bank if not already on left bank
    if (boat_on_right) {
        add_to_scene(scene, rs.boat[R].row, rs.boat[R].col, filename.at(BOAT));
    }

    /* Sequence for boat:
    - M denoting the presence of a missionary
    - C denoting the presence of a cannibal */
    int passengers = 0;
    while (*boat != '\0') {
        pos = rs.get_boat_position(static_cast<Direction>(boat_on_right), passengers);
        if (*boat == 'M') {
            add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
            missionaries += 1;
        }
        else if (*boat == 'C') {
            add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
            cannibals += 1;
        }
        passengers += 1;
        boat++;
    }

    /* Construct right river bank */
    while (missionaries != MAX_CHARACTERS) {
        pos = rs.get_bank_position(MISSIONARY, R, MAX_CHARACTERS - missionaries - 1);
        add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
        missionaries += 1;
    }
    while (cannibals != MAX_CHARACTERS) {
        pos = rs.get_bank_position(CANNIBAL, R, MAX_CHARACTERS - cannibals - 1);
        add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
        cannibals += 1;
    }

    return scene;
}

/* Points to note:
- 3 missionaries and 3 cannibals who must cross a river from the LEFT bank to
the RIGHT bank
- The boat cannot cross the river by itself with noone on board
- There must not be more cannibals than missionaries on the bank if there are
missionaries there


*/

// functions for adding / removing cannibals and missionaries and you can enter
// input position 0,1,2 making sure we adjust positions accordingly having a
// boat position 0 and 1 (for where missionaries / cannibals cross) maybe array
// of positions in ElementPositions class - [[0,0,1], [0,1,0]]... index 0 miss
// index 1 cannibals bstate = [M/C, M/C] and need to keep track of cannibals and
// missionaries on each side... need fn for removing elements from scene
// remove_missionary(missionary/cannibal, position, left/right) left / right is
// same, except for an offset

/* map out all positions
LEFT
boat = {17, 19}
boat[0] = {11, 22}     (first spot on boat)
boat[1] = {11, 28}

m1 = {2, 1}     + 6
m2 = {2, 7}     + 6
m3 = {2, 13}    + 6

c1 = {11, 1}    + 6
c2 = {11, 7}    + 6
c3 = {11, 13}   + 6

RIGHT
boat = {17, 36}
boat[0] = {11, 39}     (first spot on boat)
boat[1] = {11, 45}

m1 = {2, 54}     + 41
m2 = {2, 60}
m3 = {2, 66}

c1 = {11, 54}
c2 = {11, 60}
c3 = {11, 66}
*/