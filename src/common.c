#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include "common.h"

#define DEFAULT_BUFFER_SIZE (16 * 4096)

inline int fill_next_buffer(char *buffer, int buffer_size, int retain,
        FILE *file) {
    /* Copy the bytes we have to retain. */
    memcpy(buffer, buffer + buffer_size - retain, retain);

    /* Read. */
    return retain + fread(buffer + retain, 1, buffer_size - retain, file);
}

inline char *create_buffer(int buffer_minimum_size, int *buffer_size) {
    int size = DEFAULT_BUFFER_SIZE;
    while(size < buffer_minimum_size) size <<= 2;
    *buffer_size = size;
#   ifdef DEBUG
    printf("Created a buffer of size: %d\n", size);
#   endif
    return malloc(size);
}

inline char *read_entire_file(const char *file_name, int *buffer_size) {
    FILE *file = fopen(file_name, "r");
    char *buffer;

    /* Error opening file */
    if(!file) {
        printf("Could not open: %s\n", file_name);
        return 0;
    }

    /* Determine the size of the file */
    fseek(file, 0, SEEK_END);
    *buffer_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    /* Put the file in the buffer. The '\0' termination is not strictly
     * necessary, but it helps if we want to print out the file contents. */
    buffer = malloc(*buffer_size + 1);
    fread(buffer, 1, *buffer_size, file);
    buffer[*buffer_size] = '\0';

    fclose(file);
    return buffer;
}

void print_match(const char *file_name, ullong position) {
    printf("%s:%lld\n", file_name, position);
}

char *to_binary(ullong n) {
    int i;
    ullong mask = 1;
    char *buffer = malloc((ULLONG_BITS + 1) * sizeof(char));

    /* Fill in the bytes */
    for(i = ULLONG_BITS - 1; i >= 0; i--) {
        buffer[ULLONG_BITS - 1 - i] = (n >> i) & mask ? '1' : '0';
    }

    /* Do not forget a trailing 0 byte */
    buffer[ULLONG_BITS] = '\0';
    return buffer;
}
