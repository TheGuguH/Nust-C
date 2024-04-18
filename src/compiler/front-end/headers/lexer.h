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

#define LX_SKIPED if (lx_skip(lexer) == 0) \
            lPrintError("early EOF", LX_EARLY_EOF); \

#define LX_SKIPED_OP(text) if (lx_skip(lexer) == 0) \
            lPrintError("early EOF, " #text, LX_EARLY_EOF); \

#define LX_GET_BASE(sequence, token, verify) char *sequence = calloc(1, sizeof(char)); \
    size_t sequence##_s = 1; \
 \
    sequence[0] = lexer->_char; \
 \
    LX_SKIPED; \
 \
    while (verify) { \
        sequence##_s++; \
 \
        sequence = realloc(sequence, sequence##_s); \
 \
        sequence[sequence##_s - 1] = lexer->_char; \
 \
        LX_SKIPED; \
    } \
 \
    return tk_create_op(token, lexer->line, sequence, sequence##_s); \

Lexer* lx_create(FILE *file);

void lx_free(Lexer *lexer);

int lx_skip(Lexer *lexer);

int lx_skipEmpty(Lexer *lexer);

int lx_skipBlank(Lexer *lexer);

void lx_skipComment(Lexer *lexer);

char lx_peek(Lexer *lexer);

Token* lx_getNextToken(Lexer *lexer);

Token* lx_getDigit(Lexer *lexer);

Token* lx_getIdentifier(Lexer *lexer);

Token* lx_getChar(Lexer *lexer);

Token* lx_getString(Lexer *lexer);

Token* lx_getCompilerDirective(Lexer *lexer);

Token* lx_getAssemblerDirective(Lexer *lexer);

Token* lx_getSymbol(Lexer *lexer);

rune_t lx_convertScapeSequence(Lexer *lexer);

int lx_getKeywordType(Lexer *lexer, char identifier[], size_t identifier_s);

//The array size need to be 2

uint8_t charToHexa(char _chars[], size_t _chars_s);

void lPrint(char string[]);

void lPrintError(char string[], int errorCode);

#endif