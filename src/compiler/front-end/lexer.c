#include "headers/lexer.h"

#include "../../headers/error_codes.h"
#include "../../headers/utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Lexer* lx_create(FILE *file) {
    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer == NULL) {
        lPrintError("can't alloc memory for a lexer", LX_MEMORY_ALLOC_ERROR);
    }

    lexer->file = file;
    lexer->fileCursor = 0;
    lexer->bufferCursor = 0;

    lexer->buffer_s = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);

    if (lexer->buffer_s <= 0) {
        lPrintError("a totally empty file founded", LX_EMPTY_FILE_PASSED);
    }

    lexer->_char = lexer->buffer[0];

    return lexer;
}

int lx_skip(Lexer *lexer) {
    lexer->fileCursor++;
    lexer->bufferCursor++;

    if (lexer->bufferCursor == lexer->buffer_s) {
        lexer->buffer_s = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);

        lexer->bufferCursor = 0;

        lexer->_char = lexer->buffer[0];
    } else {
        lexer->_char = lexer->buffer[lexer->bufferCursor];
    }

    if (feof(lexer->file))
        return 0;

    return 1;
}

int lx_skipEmpty(Lexer *lexer) {
    while (isspace(lexer->_char))
        if (lx_skip(lexer) == 0) 
            return 0;

    return 1;
}

int lx_skipBlank(Lexer *lexer) {
    while (isblank(lexer->_char))
        if (lx_skip(lexer) == 0)
            return 0;

    return 1;
}

char lx_peek(Lexer *lexer) {
    if (lexer->bufferCursor + 1 < lexer->buffer_s) {
        return lexer->buffer[lexer->bufferCursor + 1];

    } else if (feof(lexer->file)) {
        return EOF;

    }

    size_t oldCursor = lexer->fileCursor;

    char nextChar;

    fseek(lexer->file, 1, SEEK_CUR);

    fread(&nextChar, 1, 1, lexer->file);

    fseek(lexer->file, oldCursor, SEEK_SET);

    return nextChar;
}

void lPrint(char string[]) {
    printf("[Lexer]: %s\n", string);
}

void lPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Lexer]: ");
}
