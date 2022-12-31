#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

#include "tube.h"

void test_validate_route(char **map, int &height, int &width, const char *route,
                         const char *name, char *destination) {
    cout << "Starting at " << name << " and taking the steps:" << endl;
    cout << route << endl;
    int result = validate_route(map, height, width, name, route, destination);
    if (result >= 0)
        cout << "is a valid route with " << result
             << " line change(s) ending at " << destination << "." << endl;
    else
        cout << "is an invalid route (" << error_description(result) << ")"
             << endl;
    cout << endl;
}

int main() {

    cout << "============== Pre-supplied functions ==================" << endl
         << endl;

    char **map;
    int height, width;

    cout << "Loading tube map with load_map():";
    map = load_map("map.txt", height, width);
    assert(map);
    cout << " done (height = " << height << ", width = " << width << ")."
         << endl
         << endl;

    cout << "Printing map with print_map():" << endl;
    print_map(map, height, width);
    cout << endl;

    cout << "====================== Question 1 ======================" << endl
         << endl;

    int r, c;
    bool success = get_symbol_position(map, height, width, 'T', r, c);
    cout << "The symbol 'T' is ";
    if (success)
        cout << "found at (" << r << "," << c << ")";
    else
        cout << "not found.";
    cout << endl << endl;

    success = get_symbol_position(map, height, width, '4', r, c);
    cout << "The symbol '4' is ";
    if (success)
        cout << "found at (" << r << "," << c << ")";
    else
        cout << "not found.";
    cout << endl << endl;

    success = get_symbol_position(map, height, width, 'z', r, c);
    cout << "The symbol 'z' is ";
    if (success)
        cout << "found at (" << r << "," << c << ")";
    else
        cout << "not found.";
    cout << endl << endl;

    cout << "====================== Question 2 ======================" << endl
         << endl;

    cout << "The symbol for Victoria station is '"
         << get_symbol_for_station_or_line("Victoria") << "'" << endl
         << endl;

    cout << "The symbol for Oxford Circus station is '"
         << get_symbol_for_station_or_line("Oxford Circus") << "'" << endl
         << endl;

    cout << "The symbol for the District Line is '"
         << get_symbol_for_station_or_line("District Line") << "'" << endl
         << endl;

    /* Birmingham station is not on the Tube map, so this should return ' ' */
    cout << "The symbol for Birmingham station is '"
         << get_symbol_for_station_or_line("Birmingham") << "'" << endl
         << endl;

    cout << "====================== Question 3 ======================" << endl
         << endl;

    char route[512], destination[512] = "nowhere";

    /* Invalid route where start station does not exist */
    test_validate_route(map, height, width, "S,SE,S,S,E,E,E",
                        "Cambridge Circus", destination);

    /* Invalid direction 'Z' */
    test_validate_route(map, height, width, "S,SE,Z,S", "Oxford Circus",
                        destination);

    /* Invalid route which goes out of bounds (starting from 'K') */
    test_validate_route(map, height, width, "N,N,E", "Aldgate", destination);

    /* Invalid route which goes off the track (starting from 'K') */
    test_validate_route(map, height, width, "N,N,W", "Aldgate", destination);

    /* Valid route to Leicester Square with 1 line change */
    test_validate_route(map, height, width, "S,SE,S,S,E,E,E,E,E,E,E,E,E,E,E",
                        "Oxford Circus", destination);

    /* Valid route from Goodge Street to Covent Garden with 2 line changes */
    test_validate_route(map, height, width, "S,S,S,E,E,E,E,E,E,SW,SW,SW",
                        "Goodge Street", destination);

    /* Invalid route because of line hopping between stations */
    test_validate_route(map, height, width, "N,N,N,N,N,NE,W", "London Bridge",
                        destination);

    /* Invalid route because of line hopping between stations */
    test_validate_route(map, height, width, "W,W,N", "South Kensington",
                        destination);

    /* Invalid route because of backtracking between stations */
    test_validate_route(map, height, width, "W,W,E,W,W,W", "Sloane Square",
                        destination);

    /* Invalid route because route goes outside of map bounds */
    test_validate_route(map, height, width, "E,NE,SE,SE,SE", "Marylebone",
                        destination);
    strcpy(route, "E,NE,SE,SE,SE");

    return 0;
}