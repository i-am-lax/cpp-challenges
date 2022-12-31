#ifndef TUBE_H
#define TUBE_H

// constraint on maximum string length
const int MAX_LENGTH = 100;

// filename storing tube lines and symbol representation
const char LINES[] = "lines.txt";
const char STATIONS[] = "stations.txt";

// Set of possible directions
enum Direction { N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION };

// Error codes for validate_route
#define ERROR_START_STATION_INVALID         -1
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION             -5
#define ERROR_OFF_TRACK                     -6
#define ERROR_OUT_OF_BOUNDS                 -7

/* Reads in an ASCII map from the file with name 'filename', sets the output
 * parameters 'height' and 'width' according to map dimensions, and returns a 2D
 * ('height' × 'width') array of characters */
char **load_map(const char *filename, int &height, int &width);

/* Displays the ASCII map stored in the 2D ('height' × 'width') array of
 * characters 'm' */
void print_map(char **m, int height, int width);

/* Provides human readable strings describing the (negative integer) error code
 * passed as a parameter */
const char *error_description(int code);

/* Converts a string describing a direction ("N", "S", "W", "E", "NE", "NW",
 * "SE", "SW") into a corresponding instance of enumerated type Direction */
Direction string_to_direction(const char *token);

/* Given a symbol character 'target' finds the coordinates ('r', 'c') of the
 * first occurrence of that symbol on an ASCII 'map' with 'height' rows and
 * 'width' columns. If the symbol is found, the function returns true and sets
 * the output parameters 'r' and 'c'. If the symbol cannot be found, the
 * function returns false and 'r' and 'c' are set to -1 */
bool get_symbol_position(char **map, const int height, const int width,
                         const char target, int &r, int &c);

/* Given the input parameter 'name' describing the name of a station or line,
 * the corresponding map symbol character is returned. If there is no such
 * station or line, the space character is returned */
char get_symbol_for_station_or_line(const char *name);

/* Given the name of an origin station 'start_station' and string 'route'
 * describing a passenger journey in terms of the direction taken on the ASCII
 * tube 'map', at each journey step, determines if the route is valid. If the
 * route is invalid then an error code is returned, otherwise the number of line
 * changes required to complete the journey is returned along with the
 * destination station in 'destination' */
int validate_route(char **map, const int &height, const int &width,
                   const char *start_station, const char *route,
                   char *destination);

#endif