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

Token* sx_assert(Syntaxer *_syntaxer, int type);

void sx_getNextAST(Syntaxer *_syntaxer);

void sx_getAttribute(Syntaxer *_syntaxer);

void syPrint(char string[]);

void syPrintError(char string[], int errorCode);

#endif