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

/* You are pre-supplied with the functions below. Add your own
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
    char **m = new char *[rows];
    assert(m);
    for (int r = 0; r < rows; r++) {
        m[r] = new char[columns];
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

/* internal helper function which gets the dimensions of a map */
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

/* pre-supplied function to load a tube map from a file*/
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

/* pre-supplied function to print the tube map */
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

/* pre-supplied helper function to report the errors encountered in Question 3
 */
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

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
    const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
    for (int n = 0; n < 8; n++) {
        if (!strcmp(token, strings[n]))
            return (Direction) n;
    }
    return INVALID_DIRECTION;
}

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

map<const string, char> create_char_map(const char *filename) {
    // declare variables for generating map
    map<const string, char> mapping;
    char value;
    char str[MAX_LENGTH];

    // instantiate input filestream
    ifstream in;
    in.open(filename);

    // add values to mapping
    while (!in.eof()) {
        string key;
        in.getline(str, MAX_LENGTH);
        value = str[0];
        for (int i = 2; i < strlen(str); i++) {
            key += str[i];
        }
        mapping[key] = value;
        str[0] = '\0';
    }

    return mapping;
}

char get_symbol_for_station_or_line(const char *name) {
    // default is space character
    char output = ' ';

    // load in station and line data
    map<const string, char> stations = create_char_map(STATIONS);
    map<const string, char> lines = create_char_map(LINES);

    // check if 'name' exists in either map and if so, return it
    if (stations.count(name)) {
        output = stations.at(name);
    } else if (lines.count(name)) {
        output = lines.at(name);
    }
    return output;
}

bool create_route_vector(const char *route, vector<Direction> &route_vector) {
    char token[3];
    Direction d;

    int idx = 0;
    while (*route != '\0') {
        if (*route == ',') {
            token[idx] = '\0';
            d = string_to_direction(token);
            if (d == INVALID_DIRECTION) {
                return false;
            }
            route_vector.push_back(d);
            idx = 0;
            token[idx] = '\0';
        } else if (*route != ',') {
            token[idx] = *route;
            idx++;
        }
        route++;
    }
    return true;
}

void update_position(Direction d, int &row, int &col) {
    switch (d) {
    case N:
        row++;
        break;
    case S:
        row--;
        break;
    case W:
        col--;
        break;
    case E:
        col++;
        break;
    case NE:
        row++;
        col++;
        break;
    case NW:
        row++;
        col--;
        break;
    case SE:
        row--;
        col++;
        break;
    case SW:
        row--;
        col--;
        break;
    case INVALID_DIRECTION:
        break;
    default:
        cerr << "Invalid." << endl;
    }
}

bool is_valid_indices(const int &row, const int &col, const int &height, const int &width) {
    if (row < 0 || row > height - 1) {
        return false;
    }
    if (col < 0 || col > width - 1) {
        return false;
    }
    return true;
}

void copy_string(string str, char* cstr) {
    for (int i = 0; i < str.length(); i++) {
        *cstr = str[i];
        cstr++;
    }
    *cstr = '\0';
}

int validate_route(char **map, const int &height, const int &width,
                   const char *start_station, const char *route,
                   char *destination) {
    // line changes
    int changes = 0;

    // load station mapping
    std::map<const string, char> stations = create_char_map(STATIONS);

    // validate start station
    if (!stations.count(start_station)) {
        cout << "ERROR_START_STATION_INVALID" << endl;
        return ERROR_START_STATION_INVALID;
    }

    // validate route
    vector<Direction> route_vector;
    if (!create_route_vector(route, route_vector)) {
        cout << "ERROR_INVALID_DIRECTION" << endl;
        return ERROR_INVALID_DIRECTION;
    }

    // get symbol for start station
    char symbol = get_symbol_for_station_or_line(start_station);

    // get coordinates for start station
    int row, col;
    get_symbol_position(map, height, width, symbol, row, col);

    cout << "start position: [" << row <<"," << col << "]" << endl;

    // traverse the route
    char line = ' ', prev;
    for (auto const &r: route_vector) {
        // get previous position
        prev = map[row][col];

        // move to the next step
        update_position(r, row, col);
        cout << "position is now [" << row<<"," << col <<  "]" << endl;

        // check if row and column are valid positions on map
        if (!is_valid_indices(row, col, height, width)) {
            cout << "ERROR_OUT_OF_BOUNDS" << endl;
            return ERROR_OUT_OF_BOUNDS;
        }
        
        // check if at a station or on a track
        if (map[row][col] == ' ') {
            cout << "ERROR_OFF_TRACK" << endl;
            return ERROR_OFF_TRACK;
        }

        // get the starting tube line
        if (line == ' ') {
            line = map[row][col];
        }

        if (!isalnum(map[row][col])) {
            if (line != map[row][col]) {
                if (isalnum(prev)) {
                    changes += 1;
                    line = map[row][col];
                }
                else {
                    cout << "ERROR_LINE_HOPPING_BETWEEN_STATIONS" << endl;
                    return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
                }
            }
        }
    }
    // check if final position is a station
    if (!isalnum(map[row][col])) {
        cout << "ERROR_ROUTE_ENDPOINT_IS_NOT_STATION" << endl;
        return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
    }

    for (std::map<const string, char>::iterator it = stations.begin(); it != stations.end(); it++) {
        if (it->second == map[row][col]) {
            copy_string(it->first, destination);
        }
    }
    return changes;
}