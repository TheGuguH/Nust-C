#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include "../../../headers/utf8_compatibility.h"

#include <stdio.h>

typedef struct {
    char buffer[1024];
    char _char;
    size_t bufferCursor;
    size_t fileCursor;
    size_t buffer_s;
    size_t line;
    FILE *file;
} Lexer;

#define BUFFER_SIZE 1024

Lexer* lx_create(FILE *file);

void lx_free(Lexer *_lexer);

int lx_skip(Lexer *_lexer);

int lx_skipEmpty(Lexer *_lexer);

int lx_skipBlank(Lexer *_lexer);

void lx_skipComment(Lexer *_lexer);

char lx_peek(Lexer *_lexer);

Token* lx_getNextToken(Lexer *_lexer);

Token* lx_getNextTokenInSameLine(Lexer *_lexer);

Token* lx_identifyToken(Lexer *_lexer);

Token* lx_getDigit(Lexer *_lexer);

Token* lx_getIdentifier(Lexer *_lexer);

Token* lx_getChar(Lexer *_lexer);

Token* lx_getString(Lexer *_lexer);

Token* lx_getCompilerDirective(Lexer *_lexer);

Token* lx_getAssemblerDirective(Lexer *_lexer);

Token* lx_getSymbol(Lexer *_lexer);

rune_t lx_convertScapeSequence(Lexer *_lexer);

int lx_getKeywordType(char identifier[], size_t identifier_s);

//The array size need to be 8

uint32_t hexaToRune(char _chars[], size_t _chars_s, size_t line);

//The array size need to be 2

uint8_t hexaToValue(char _chars[], size_t _chars_s, size_t line);

void lPrint(char string[], size_t line);

void lPrintError(char string[], int errorCode, size_t line);

#endif