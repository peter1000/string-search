#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

/**
 * Keep retain bytes in the buffer, move them to the front and fill the
 * remainder with bytes read from the file.
 */
int fill_next_buffer(char *buffer, int buffer_size, int retain, FILE *file);

/**
 * Create a new buffer with a nice size. buffer_size will contain the resulting
 * size.
 */
char *create_buffer(int buffer_minimum_size, int *buffer_size);

/**
 * Read an entire file into a newly created buffer. buffer_size will contain the
 * size of the resulting buffer
 */
char *read_entire_file(const char *file_name, int *buffer_size);

#endif
