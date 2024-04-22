#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "namespace.h"
#include "token.h"

#include <stdio.h>

typedef struct {
    int id;
    char *name;
    size_t name_s;
} Type;

typedef struct {
    size_t namespace_c;
    Namespace *namespace_v;
} SymbolTable;

SymbolTable* st_create();

void st_free(SymbolTable *_symbolTable);

void st_addNamespace(SymbolTable *_symbolTable, Namespace *_namespace);

#endif
