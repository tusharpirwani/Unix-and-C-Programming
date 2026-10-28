#ifndef FILE_IO_H
#define FILE_IO_H

/*
 * Reads a map from a text file and creates a dynamically allocated 2D char array.
 * PARAM: filename, the name of the map file to read.
 * PARAM: outRows, an output parameter to store the number of rows in the map.
 * PARAM: outCols, an output parameter to store the number of columns in the map.
 * RETURN: A pointer to the 2D map array, or NULL if the file could not be opened.
 */
char** readMapFile(char* filename, int* outRows, int* outCols);

#endif