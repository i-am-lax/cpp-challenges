#include "maze.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {

    // This section illustrates the use of the pre-supplied functions */
    cout << "============== Pre-supplied functions ==================" << endl
         << endl;

    char **maze;
    int height, width;

    cout << "Loading simple maze...";
    // Loads the maze from the file "simple.txt" and sets height and width
    maze = load_maze("simple.txt", height, width);
    assert(maze);
    cout << " done (height = " << height << ", width = " << width << ")."
         << endl
         << endl;

    cout << "Printing simple maze:" << endl;
    // Prints the maze with row and column numbers
    print_maze(maze, height, width);
    cout << endl;

    cout << "====================== Question 1 ======================" << endl
         << endl;

    int row = 0, column = 0;
    cout << "Finding the entrance:" << endl;
    find_marker('>', maze, height, width, row, column);
    cout << "The entrance is at row " << row << " column " << column << endl
         << endl;

    cout << "Finding the exit:" << endl;
    find_marker('X', maze, height, width, row, column);
    cout << "The exit is at row " << row << " column " << column << endl
         << endl;

    cout << "====================== Question 2 ======================" << endl
         << endl;

    cout << "The move sequence 'EEEEESEENN' is ";
    if (!valid_solution("EEEEESEENN", maze, height, width))
        cout << "NOT ";
    cout << "a solution to the maze" << endl << endl;

    cout << "The move sequence 'EEEEEEEESSSSSS' is ";
    if (!valid_solution("EEEEEEEESSSSSS", maze, height, width))
        cout << "NOT ";
    cout << "a solution to the maze" << endl << endl;

    cout << "The move sequence 'ESSSSSSEEEEEEE' is ";
    if (!valid_solution("ESSSSSSEEEEEEE", maze, height, width))
        cout << "NOT ";
    cout << "a solution to the maze" << endl << endl;

    cout << "====================== Question 3 ======================" << endl
         << endl;

    char *path;

    // Easy example with a known solution
    cout << "A path through the maze from '>' to 'X' is: ";
    path = find_path(maze, height, width, '>', 'X');
    cout << path << endl;

    cout << "The path is shown below: " << endl;
    print_maze(maze, height, width);
    cout << endl;

    deallocate_char_array(path);
    deallocate_2D_array(maze, height);

    // An impossible example - should return "no solution"
    maze = load_maze("simple.txt", height, width);
    assert(maze);

    cout << "A path through the maze from '>' to 'U' is: ";
    path = find_path(maze, height, width, '>', 'U');
    cout << path << endl;

    deallocate_char_array(path);
    deallocate_2D_array(maze, height);

    cout << "=================== Bonus Question =====================" << endl
         << endl;

    // Find path from the entrance to the middle of the Hatfield House maze
    maze = load_maze("hatfield.txt", height, width);
    assert(maze);

    cout << "And now the Hatfield House maze:" << endl;
    print_maze(maze, height, width);
    cout << endl;

    cout << "A path through the maze from '>' to 'M' is: " << endl;
    path = find_path(maze, height, width, '>', 'M');
    cout << path << endl;

    cout << "The path is shown below: " << endl;
    print_maze(maze, height, width);
    cout << endl;

    deallocate_char_array(path);
    deallocate_2D_array(maze, height);

    /* Find path from the middle to the exit of the Hatfield House maze */
    maze = load_maze("hatfield.txt", height, width);
    assert(maze);

    cout << "A path through the maze from 'M' to 'X' is: " << endl;
    path = find_path(maze, height, width, 'M', 'X');
    cout << path << endl;

    cout << "The path is shown below: " << endl;
    print_maze(maze, height, width);
    cout << endl;

    deallocate_char_array(path);
    deallocate_2D_array(maze, height);

    cout << "======================= The End ========================" << endl
         << endl;

    return 0;
}