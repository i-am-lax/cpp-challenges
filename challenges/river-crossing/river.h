#ifndef RIVER_H
#define RIVER_H

#include <map>
#include <vector>

enum Element {
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

// Position (row and column) on scene array
struct Position {
    int row, col;
    Position();
    Position(int _row, int _col);
};

// Class for position information and manipulation
class ElementPositions {
  public:
    Position sun = {3, 30};
    Position river = {19, 19};
    std::vector<Position> bank = {{0, 0}, {0, 53}};
    std::vector<Position> boat = {{17, 19}, {17, 36}};
    std::vector<Position> missionary = {{2, 1}, {2, 54}};
    std::vector<Position> cannibal = {{11, 1}, {11, 54}};
    int offset = 6;   // general column offset between characters

    /* Get coordinates (row, column) for a Missionary / Cannibal at 'index' at
     * either the left or right bank based on Direction 'd' and 'index' */
    Position get_bank_position(Element e, Direction d, int index);

    /* Get coordinates (row, column) at 'index' either first or second position)
     * on the boat at either the left or right bank based on Direction 'd' */
    Position get_boat_position(Direction d, int index);
};

// File name mapping
const std::map<Element, const char *> filename = {
    {SUN, "sun.txt"},
    {RIVER, "river.txt"},
    {BANK, "bank.txt"},
    {BOAT, "boat.txt"},
    {MISSIONARY, "missionary.txt"},
    {CANNIBAL, "cannibal.txt"},
};

const int SCENE_HEIGHT = 24, SCENE_WIDTH = 72;

const int MAX_CHARACTERS = 3;

char **create_scene();
bool add_to_scene(char **scene, int row, int col, const char *filename);
void print_scene(char **scene);
void destroy_scene(char **scene);
const char *status_description(int code);

char **make_river_scene(const char *left, const char *boat);
int perform_crossing(char* left, const char* targets);
int play_game();

#endif