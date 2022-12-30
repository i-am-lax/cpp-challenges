// Set of outcomes for a move
enum MoveResult {
    INVALID_MOVE = -3,
    REDUNDANT_MOVE = -2,
    BLOWN_UP = -1,
    SOLVED_BOARD = 1,
    VALID_MOVE = 0
};

/* Pre-supplied helper functions to load, display and initialise the board */
void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

/* Takes 2 9×9 arrays of characters, the first ('mines') representing mine
 * locations and the second ('revealed') the current playing board, and returns
 * true if all non-mine squares in the playing board have been uncovered */
bool is_complete(const char mines[9][9], const char revealed[9][9]);

/* Returns the number of mines around a particular square. 'position' is a
 * two-character string denoting row and column board coordinates (e.g. "I8")
 * and 'mines' is a 2D character array of mine locations */
int count_mines(const char *position, const char mines[9][9]);

/* Uncovers or flags a square on the current board 'revealed' at the given
 * 'position'. If the third character of 'position' is '*' the move is to flag
 * the square in 'revealed', otherwise we uncover the square. */
MoveResult make_move(const char *position, const char mines[9][9],
                     char revealed[9][9]);

/* Determines if a risk-free move (no guess-work) is available starting from the
 * current playing board 'revealed'. The return value of the function is true if
 * a risk-free move(s) is available, and the output string 'move' contains the
 * sequence. Otherwise the return value of the function is false and 'move' is
 * empty string */
bool find_safe_move(const char revealed[9][9], char *move);