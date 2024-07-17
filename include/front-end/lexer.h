#ifndef NUSTC_FRONT_END_LEXER_H
#define NUSTC_FRONT_END_LEXER_H

#include <stdio.h>

#define BUFFER_SIZE 4096

typedef struct Lexer {
    FILE *_file;
    size_t fileOffset;
    size_t bufferOffset;
    char buffer[BUFFER_SIZE];
    size_t buffer_s;
    char _char;
} Lexer;

Lexer* lx_create();



void lx_free(Lexer *_lexer);

#endif