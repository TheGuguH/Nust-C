#include "headers/symbol_table.h"

#include "headers/namespace.h"

#include <stdlib.h>

SymbolTable* st_create() {
    SymbolTable *_symbolTable = malloc(sizeof(SymbolTable));

    _symbolTable->namespace_c = 0;
    _symbolTable->namespace_v = NULL;

    return _symbolTable;
}

void st_free(SymbolTable *_symbolTable) {
    ns_free(_symbolTable->namespace_v);
    _symbolTable->namespace_v = NULL;

    free(_symbolTable);
    _symbolTable = NULL;
}
