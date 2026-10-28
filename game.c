#include <stdio.h>
#include <stdlib.h> /* Required for system("clear") */
#include "game.h"
#include "color.h"  /* For setting background colors */


/*
 * Prints the current state of the map to the terminal, including borders and colors.
 */
void printMap(char** map, int rows, int cols)
{
    int i, j;

    /* Clears the terminal screen for a fresh display */
    system("clear");

    /* Loop through each cell of the grid plus a border */
    for (i = 0; i < rows + 2; i++)
    {
        for (j = 0; j < cols + 2; j++)  
        {   
            /* If it's a border cell, print a '*' */
            if (i == 0 || i == rows + 1 || j == 0 || j == cols + 1)
            {
                printf("*");
            }
            else
            {   
                /* Get the actual map character for the current position */
                char current_char = map[i-1][j-1];
                
                 /* Check if the character needs a special background color */
                switch(current_char)
                {
                    case '@':
                        setBackground("red");
                        printf("%c", current_char);
                        setBackground("reset");
                        break;
                    case 'G':
                        setBackground("green");
                        printf("%c", current_char);
                        setBackground("reset");
                        break;
                    case '~':
                        setBackground("blue");
                        printf("%c", current_char);
                        setBackground("reset");
                        break;

                    /* If no special color, just print the character */
                    default:
                        printf("%c", current_char);
                }
            }
        }
        printf("\n");  /* Newline after each row */
    }
    
    /* Print user instructions below the map */
    printf("Press 'w' to move UP\n");
    printf("Press 's' to move DOWN\n");
    printf("Press 'a' to move LEFT\n");
    printf("Press 'd' to move RIGHT\n");
    printf("Press 'u' to UNDO\n");
}

/*
 * Finds the initial position of the player ('P') on the map.
 * The coordinates are returned via output parameters.
 */ 
void findPlayer(char** map, int rows, int cols, int* playerRow, int*playerCol)
{
    int i, j;

    /* Iterate through every cell of the map */
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {   
            /* When 'P' is found, store its coordinates */
            if (map[i][j] == 'P')
            {
                *playerRow = i;
                *playerCol = j;
            }
        }
    }

    
}


/*
 * Removes all trapdoors ('X') from the map by replacing them with empty spaces.
 */
void triggerTrap(char** map, int rows, int cols)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (map[i][j] == 'X')
            {
                map[i][j] = ' ';
            }
        }
    }

}


/*
 * Implements a two-pass flood fill algorithm to spread water.
 */
void floodFill(char** map, int rows, int cols)
{
    int i, j;
    /* First pass: Mark where new water should be without changing it yet with a temp variable ('#') */
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (map[i][j] == '~')
            {
                /* Check and mark neighbors (up, down, left, right) if they are empty */
                if (i > 0 && map[i-1][j] == ' ') map[i-1][j] = '#';
                if (i < rows-1 && map[i+1][j] == ' ') map[i+1][j] = '#';
                if (j > 0 && map[i][j-1] == ' ') map[i][j-1] = '#';
                if (j < cols-1 && map[i][j+1] == ' ') map[i][j+1] = '#';
            }
        }
    }

    /* Second pass: Change all marked cells ('#') into actual water ('~'). */
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            if (map[i][j] == '#')
            {
                map[i][j] = '~';
            }
        }
    }

}

/*
 * Handles the player's movement based on user input.
 * Returns the game status after the move.
 */
int movePlayer(char** map, int rows, int cols, int* playerRow, int*playerCol, char move, int* trapTriggered)
{
    int nextRow, nextCol;
    int gameStatus = 0; /* 0 = continue, 1 = win, -1 = lose */
    char destination;
    
    /* Start with the player's current position */
    nextRow = *playerRow;
    nextCol = *playerCol;

    /* Determine the new potential coordinates based on the move command*/
    switch(move)
    {
        case 'w': nextRow--; break;
        case 's': nextRow++; break;
        case 'a': nextCol--; break;
        case 'd': nextCol++; break;
    }

    /* Check if the move is valid (within bounds and not into a wall or trapdoor) */
    if (nextRow >= 0 && nextRow < rows && nextCol >= 0 && nextCol < cols && 
        map[nextRow][nextCol] != 'O' && map[nextRow][nextCol] != 'X')
    {   
        /* Get the character at the destination tile */
        destination = map[nextRow][nextCol];

        /* Check what the player is moving onto */
        if (destination == 'G')
        {
            gameStatus = 1; /* Player wins */
        }
        else if (destination == '~')
        {
            gameStatus = -1; /* Player loses */
        }
        else if (destination == '@')
        {
            triggerTrap(map, rows, cols); /* Trigger the trap mechanism */
            *trapTriggered = 1; /* Set the trap flag to true */
        }

        /* Update the map with the player's new position */
        map[*playerRow][*playerCol] = ' '; /* Set old position to empty */
        *playerRow = nextRow;              /* Update player's row */
        *playerCol = nextCol;              /* Update player's column */
        map[*playerRow][*playerCol] = 'P'; /* Set new position to 'P' */
    }

    return gameStatus;

}


/*
 * Creates and returns a new 2D char array that is a complete,
 * independent copy of the original map.
 */
char** deepCopyMap(char** map, int rows, int cols)
{
    int i, j;
    
      /* Allocate memory for the rows (array of pointers) */
    char** newMap = (char**)malloc(rows * sizeof(char*));

    /* For each row, allocate memory for the columns and copy the data */
    for(i = 0; i < rows; i++)
    {
        newMap[i] = (char*)malloc(cols * sizeof(char));
        for(j = 0; j < cols; j++)
        {
            newMap[i][j] = map[i][j];
        }
    }
    return newMap;
}