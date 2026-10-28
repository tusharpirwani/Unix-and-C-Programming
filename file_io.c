#include <stdio.h>
#include <stdlib.h>
#include "file_io.h"


/*
 * Reads a map from a text file and creates a dynamically allocated 2D char array.
 */
char** readMapFile(char* filename, int* outRows, int* outCols)
{
    /* All variables are declared at the top, following C89 standard. */
    char** map = NULL;
    FILE* file = NULL;
    int i, j;
    int mapRows, mapCols;
    int temp;
    
    /* Attempt to open the specified file in read mode */
    file = fopen(filename, "r");

     /* Check if the file opened successfully. If not, fopen returns NULL. */
    if (file == NULL)
    {
        perror("Error opening file");
        /* If the file fails to open, 'map' remains NULL. which will be returned at the end to signal an error. */
    }
    else
    {
         /* --- File opened successfully, proceed with reading --- */
        
        /* Read the first line to get the map dimensions */
        fscanf(file, "%d %d", &mapRows, &mapCols);

        /* Store the dimensions in the output parameters for the main function to use */
        *outRows = mapRows;
        *outCols = mapCols;

        /* Allocate memory for the rows (an array of char pointers) */
        map = (char**)malloc(sizeof(char*) * mapRows);

        /* For each row, allocate memory for the columns (an array of chars) */
        for (i = 0; i < mapRows; i++)
        {
            map[i] = (char*)malloc(sizeof(char) * mapCols);
        }

        /* Read the map data from the file, integer by integer */
        for (i = 0; i < mapRows; i++)
        {
            for (j = 0; j < mapCols; j++)
            {
                fscanf(file, "%d", &temp);
                /* Convert the integer code into the correct map character */
                switch(temp)
                {
                    case 0: map[i][j] = ' '; break;
                    case 1: map[i][j] = 'O'; break;
                    case 2: map[i][j] = '~'; break;
                    case 3: map[i][j] = 'X'; break;
                    case 4: map[i][j] = '@'; break;
                    case 5: map[i][j] = 'P'; break;
                    case 6: map[i][j] = 'G'; break;
                }
            }
        }
        
        /* Close the file once we are done reading from it. very important */
        fclose(file);
    }
    
    /* This is the single return point for the function. It will return
       the fully loaded map, or NULL if there was an error opening the file. */
    return map;
}