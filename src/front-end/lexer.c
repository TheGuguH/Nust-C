#include "front-end/lexer.h"

#include "error.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// Heap manipulation

Lexer* lx_create(char *file) {

    Lexer *lx = malloc(sizeof(Lexer));

    if (lx == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Lexer");

    // Making token memory pool
    lx->_token = malloc(sizeof(Token));

    if (lx->_token == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Token pool in Lexer");

    // Making token string memory pool (with 32 of defalt size)
    lx->strPool = malloc(STR_POOL_DEFAULT_SIZE * sizeof(char));

    if (lx->_token == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Token string pool in Lexer");

    // Open file and set offset and line to 0
    lx->_file = fopen(file, "r");
    lx->line = 0;
    lx->fileOffset = 0;

    // If file equals null, that means the file don't exists (or we can't access)
    if (lx->_file == NULL)
        err_printExit(ERROR_TRY_TO_USE_NULL, "The file \"%s\"don't exists or we can't access", file);

    // Read a maximum of 4096 bytes of file (the size of bytes readed will go in buffer_s variable)
    lx->buffer_s = fread(lx->buffer, 1, BUFFER_SIZE, lx->_file);

    // If the buffer_s is less than 1 (0) or we reach the EOF (end of file), it means the file is empty
    if (lx->buffer_s < 1 || feof(lx->_file))
        err_warn(WARN_EMPTY_FILE, "The file \"%s\" is empty", file);

    // Set the buffer offset to 0
    lx->bufferOffset = 0;

    return lx;
}

void lx_free(Lexer *lx) {

    // Freeing all heap memory
    free(lx->strPool);
    lx->strPool = NULL;

    free(lx->_token);
    lx->_token = NULL;

    fclose(lx->_file);
    lx->_file = NULL;

    free(lx);
    lx = NULL;
}

void lx_addChar(Lexer *lx, char _char) {

    // Realloc new size of string pool
    if ((lx->strPool_s - 1) == lx->_token->value_s)
        lx->strPool = realloc(lx->strPool, sizeof(char) * (lx->strPool_s * STR_POOL_MULTIPLIER));

    // Set the next char (lx->_token->value_s)
    lx->strPool[lx->_token->value_s] = _char;
    
    // Add one to lenght
    lx->_token->value_s++;
    
    // Set the next char to null character (i.e. string terminator)
    lx->strPool[lx->_token->value_s] = '\0';
}

// Utils

/*
 This functions has few possible cases:

 1º: if we reach the end of file, set the char to 'EOF' and return
 2º: we reach the end of buffer
  In this case we need to treat some others cases:
   We read the next buffer
   If the new buffer size is greater than 0 and we don't reach the eof, get the next char
   Else, set char to 'EOF' and return
 3º: if we not reach the end of buffer, get the next char

 EOF represents the end of file, a char type, so it can be:
  signed char: EOF = -1
  unsigned char: EOF = 255
 ALWAYS 0xff!
*/
void lx_skip(Lexer *lx) {

    // Add one to buffer and file offset (aka cursor)
    lx->fileOffset++;
    lx->bufferOffset++;

    // Every 'new line' char is a new line and a new collum!
    if (lx->_char == '\n') {

        lx->line++;
        lx->collum = 0;
    } else
        lx->collum++;

    // 1º case (eof)
    if (feof(lx->_file) || (lx->buffer_s < BUFFER_SIZE && lx->bufferOffset == lx->buffer_s)) {

        lx->_char = EOF;

        // return... we don't have more to work ¯\_(ツ)_/¯
        return;
    }

    // 2º case (end of buffer)
    if (lx->bufferOffset == lx->buffer_s) {

        lx->buffer_s = fread(lx->buffer, 1, BUFFER_SIZE, lx->_file);

        // 1º case (buffer is greater than 0 and we don't reach the end of file) of 2º (end of buffer)...
        if (lx->buffer_s > 0 && !feof(lx->_file)) {

            lx->bufferOffset = 0;
            lx->_char = lx->buffer[0];
        } else { // 2º case (we reach end of file or the buffer is less or equals than 0) of 2º (end of buffer)...

            lx->_char = EOF;

            // return... we don't have more to work ¯\_(ツ)_/¯
            return;
        }
    } else // 3º case (not end of buffer)
        lx->_char = lx->buffer[lx->bufferOffset];
}

void lx_skipBlank(Lexer *lx) {

    while (isgraph(lx->_char) == 0)
        lx_skip(lx);
}

void lx_skipBlankInLine(Lexer *lx) {

    while (lx->_char == ' ' || lx->_char == '\t' || lx->_char == '\v' || lx->_char == '\f')
        lx_skip(lx);
}
