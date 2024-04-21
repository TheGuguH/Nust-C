#ifndef SYNTAXER_H
#define SYNTAXER_H

#include "lexer.h"
#include "symbol_table.h"

#include <stdio.h>

typedef struct {
    char *fileName;
    size_t fileName_s;
    Lexer *_lexer;
    SymbolTable *_symbolTable;
} Syntaxer;

Syntaxer* sx_create(char *fileName, size_t fileName_s, FILE *_file, SymbolTable *_symbolTable);

void sx_free(Syntaxer *_syntaxer);

void sx_start(SymbolTable *_syntaxer);

#endif