#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include "river.h"

Position::Position(int _row, int _col) {
    if (_row < 0 || _row > SCENE_HEIGHT) {
        throw invalid_argument(
            "[Invalid Argument Error]: row is out of range");
    }
    if (_col < 0 || _col > SCENE_WIDTH) {
        throw invalid_argument(
            "[Invalid Argument Error]: col is out of range");
    }
    row = _row;
    col = _col;
};

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

    // create 2-D array to represent scene
    char **scene = create_scene();

    // declare variables for storing state
    int missionary_count = 3, cannibal_count = 3;

    // initial state of scene with riverbanks, sun and river
    add_to_scene(scene, rs.lbank.row, rs.lbank.col, filename.at(BANK));
    add_to_scene(scene, rs.rbank.row, rs.rbank.col, filename.at(BANK));
    add_to_scene(scene, rs.sun.row, rs.sun.col, filename.at(SUN));
    add_to_scene(scene, rs.river.row, rs.river.col, filename.at(RIVER));

    /* sequence for left river bank:
    - M denoting the presence of a missionary
    - C denoting the presence of a cannibal
    - B denoting that the boat is at the left bank */
    bool boat_on_left = false;
    while (*left != '\0') {
        if (*left == 'B') {
            add_to_scene(scene, rs.lboat.row, rs.lboat.col, filename.at(BOAT));
            boat_on_left = true;
        } else if (*left == 'M') {
            add_to_scene(scene, rs.lmissionary.row, rs.lmissionary.col,
                         filename.at(MISSIONARY));
            missionary_count -= 1;
        } else if (*left == 'C') {
            add_to_scene(scene, rs.lcannibal.row, rs.lcannibal.col,
                         filename.at(CANNIBAL));
            cannibal_count -= 1;
        }
        left++;
    }

    /* boat can contain only M or C characters */

    return scene;
}

// functions for adding / removing cannibals and missionaries and you can enter input position 0,1,2
// making sure we adjust positions accordingly
// having a boat position 0 and 1 (for where missionaries / cannibals cross)
// maybe array of positions in ElementPositions class
