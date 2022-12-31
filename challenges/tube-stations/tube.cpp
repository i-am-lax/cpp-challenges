#include <cassert>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>

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
    }
    else if (lines.count(name)) {
        output = lines.at(name);
    }
    return output;
}