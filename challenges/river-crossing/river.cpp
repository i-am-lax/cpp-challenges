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

// Internal helper function which allocates a dynamic 2D array of rows x columns
char **allocate_2D_array(int rows, int columns) {
    char **m = new (nothrow) char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new (nothrow) char[columns];
        assert(m[r]);
    }
    return m;
}

// Internal helper function which deallocates a dynamic 2D array 'm'
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
   into a given ASCII-art scene starting at coordinates (row, col)  */
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
            add_to_scene(scene, rs.boat[L].row, rs.boat[L].col,
                         filename.at(BOAT));
            boat_on_right = false;
        } else if (*left == 'M') {
            pos = rs.get_bank_position(MISSIONARY, L, missionaries);
            add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
            missionaries++;
        } else if (*left == 'C') {
            pos = rs.get_bank_position(CANNIBAL, L, cannibals);
            add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
            cannibals++;
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
        pos = rs.get_boat_position(static_cast<Direction>(boat_on_right),
                                   passengers);
        if (*boat == 'M') {
            add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
            missionaries++;
        } else if (*boat == 'C') {
            add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
            cannibals++;
        }
        passengers++;
        boat++;
    }

    /* Construct right river bank */
    while (missionaries != MAX_CHARACTERS) {
        pos = rs.get_bank_position(MISSIONARY, R,
                                   MAX_CHARACTERS - missionaries - 1);
        add_to_scene(scene, pos.row, pos.col, filename.at(MISSIONARY));
        missionaries++;
    }
    while (cannibals != MAX_CHARACTERS) {
        pos = rs.get_bank_position(CANNIBAL, R, MAX_CHARACTERS - cannibals - 1);
        add_to_scene(scene, pos.row, pos.col, filename.at(CANNIBAL));
        cannibals++;
    }

    return scene;
}

bool is_valid_move(const char *left, const char *targets) {
    // ensure correct length and also cannot make empty crossing
    if (strlen(targets) == 0 || strlen(targets) > 2) {
        return false;
    }

    // counts of missionaries and cannibals on left bank and potential crossing
    int missionaries = count_entity(left, 'M');
    int cannibals = count_entity(left, 'C');
    int target_missionaries = count_entity(targets, 'M');
    int target_cannibals = count_entity(targets, 'C');

    // ensure contains correct entity set
    while (*targets != '\0') {
        if (*targets != 'M' && *targets != 'C') {
            return false;
        }
        targets++;
    }

    // ensure entities in target <= entities on left bank
    if (target_missionaries > missionaries || target_cannibals > cannibals) {
        return false;
    }
    return true;
}

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

/* String is reconstructed based on counts of missionaries and cannibals in
 * targets */
void update_left(char *left, const char *targets, bool add) {
    // nothing to do if targets is empty
    if (strlen(targets) == 0) {
        return;
    }

    // if add = true then simply concatenate
    if (add) {
        strcat(left, targets);
        return;
    }

    // get counts of missionaries and cannibals
    int missionaries = count_entity(targets, 'M');
    int cannibals = count_entity(targets, 'C');
    bool boat = count_entity(targets, 'B');

    // declare string to store output
    char output[strlen(left) + 1];
    char *output_ptr = output;

    // copy chars from left that are not in targets
    char *left_ptr = left;
    while (*left_ptr != '\0') {
        if (*left_ptr == 'M' && missionaries > 0) {
            left_ptr++;
            missionaries--;
        } else if (*left_ptr == 'C' && cannibals > 0) {
            left_ptr++;
            cannibals--;
        } else if (*left_ptr == 'B' && boat) {
            left_ptr++;
            boat = false;
        } else {
            *output_ptr = *left_ptr;
            output_ptr++;
            left_ptr++;
        }
    }
    *output_ptr = '\0';

    // update left string
    strcpy(left, output);
}

int perform_crossing(char *left, const char *targets) {
    // case of an invalid targets string
    if (!is_valid_move(left, targets)) {
        return ERROR_INVALID_MOVE;
    }

    // check if boat is on the left or right
    bool boat_on_left = count_entity(left, 'B');

    // Scene: Loading the boat
    if (boat_on_left) {
        update_left(left, targets, false);
    }

    char **scene = make_river_scene(left, targets);
    print_scene(scene);

    // if boat on left then remove it, otherwise add it
    if (boat_on_left) {
        update_left(left, "B", false);
    } else {
        strcat(left, "B");
    }

    // Scene: Crossing the rivers
    scene = make_river_scene(left, targets);
    print_scene(scene);

    // Scene: Unloading the boat
    if (!boat_on_left) {
        update_left(left, targets, true);
    }
    scene = make_river_scene(left, "");
    print_scene(scene);

    // count missionaries and cannibals on the left bank
    int missionaries = count_entity(left, 'M');
    int cannibals = count_entity(left, 'C');

    // deallocate memory for scene
    destroy_scene(scene);

    // missionaries eaten if they're outnumbered on either bank
    if (missionaries > 0 && cannibals > missionaries) {
        return ERROR_MISSIONARIES_EATEN;
    }
    if (MAX_CHARACTERS - missionaries > 0 &&
        MAX_CHARACTERS - cannibals > MAX_CHARACTERS - missionaries) {
        return ERROR_MISSIONARIES_EATEN;
    }

    // valid non-goal state
    if (strlen(left) > 0) {
        return VALID_NONGOAL_STATE;
    }
    return VALID_GOAL_STATE;
}

int play_game() {
    cout << "Game begins!" << endl;
    // initial state
    char play, left[8] = "BCCCMMM", targets[3];
    int result;
    char **scene = make_river_scene(left, "");
    print_scene(scene);
    destroy_scene(scene);

    do {
        cout << "Suggest a crossing." << endl;
        cin >> targets;

        cout << "Left is: " << left << " and Targets is: " << targets << endl;
        int result = perform_crossing(left, targets);

        cout << "Press 'y' if you'd like to continue playing otherwise press "
                "any key."
             << endl;
        cin >> play;
    } while (tolower(play) == 'y' || result == VALID_NONGOAL_STATE);

    return result;
}