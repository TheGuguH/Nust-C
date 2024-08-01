#ifndef NUSTC_FRONT_END_LEXER_H
#define NUSTC_FRONT_END_LEXER_H

#include "token.h"

#include <stdio.h>

// It's not recommended to make a buffer with more than 16 KB for old devices compatibility
#define BUFFER_SIZE 4096
 
// Represents a Lexer object for... lexer phase ¯\_(ツ)_/¯
typedef struct Lexer {
    // Current file linked on this object
    FILE *_file;
    // File offset, aka file cursor
    size_t fileOffset;
    // Buffer offset, aka buffer cursor
    size_t bufferOffset;
    // Buffer with 4096B (4KB)
    char buffer[BUFFER_SIZE];
    // Buffer size
    // If the buffer size is less than 4096, it means we are in the last 4KB block of the file
    size_t buffer_s;
    // Current char
    char _char;
    // Memory pools
    /*
     Memory pool is a technique that optimizes dynamic memory allocation and deallocation 
         time by making a buffer for the desired object or several objects (a kind of array).
     It can also be used as a kind of custom malloc, realloc and free and is faster because it is allocated on the heap only once.
     Extremely useful for cases where heap memory allocation is recurrent and mainly for a specific use.
     It can be more memory safety
    */
    // Token memory pool
    Token *token;
    // String (token value) memory pool (string lenght is in Token memory Pool)
    char *strPool;
    // Current string pool size
    size_t strPool_s;
} Lexer;

// Create a lexer object
Lexer* lx_create(char *file);



// Free a lexer object
void lx_free(Lexer *_lexer);

#endif