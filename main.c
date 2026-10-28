#include<stdio.h>
#include<stdlib.h>  /* Needed for malloc() and free() */
#include "file_io.h" /* For reading the map file */
#include "game.h"    /* For game logic functions */
#include "terminal.h" /* For immediate char input */
#include "linkedlist.h" /* For the undo stack */

/*
 * A helper function that knows how to free the specific GameState struct.
 * This is passed to the linked list so it can free complex data.
 * PARAM: data, a void pointer to the GameState to be freed.
 */
void freeGameState(void* data)
{
    int i;
    GameState* state = (GameState*)data; /* Cast the void pointer back to the correct type */
    
    /* Free each row of the 2D map array */
    for (i = 0; i < state->mapRows; i++)
    {
        free(state->map[i]);
    }
    /* Free the array of pointers itself */
    free(state->map);
    /* Free the GameState struct */
    free(state);
}


/*
 * The main entry point of the program.
 */
int main(int argc, char * argv[])
{
    int i;
    int rows, cols;
    int playerRow, playerCol;
    char** map = NULL; /* Initialize to NULL for safety */
    char input;
    int gameStatus = 0; /* 0=playing, 1=win, -1=lose */
    int trapTriggered = 0; /* Flag to track if the trap has been sprung */
    LinkedList* undoStack = createLinkedList();

    /* Flag to track if the trap has been sprung */
    if (argc != 2)
    {
        printf("Usage: %s <map_file>\n", argv[0]);
    }
    else
    {
        /* Call the function to read the map file */
        map = readMapFile(argv[1], &rows, & cols);

        /* Check if the map was loaded successfully */
        if (map == NULL)
        {
            printf("Failed to load map file.\n");
        }
        else
        {
            /* Find the player's starting position on the newly loaded map*/
            findPlayer(map, rows, cols, &playerRow, &playerCol);

            
            /* The main game loop. It continues as long as the game is in a 'playing' state. */
             while (gameStatus == 0)
             {   
                 /* Print the current state of the map to the screen */
                 printMap(map, rows, cols);

                /* Get a single character from the user without needing to press Enter */
                 disableBuffer();
                 scanf(" %c", &input);
                 enableBuffer();

                 /* Handle the 'undo' command */
                 if (input == 'u')
                 {  
                    /* Try to get the previous state from the undo stack */
                    GameState* prevState = (GameState*)removeFirst(undoStack);

                    /* If a previous state exists, restore it */
                    if (prevState != NULL)
                    {   
                        /* Free the memory of the CURRENT map before replacing it */
                        for(i = 0; i < rows; i++) { free(map[i]); }
                        free(map);
                        
                         /* Restore all game variables from the previous state */
                        map = prevState->map;
                        rows = prevState->mapRows;
                        cols = prevState->mapCols;
                        playerRow = prevState->playerRow;
                        playerCol = prevState->playerCol;
                        trapTriggered = prevState->trapTriggered;

                        /* Free the prevState struct itself, but not its map, which is now in use */
                        free(prevState);
                    }
                 }
                 /* Handle movement commands */
                 else if (input == 'w' || input == 'a' || input == 's' || input == 'd')
                 {  
                    /* Create a new GameState to save the CURRENT state for a potential undo */
                    GameState* newState = (GameState*)malloc(sizeof(GameState));
                    newState->map = deepCopyMap(map, rows, cols);  /* Make a deep copy of the map */
                    newState->mapRows = rows;
                    newState->mapCols = cols;
                    newState->playerRow = playerRow;
                    newState->playerCol = playerCol;
                    newState->trapTriggered = trapTriggered;
                    
                    /* Add (push) the saved state onto the undo stack */
                    insertFirst(undoStack, newState);
                    
                     /* Attempt to move the player and update the game status */
                    gameStatus = movePlayer(map, rows, cols, &playerRow, &playerCol, input, &trapTriggered);

                     /* After a move, if the trap is active, spread the water */
                    if (trapTriggered == 1 && gameStatus == 0)
                    {
                        floodFill(map, rows, cols);

                        /* Check if the player has been submerged by the new water */
                        if (map[playerRow][playerCol] == '~')
                        {
                            gameStatus = -1;
                        }
                    }
                }
                 /* Any other input is ignored and the loop continues */
             }

             /* After the loop ends, print the final map and the result */
             printMap(map, rows, cols);
             if (gameStatus == 1)
             {
                printf("You win!\n");
             }
             else if (gameStatus == -1)
             {
                printf("You lose!\n");
             }
                 
             /* --- Final Memory Cleanup --- */
            /* Free the last active map */
            for (i = 0; i < rows; i++)
            {
                free(map[i]);
            }
            free(map);
            map = NULL; /* Good practice (as told by antoni)*/

            /* Free the undo stack and any remaining game states within it */
            freeLinkedList(undoStack, &freeGameState);
        }

    }
    return 0;
} 