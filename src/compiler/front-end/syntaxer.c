#include "headers/syntaxer.h"

#include "headers/lexer.h"
#include "headers/symbol_table.h"

#include <stdio.h>

Syntaxer* sx_create(char *fileName, size_t fileName_s, FILE *file, SymbolTable *_symbolTable) {
    Syntaxer *_syntaxer = malloc(sizeof(Syntaxer));

    _syntaxer->fileName = fileName;
    _syntaxer->fileName_s = fileName_s;
    _syntaxer->_lexer = lx_create(file);
    _syntaxer->_symbolTable = _symbolTable;

    return _syntaxer;
}

void sx_free(Syntaxer *_syntaxer) {
    lx_free(_syntaxer->_lexer);
    _syntaxer->_lexer = NULL;

    st_free(_syntaxer->_symbolTable);
    _syntaxer->_symbolTable = NULL;

    free(_syntaxer);
    _syntaxer = NULL;
}
