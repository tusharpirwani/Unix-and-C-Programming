#ifndef GAME_H
#define GAME_H


/* * A struct to hold a complete snapshot of the game's state.
 * This is used by the undo feature to save and restore progress.
 */
typedef struct {
    char** map;
    int playerRow;
    int playerCol;
    int trapTriggered;
    int mapRows;
    int mapCols;
} GameState;

/*
 * Prints the entire game map to the terminal, including a border,
 * colored objects, and user instructions.
 */
void printMap(char** map, int rows, int cols);

/*
 * Finds the starting coordinates of the player ('P') on the map.
 */
void findPlayer(char** map, int rows, int cols, int* playerRow, int* playerCol);

/*
 * Processes a player's move, checks for collisions, and updates game state.
 * Returns an integer representing the current game status (0=continue, 1=win, -1=lose).
 */
int movePlayer(char** map, int rows, int cols, int* playerRow, int* playerCol, char move, int* trapTriggered);

/*
 * Removes all trapdoors ('X') from the map once the trap is triggered.
 */
void triggerTrap(char** map, int rows, int cols);

/*
 * Spreads water ('~') to adjacent empty cells in the map.
 */
void floodFill(char** map, int rows, int cols); 

/*
 * Creates and returns a new, independent copy of the 2D map array.
 * This is essential for the undo feature to work correctly.
 */
char** deepCopyMap(char** map, int rows, int cols); 

#endif