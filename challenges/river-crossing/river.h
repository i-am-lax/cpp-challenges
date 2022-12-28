#ifndef RIVER_H
#define RIVER_H

#include <map>

enum Element {
    SUN,
    RIVER,
    BANK,
    BOAT,
    MISSIONARY,
    CANNIBAL,
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
    int row = -1, col = -1;
    Position(int _row, int _col);
};

// Class for position information and manipulation
class ElementPositions {
    public:
        Position sun = {3, 30};
        Position river = {19,19};
        Position lbank = {0, 0};
        Position rbank = {0, 53};
        Position lboat = {17, 19};
        Position lmissionary = {2, 1};
        Position lcannibal = {11, 1};
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

const ElementPositions rs;

const int SCENE_HEIGHT = 24, SCENE_WIDTH = 72;

char **create_scene();
bool add_to_scene(char **scene, int row, int col, const char *filename);
void print_scene(char **scene);
void destroy_scene(char **scene);

char **make_river_scene(const char *left, const char *boat);

#endif