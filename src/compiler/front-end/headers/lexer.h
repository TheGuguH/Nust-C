#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <stdio.h>

typedef struct {
    char buffer[1024];
    char _char;
    size_t bufferCursor;
    size_t fileCursor;
    size_t buffer_s;
    FILE *file;
} Lexer;

#define BUFFER_SIZE 1024

Lexer* lx_create(FILE *file);

int lx_skip(Lexer *lexer);

int lx_skipEmpty(Lexer *lexer);

int lx_skipBlank(Lexer *lexer);

char lx_peek(Lexer *lexer);

Token* lx_getNextToken(Lexer *lexer);

Token* lx_getString(Lexer *lexer);

Token* lx_getChar(Lexer *lexer);

Token* lx_getIdentifier(Lexer *lexer);

Token* lx_getCompilerDirective(Lexer *lexer);

Token* lx_getAssemblerDirective(Lexer *lexer);

void lPrint(char string[]);

void lPrintError(char string[], int errorCode);

#endif