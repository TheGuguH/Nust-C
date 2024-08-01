#include "front-end/lexer.h"

#include "error.h"

#include <stdio.h>
#include <stdlib.h>

Lexer* lx_create(char *file) {

    Lexer *lx = malloc(sizeof(Lexer));

    if (lx == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Lexer");

    lx->_file = fopen(file, "r");
    lx->fileOffset = 0;

    if (lx->_file == NULL)
        err_printExit(ERROR_TRY_TO_USE_NULL, "File don't exists");

    lx->buffer_s = fread(lx->buffer, 1, BUFFER_SIZE, lx->_file);

    if (lx->buffer_s < 1 || feof(lx->_file))
        err_warn(WARN_EMPTY_FILE, "Empty file");

    lx->bufferOffset = 0;

    return lx;
}
