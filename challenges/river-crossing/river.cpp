#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#include "river.h"

/***** Implementation for Position *****/

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

/***** Implementation for EntityPosition *****/

/* Get coordinates (row, column) for a Missionary / Cannibal at 'index' on
 * either the left or right bank based on Direction 'd'. The default position is
 * used as a starting point and then offset */
Position EntityPosition::get_bank_position(Entity e, Direction d, int index) {
    if (index < 0 || index > 2) {
        throw invalid_argument(
            "[Invalid Argument Error]: index for get_bank_position() is out of "
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
        cerr << "Invalid entity." << endl;
        return pos;
    }
    // construct position (1, 2, 3) on bank using offset
    pos.col += (index * offset);
    return pos;
}

/* Get coordinates (row, column) at 'index' (first or second position)
 * on the boat at either the left or right bank based on Direction 'd'. The
 * default position for the boat on the left bank is used as a starting point
 * after which the row is adjusted to match that of the cannibal, and column is
 * offset */
Position EntityPosition::get_boat_position(Direction d, int index) {
    if (index < 0 || index > 1) {
        throw invalid_argument(
            "[Invalid Argument Error]: index for get_boat_position() is out of "
            "range - must be 0 or 1.");
    }
    Position pos = boat[d];
    pos.row = cannibal[d].row;
    pos.col += 3 + (index * offset);
    return pos;
}

// Internal helper function which allocates a dynamic 2-D array of rows x
// columns
char **allocate_2D_array(int rows, int columns) {
    char **m = new (nothrow) char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new (nothrow) char[columns];
        assert(m[r]);
    }
    return m;
}

// Internal helper function which deallocates a dynamic 2-D array 'm'
void deallocate_2D_array(char **m, int rows) {
    for (int r = 0; r < rows; r++)
        delete[] m[r];
    delete[] m;
}

// Pre-supplied function which creates an empty ASCII-art scene
char **create_scene() {
    char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

    for (int i = 0; i < SCENE_HEIGHT; i++)
        for (int j = 0; j < SCENE_WIDTH; j++)
            scene[i][j] = ' ';

    return scene;
}

// Pre-supplied function which frees up memory allocated for an ASCII-art scene
void destroy_scene(char **scene) { deallocate_2D_array(scene, SCENE_HEIGHT); }

// Pre-supplied function which displays an ASCII-art scene
void print_scene(char **scene) {
    for (int i = 0; i < SCENE_HEIGHT; i++) {
        for (int j = 0; j < SCENE_WIDTH; j++)
            cout << scene[i][j];
        cout << endl;
    }
}

// Helper function which removes carriage returns and newlines from strings
void filter(char *line) {
    while (*line) {
        if (*line >= ' ')
            line++;
        else
            *line = '\0';
    }
}

/* Pre-supplied function which inserts an ASCII-art drawing stored in a file
 * into a given ASCII-art scene starting at coordinates (row, col)  */
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

// Pre-supplied helper function to report status codes encountered
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

/* Internal helper function to count the occurences of 'entity' in the input
 * string 'str' */
int count_entity(const char *str, const char entity) {
    int count = 0;
    while (*str != '\0') {
        if (*str == entity) {
            count++;
        }
        str++;
    }
    return count;
}

/* Internal helper function to populate the left or right bank (based on
 * direction 'd') with the count of entities 'e' (M or C) in input 'str' */
int populate_bank(char **scene, const char *str, const char e, Direction d) {
    EntityPosition rs;
    Position pos;
    Entity entity = char_to_entity.at(e);

    // count of entity
    int count = count_entity(str, e);

    // for the right bank we simply subtract the entity count from the maximum
    if (d == R) {
        count = MAX_CHARACTERS - count;
    }

    // add entity to the bank
    for (int c = 0; c < count; c++) {
        pos = rs.get_bank_position(entity, d, c);
        add_to_scene(scene, pos.row, pos.col, filename.at(entity));
    }
    return count;
}

/* Internal helper function to add the boat and its passengers (based on
 * direction 'd' and input 'boat') */
void populate_boat(char **scene, const char *boat, Direction d) {
    EntityPosition rs;
    Position pos;
    int passengers = 0;

    // add boat to left or right bank depending on 'd'
    add_to_scene(scene, rs.boat[d].row, rs.boat[d].col, filename.at(BOAT));

    // add passengers
    while (*boat != '\0') {
        pos = rs.get_boat_position(d, passengers);
        add_to_scene(scene, pos.row, pos.col,
                     filename.at(char_to_entity.at(*boat)));
        passengers++;
        boat++;
    }
}

/* Returns 2-D array containing the ASCII-art scene based on 'left' (up to 7
 * characters of M/C/B) which describes the contents of the left river bank, and
 * 'boat' (up to 2 characters with M and C only) which describes the contents of
 * the boat. */
char **make_river_scene(const char *left, const char *boat) {
    // ensure the inputs are of the correct length (length <= 7 and boat <= 2)
    if (strlen(left) > 7 || strlen(boat) > 2) {
        throw invalid_argument(
            "[Invalid Argument Error]: 'left' and 'boat' should be a "
            "maximum of 7 and 2 characters respectively");
    }

    // get entity positions
    EntityPosition rs;

    // create 2-D array to represent scene
    char **scene = create_scene();

    // initial state of scene with riverbanks, sun and river
    add_to_scene(scene, rs.bank[L].row, rs.bank[L].col, filename.at(BANK));
    add_to_scene(scene, rs.bank[R].row, rs.bank[R].col, filename.at(BANK));
    add_to_scene(scene, rs.sun.row, rs.sun.col, filename.at(SUN));
    add_to_scene(scene, rs.river.row, rs.river.col, filename.at(RIVER));

    // current state of boat
    bool boat_on_left = count_entity(left, 'B');
    Direction boat_direction = static_cast<Direction>(!boat_on_left);

    // populate left river bank
    int missionaries = populate_bank(scene, left, 'M', L);
    int cannibals = populate_bank(scene, left, 'C', L);

    // add boat and passengers
    populate_boat(scene, boat, boat_direction);

    // populate right river bank
    char total[10];
    strcpy(total, left);
    strcat(total, boat);
    populate_bank(scene, total, 'M', R);
    populate_bank(scene, total, 'C', R);

    return scene;
}

/* Internal helper function whereby the string for the left bank (input 'left')
 * is updated based on counts of missionaries and cannibals in 'str'. If
 * 'str' is empty then we do nothing. If the parameter 'add' is set to true
 * then we simply concatenate 'str' onto 'left. Otherwise, we perform the
 * subtraction. */
void update_left(char *left, const char *str, bool add = false) {
    // nothing to do if targets is empty
    if (strlen(str) == 0) {
        return;
    }

    // if add = true then simply concatenate
    if (add) {
        strcat(left, str);
        return;
    }

    // declare string to store output
    char output[strlen(left) + 1];
    char *optr = output;

    // get counts of entities in 'str'
    int missionaries = count_entity(str, 'M');
    int cannibals = count_entity(str, 'C');
    bool boat = count_entity(str, 'B');

    // copy chars from 'left' that are not in 'targets'
    char *lptr = left;
    while (*lptr != '\0') {
        if (*lptr == 'M' && missionaries > 0) {
            missionaries--;
        } else if (*lptr == 'C' && cannibals > 0) {
            cannibals--;
        } else if (*lptr == 'B' && boat) {
            boat = false;
        } else {
            *optr = *lptr;
            optr++;
        }
        lptr++;
    }
    *optr = '\0';

    // update left string
    strcpy(left, output);
}

/* Internal helper function to check if target entity exceeds entity count
 * on the corresponding bank */
bool valid_entity_count(const char *left, const char *targets,
                        const char entity) {
    // counts of entity on left bank and potential crossing
    bool boat_on_left = count_entity(left, 'B');
    int count = count_entity(left, entity);
    int target_count = count_entity(targets, entity);

    // target entities cannot be greater than entities on...
    // left bank
    if (boat_on_left && target_count > count) {
        return false;
    }
    // right bank
    if (!boat_on_left && target_count > MAX_CHARACTERS - count) {
        return false;
    }
    return true;
}

/* Internal helper function to check that 'targets is a valid input based on:
 * - 'targets' is the correct length (and an empty crossing is not possible)
 * - 'targets' contains the correct entities (M or C) */
bool valid_target_string(const char *targets) {
    // ensure correct length and also cannot make empty crossing
    if (strlen(targets) == 0 || strlen(targets) > 2) {
        return false;
    }

    // ensure contains correct entity set
    int idx = 0;
    while (targets[idx] != '\0') {
        if (targets[idx] != 'M' && targets[idx] != 'C') {
            return false;
        }
        idx++;
    }

    return true;
}

/* Internal helper function to compute from input 'left' describing the left
 * bank as to whether missionaries are outnumbered and hence eaten */
bool missionaries_eaten(const char *left) {
    // count missionaries and cannibals on the left bank
    int lm = count_entity(left, 'M');
    int lc = count_entity(left, 'C');

    // deduce counts on the right bank
    int rm = MAX_CHARACTERS - lm, rc = MAX_CHARACTERS - lc;

    // missionaries eaten if they're outnumbered on either bank
    if ((lm > 0 && lc > lm) || (rm > 0 && rc > rm)) {
        return true;
    }
    return false;
}

/* Performs a crossing of the river from one bank of the river to the other
 * using the boat. The parameter 'left' is an input and an output parameter: it
 * initially describes the contents of the left river bank, and is modified to
 * reflect the result of the move. The parameter 'targets' is a string with one
 * or two characters (M or C). The scene is displayed for each stage of the
 * crossing. The return value is a status code. */
int perform_crossing(char *left, const char *targets) {
    // error checking on 'targets' input
    if (!valid_target_string(targets)) {
        return ERROR_INVALID_MOVE;
    }
    if (!valid_entity_count(left, targets, 'M')) {
        return ERROR_INVALID_MISSIONARY_COUNT;
    }
    if (!valid_entity_count(left, targets, 'C')) {
        return ERROR_INVALID_CANNIBAL_COUNT;
    }

    // check if boat is on the left or right bank
    bool boat_on_left = count_entity(left, 'B');

    // Phase 1: Loading the boat
    if (boat_on_left) {
        update_left(left, targets);
    }
    char **scene = make_river_scene(left, targets);
    print_scene(scene);

    // Phase 2: Crossing the rivers
    update_left(left, "B", !boat_on_left);
    scene = make_river_scene(left, targets);
    print_scene(scene);

    // Phase 3: Unloading the boat
    if (!boat_on_left) {
        update_left(left, targets, true);
    }
    scene = make_river_scene(left, "");
    print_scene(scene);

    // free memory for scene
    destroy_scene(scene);

    // missionaries eaten if they're outnumbered on either bank
    if (missionaries_eaten(left)) {
        return ERROR_MISSIONARIES_EATEN;
    }

    // valid non-goal state
    if (strlen(left) > 0) {
        return VALID_NONGOAL_STATE;
    }
    return VALID_GOAL_STATE;
}

/* Allows a user to play the game of missionaries and cannibals by suggesting
 * boat crossings via the keyboard. The return value is an appropriate status
 * code. The default starting position is "BCCCMMM". The user is prompted for
 * game continuation after each crossing provided that the game didn't end in a
 * win or an error.
 * Scenarios:
 * - MM (game ends with ERROR_MISSIONARIES_EATEN)
 * - CC -> M (game ends with ERROR_INVALID_MISSIONARY_COUNT)
 * - MC -> CC (game ends with ERROR_INVALID_CANNIBAL_COUNT)
 * - MC -> M -> CC -> C -> MM -> MC -> MM -> C -> CC -> C -> CC (game ends with
 * VALID_GOAL_STATE) */
int play_game() {
    cout << "Game begins!" << endl;
    // initial game state
    char play, left[8] = "BCCCMMM", targets[3];
    int result;

    // print starting scene
    char **scene = make_river_scene(left, "");
    print_scene(scene);
    destroy_scene(scene);

    do {
        cout << "Suggest a crossing." << endl;
        cin >> targets;
        int result = perform_crossing(left, targets);

        // if crossing resulted in a win or error then game is terminated
        if (result != VALID_NONGOAL_STATE) {
            if (result == VALID_GOAL_STATE) {
                cout << "Congratulations! You win!" << endl;
            }
            return result;
        }

        // prompt user to complete playing provided the game hasn't ended
        cout << "Press 'y' if you'd like to continue playing otherwise press "
                "any key."
             << endl;
        cin >> play;
    } while (tolower(play) == 'y');

    return result;
}