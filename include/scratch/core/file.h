/*

  This file is a part of the Scratch to C
  project and distributed under the MIT license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/scratch2c

*/


#ifndef _SCRATCH_FILE_H
#define _SCRATCH_FILE_H

#include "scratch/_scratch.h"


/**
 * @brief Read file content.
 * 
 * This function returns allocated memory, you are responsible to free it.
 * 
 * @param filename Filename
 * @return char * 
 */
char *sc_read(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open file.");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(filesize + 1); // +1 for null terminator
    if (!buffer) {
        fprintf(stderr, "Failed to allocate memory.");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t read_size = fread(buffer, 1, filesize, file);
    if (read_size != filesize) {
        fprintf(stderr, "Failed to read file.");
        free(buffer);
        fclose(file);
        exit(EXIT_FAILURE);
    }

    buffer[filesize] = '\0';
    fclose(file);
    return buffer;
}


#endif