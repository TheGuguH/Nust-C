#include "front-end/lexer.h"

#include "error.h"

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

    // Open file and set offset to 0
    lx->_file = fopen(file, "r");
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

 1º: we reach the end of buffer
  In this case we need to treat some others cases:
   If the buffer is less than 4096, set lexer char to 'EOF'
   Else (the buffer size will be 4096) we read the next buffer, and ANOTHER other cases:
    If the lenght of the new buffer is more than 0 or ins't the EOF, we go normally
    Else we set lexer char to 'EOF'
 2º: if we not reach the end of buffer, we get the next char
 3º: if we reach the end of file, we set the char to 'EOF'
*/
void lx_skip(Lexer *lx) {

}
