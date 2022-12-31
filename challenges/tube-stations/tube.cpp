#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#include "tube.h"

// Internal helper function which allocates a dynamic 2D array
char **allocate_2D_array(int rows, int columns) {
    char **m = new char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new char[columns];
        assert(m[r]);
    }
    return m;
}

// Internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
    for (int r = 0; r < rows; r++)
        delete[] m[r];
    delete[] m;
}

/* Internal helper function which gets the dimensions of a map from 'filename'
 * and stores in 'height' and 'width' */
bool get_map_dimensions(const char *filename, int &height, int &width) {
    char line[512];

    ifstream input(filename);

    height = width = 0;

    input.getline(line, 512);
    while (input) {
        if ((int) strlen(line) > width)
            width = strlen(line);
        height++;
        input.getline(line, 512);
    }

    if (height > 0)
        return true;
    return false;
}

/* Pre-supplied function to load an ASCII tube map from a file ('filename') and
 * add dimensions to 'height' and 'width' */
char **load_map(const char *filename, int &height, int &width) {

    bool success = get_map_dimensions(filename, height, width);

    if (!success)
        return NULL;

    char **m = allocate_2D_array(height, width);

    ifstream input(filename);

    char line[512];
    char space[] = " ";

    for (int r = 0; r < height; r++) {
        input.getline(line, 512);
        strcpy(m[r], line);
        while ((int) strlen(m[r]) < width)
            strcat(m[r], space);
    }

    return m;
}

/* Pre-supplied function to print the ASCII map ('height' × 'width') stored in
 * 'm' */
void print_map(char **m, int height, int width) {
    cout << setw(2) << " "
         << " ";
    for (int c = 0; c < width; c++)
        if (c && (c % 10) == 0)
            cout << c / 10;
        else
            cout << " ";
    cout << endl;

    cout << setw(2) << " "
         << " ";
    for (int c = 0; c < width; c++)
        cout << (c % 10);
    cout << endl;

    for (int r = 0; r < height; r++) {
        cout << setw(2) << r << " ";
        for (int c = 0; c < width; c++)
            cout << m[r][c];
        cout << endl;
    }
}

/* Pre-supplied helper function to provide human readable strings describing the
 * (negative integer) error 'code' */
const char *error_description(int code) {
    switch (code) {
    case ERROR_START_STATION_INVALID:
        return "Start station invalid";
    case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
        return "Route endpoint is not a station";
    case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
        return "Line hopping between stations not possible";
    case ERROR_BACKTRACKING_BETWEEN_STATIONS:
        return "Backtracking along line between stations not possible";
    case ERROR_INVALID_DIRECTION:
        return "Invalid direction";
    case ERROR_OFF_TRACK:
        return "Route goes off track";
    case ERROR_OUT_OF_BOUNDS:
        return "Route goes off map";
    }
    return "Unknown error";
}

/* Pre-supplied helper function for converting string 'token' ("N", "S", "W",
 * "E", "NE", "NW", "SE", "SW") to Direction enum */
Direction string_to_direction(const char *token) {
    const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
    for (int n = 0; n < 8; n++) {
        if (!strcmp(token, strings[n]))
            return (Direction) n;
    }
    return INVALID_DIRECTION;
}

/* Returns true and sets the coordinates ('r', 'c') if the character 'target' is
 * found in the input 'map' with 'height' rows and 'width' columns. Otherwise
 * returns false and sets 'r' = -1 and 'c' = -1  */
bool get_symbol_position(char **map, const int height, const int width,
                         const char target, int &r, int &c) {
    // iterate through map and identify the symbol 'target'
    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (map[row][col] == target) {
                r = row;
                c = col;
                return true;
            }
        }
    }
    // set to -1 if 'target' not found
    r = -1;
    c = -1;
    return false;
}

/* Internal helper function to generate a mapping from 'filename' whereby the
 * key is a string and the value is a character. This function applies to the
 * structure of the files given by STATIONS and LINES constants */
map<const string, char> create_char_map(const char *filename) {
    // declare variables for generating map
    char value, str[MAX_LENGTH];
    map<const string, char> mapping;

    // instantiate input filestream
    ifstream in;
    in.open(filename);

    // add values to mapping
    while (!in.eof()) {
        string key;
        // read line and set first character to 'value'
        in.getline(str, MAX_LENGTH);
        value = str[0];
        // append the remaining characters to 'key'
        for (int i = 2; i < strlen(str); i++) {
            key += str[i];
        }
        // insert into map
        mapping[key] = value;
    }
    in.close();

    return mapping;
}

/* Returns the symbol character corresponding to the input station or line
 * ('name') if it exists otherwise returns the space character. The station and
 * line information is loaded from text files whose names are given by STATIONS
 * and LINES */
char get_symbol_for_station_or_line(const char *name) {
    // default is space character
    char output = ' ';

    // generate station and line maps
    map<const string, char> stations = create_char_map(STATIONS),
                            lines = create_char_map(LINES);

    // check if 'name' exists in either map and if so, set output character
    if (stations.count(name)) {
        output = stations.at(name);
    } else if (lines.count(name)) {
        output = lines.at(name);
    }
    return output;
}

/* Internal helper function to check whether or not 'row' and 'col' are within
 * range in terms of 'height' and 'width' */
bool is_valid_indices(const int &row, const int &col, const int &height,
                      const int &width) {
    if (row < 0 || row > height - 1) {
        return false;
    }
    if (col < 0 || col > width - 1) {
        return false;
    }
    return true;
}

/* Internal helper function to increment / decrement the input 'row' and 'col'
 * values (representing a position on the map) based on the Direction 'd' */
void update_position(Direction d, int &row, int &col) {
    switch (d) {
    case N:
        row--;
        break;
    case S:
        row++;
        break;
    case W:
        col--;
        break;
    case E:
        col++;
        break;
    case NE:
        row--;
        col++;
        break;
    case NW:
        row--;
        col--;
        break;
    case SE:
        row++;
        col++;
        break;
    case SW:
        row++;
        col--;
        break;
    case INVALID_DIRECTION:
        cerr << error_description(INVALID_DIRECTION) << endl;
        break;
    }
}

/* Internal helper function to return the opposite direction to the input 'd' */
Direction opposite_direction(Direction &d) {
    switch (d) {
    case N:
        return S;
    case S:
        return N;
    case E:
        return W;
    case W:
        return E;
    case NE:
        return SW;
    case NW:
        return SE;
    case SE:
        return NW;
    case SW:
        return NE;
    case INVALID_DIRECTION:
        cerr << error_description(INVALID_DIRECTION) << endl;
        return INVALID_DIRECTION;
    }
}

/* Internal helper function to update 'route_vector' which is a vector of type
 * Direction by extracting tokens from 'route' which is a comma-separated string
 * of directions e.g. "S,SE,E,N". Return true if extraction successful */
bool create_route_vector(const char *route, vector<Direction> &route_vector) {

    // create copy so we don't modify route
    char copy[MAX_LENGTH];
    strcpy(copy, route);

    // tokenise on comma and push to vector
    Direction d;
    char *r = strtok(copy, ",");
    while (r) {
        d = string_to_direction(r);
        if (d == INVALID_DIRECTION) {
            return false;
        }
        route_vector.push_back(d);
        r = strtok(NULL, ",");
    }
    return true;
}

/* Given the name of an origin station 'start_station' and string 'route'
 * describing a journey on 'map', at each journey step, determines if the route
 * is valid. If the route is invalid then an error code is returned, otherwise
 * the number of line changes required to complete the journey is returned along
 * with the final station in 'destination'. Logic:
 * - if 'start_station' does not exist then return ERROR_START_STATION_INVALID
 * - if a given direction in 'route' is invalid then return
 * ERROR_INVALID_DIRECTION
 * - if route strays outside bounds of the map return ERROR_OUT_OF_BOUNDS
 * - if route strays off a station or line/track, return ERROR_OFF_TRACK
 * - if attempt made to change lines outside of a station, return
 * ERROR_LINE_HOPPING_BETWEEN_STATIONS
 * - if attempt to retrace a journey step outside of a station, return
 * ERROR_BACKTRACKING_BETWEEN_STATIONS
 * - if endpoint is not a station return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION */
int validate_route(char **map, const int &height, const int &width,
                   const char *start_station, const char *route,
                   char *destination) {
    // line changes
    int changes = 0;

    // load station mapping
    std::map<const string, char> stations = create_char_map(STATIONS);

    // validate start station
    if (!stations.count(start_station)) {
        return ERROR_START_STATION_INVALID;
    }

    // validate route
    vector<Direction> route_vector;
    if (!create_route_vector(route, route_vector)) {
        return ERROR_INVALID_DIRECTION;
    }

    // get symbol for start station
    char symbol = get_symbol_for_station_or_line(start_station);

    // get coordinates for start station
    int row, col;
    get_symbol_position(map, height, width, symbol, row, col);

    // traverse the route
    char current_line = ' ', prev_position;
    Direction prev_direction = INVALID_DIRECTION;
    for (auto const &r : route_vector) {

        // record previous position and direction
        prev_position = map[row][col];

        // check for backtracking
        if (prev_direction != INVALID_DIRECTION &&
            r == opposite_direction(prev_direction) &&
            !isalnum(prev_position)) {
            return ERROR_BACKTRACKING_BETWEEN_STATIONS;
        }
        prev_direction = r;

        // move to the next step
        update_position(r, row, col);

        // check if row and column are valid positions on map
        if (!is_valid_indices(row, col, height, width)) {
            return ERROR_OUT_OF_BOUNDS;
        }

        // check if at a station or on a track
        if (map[row][col] == ' ') {
            return ERROR_OFF_TRACK;
        }

        // get the starting tube line
        if (current_line == ' ') {
            current_line = map[row][col];
        }

        if (!isalnum(map[row][col])) {
            if (current_line != map[row][col]) {
                if (isalnum(prev_position)) {
                    changes += 1;
                    current_line = map[row][col];
                } else {
                    return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
                }
            }
        }
    }
    // check if final position is a station
    if (!isalnum(map[row][col])) {
        return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
    }

    for (std::map<const string, char>::iterator it = stations.begin();
         it != stations.end(); it++) {
        if (it->second == map[row][col]) {
            // copy_string(it->first, destination);
            strcpy(destination, it->first.c_str());
        }
    }
    return changes;
}