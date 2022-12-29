#ifndef RIVER_H
#define RIVER_H

#include <map>
#include <vector>

// Entity set of interest
enum Entity {
    SUN,
    RIVER,
    BANK,
    BOAT,
    MISSIONARY,
    CANNIBAL,
};

// L (Left) or R (Right) direction
enum Direction {
    L,
    R,
};

// Status codes
enum Status {
    VALID_GOAL_STATE = 2,
    VALID_NONGOAL_STATE = 1,
    ERROR_INVALID_CANNIBAL_COUNT = -1,
    ERROR_INVALID_MISSIONARY_COUNT = -2,
    ERROR_INVALID_MOVE = -3,
    ERROR_MISSIONARIES_EATEN = -4,
    ERROR_INPUT_STREAM_ERROR = -5,
    ERROR_BONUS_NO_SOLUTION = -6
};

// Position (row and column) on 2-D array for the scene
struct Position {
    int row, col;
    Position();
    Position(int _row, int _col);
};

// Class for position information and manipulation
class EntityPosition {
  public:
    // Default positions for each entity and offset information
    Position sun = {3, 30};
    Position river = {19, 19};
    std::vector<Position> bank = {{0, 0}, {0, 53}};
    std::vector<Position> boat = {{17, 19}, {17, 36}};
    std::vector<Position> missionary = {{2, 1}, {2, 54}};
    std::vector<Position> cannibal = {{11, 1}, {11, 54}};
    int offset = 6;   // general column offset between characters

    /* Get coordinates (row, column) for a Missionary / Cannibal at 'index' on
     * either the left or right bank based on Direction 'd' */
    Position get_bank_position(Entity e, Direction d, int index);

    /* Get coordinates (row, column) at 'index' (first or second position)
     * on the boat at either the left or right bank based on Direction 'd' */
    Position get_boat_position(Direction d, int index);
};

// File name mapping (storing ASCII art) for each entity
const std::map<Entity, const char *> filename = {
    {SUN, "sun.txt"},
    {RIVER, "river.txt"},
    {BANK, "bank.txt"},
    {BOAT, "boat.txt"},
    {MISSIONARY, "missionary.txt"},
    {CANNIBAL, "cannibal.txt"},
};

// Character to Entity mapping
const std::map<const char, Entity> char_to_entity = {
    {'B', BOAT},
    {'M', MISSIONARY},
    {'C', CANNIBAL},
};

// Width and height of 2-D array storing scene
const int SCENE_HEIGHT = 24, SCENE_WIDTH = 72;

// Constraint on the maximum entities at each bank
const int MAX_CHARACTERS = 3;

/* Allocate memory for and initialise an empty ASCII-art scene. Return value is
 * a 2-D array of characters with SCENE_HEIGHT rows and SCENE_WIDTH columns */
char **create_scene();

/* Inserts the ASCII-art drawing stored in the file named 'filename' into the
 * given scene starting at row–column coordinates (row, col) */
bool add_to_scene(char **scene, int row, int col, const char *filename);

// Displays the ASCII-art scene stored in the 2-D array of characters 'scene'
void print_scene(char **scene);

// Frees the memory allocated by a call to create scene()
void destroy_scene(char **scene);

// Provides human readable strings describing the status code passed as input
const char *status_description(int code);

/* Takes two character strings: 'left' describing the contents of the left river
 * bank, and 'boat' describing the contents of the boat, and which returns a
 * corresponding ASCII-art scene. The character string for the left bank may be
 * up to 7 letters long and may contain three different kinds of characters:
 * - M denoting the presence of a missionary
 * - C denoting the presence of a cannibal
 * - B denoting that the boat is at the left bank.
 * The character string for the boat may be up to 2 letters long and may contain
 * only the characters M and C. */
char **make_river_scene(const char *left, const char *boat);

/* Performs a crossing of the river by one or two target entities from one
 * bank of the river to the other using the boat. The parameter 'left'
 * is both an input and an output parameter: it initially describes the contents
 * of the left river bank, but is modified to reflect the result of the move
 * (including an update on the boat’s position). The parameter 'targets' is a
 * string with one or two characters (M or C) according to the entities
 * performing the crossing */
int perform_crossing(char *left, const char *targets);

/* Allows a user to play the game of missionaries and cannibals by suggesting
 * boat crossings via the keyboard. The return value is an appropriate status
 * code */
int play_game();

#endif