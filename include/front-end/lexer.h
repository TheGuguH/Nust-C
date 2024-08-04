#ifndef NUSTC_FRONT_END_LEXER_H
#define NUSTC_FRONT_END_LEXER_H

#include "token.h"

#include <stdio.h>

// It's not recommended to make a buffer with more than 16 KB for old devices compatibility
#define BUFFER_SIZE 4096

// Standard string pool size
#define STR_POOL_DEFAULT_SIZE 32

// This multiplier is used for reallocing string pool size
// It's not recommended to realloc one by one byte, because some reasons:
//  Heap memory allocation, reallocation or freeing is't fast
//  Reallocing many times can couse memory fragmentation, causing errors or slowing down program flow
#define STR_POOL_MULTIPLIER 1.5
 
// Represents a Lexer object for... lexer phase ¯\_(ツ)_/¯
typedef struct Lexer {
    // Current file linked on this object
    FILE *_file;
    // Current line of file
    size_t line;
    // Current collum of line
    size_t collum;
    // File offset, aka file cursor
    size_t fileOffset;
    // Buffer offset, aka buffer cursor
    unsigned short bufferOffset;
    // Buffer with 4096B (4KB)
    // A buffer is used to read giant files fast, and without errors, making a more bug free software
    char buffer[BUFFER_SIZE];
    // Buffer size
    // (If the buffer size is less than 4096, it means we are in the last 4KB block of the file)
    unsigned short buffer_s;
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
    Token *_token;
    // String (token value) memory pool (string lenght is in Token memory Pool)
    char *strPool;
    // Current string pool size
    size_t strPool_s;
} Lexer;

// Heap manipulation

// Create a lexer object
Lexer* lx_create(char *file);

// Free a lexer object
void lx_free(Lexer *lx);

// Add char to string memory pool
void lx_addChar(Lexer *lx, char _char);

// Token related functions

// Reset the _token of Lexer *lx
void lx_tk_reset(Lexer *lx);

// Copy the current token to a new heap location/object
Token* lx_tk_copy(Lexer *lx);

// Utils

// Get the next char in lexer
void lx_skip(Lexer *lx);

// Get the next char non-space in the same line in lexer
void lx_skipSpace(Lexer *lx);

// Get the next char non-blank (i.e., not a space or invisible character) in lexer
void lx_skipBlank(Lexer *lx);

// Call the correct function for a token based on lexer char
Token* lx_getToken(Lexer *lx);

// Get the next token
Token* lx_getNext(Lexer *lx);

// Get the next token in the same line
Token* lx_getNextInLine(Lexer *lx);

// Token types identifier (no implementation)

// Get preprocessors (start with '#')
void lx_getPreProcessor(Lexer *lx);

// Get Assembler directives (start with '$')
void lx_getAssembly(Lexer *lx);

// Get numbers constants
void lx_getNumber(Lexer *lx);

// Get the identifier (can start with '_')
void lx_getIdentifier(Lexer *lx);

// Get the word (don't start with '_'), returning a keyword or a identifier
void lx_getWord(Lexer *lx);

// Get the symbol
void lx_getSymbol(Lexer *lx);

// Read the word and set to string pool
void lx_readWord(Lexer *lx);

// Identify if the string in string pool is a keyword or a identifer
enum TokenType lx_identifyKeyWord(Lexer *lx);

#endif