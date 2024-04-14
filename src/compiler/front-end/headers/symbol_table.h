#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>

typedef struct {

} Symbol;

typedef struct {

} Function;

typedef struct {

} SubRotine;

typedef struct {

} Procedure;

typedef struct {

} Macro;


typedef struct {

} Variable;

typedef struct {

} Typedef;

typedef struct {

} Struct;

typedef struct {

} Unit;

typedef struct {

} Module;

typedef struct {

} Const;

typedef struct {
    char *name;
    size_t name_s;
    char *_namespace;
    size_t namespace_s;
    Function _functions;
    SubRotine _subRotines;
    Procedure _procedures;
    Macro _macros;
    Variable _variables;
    Typedef _typedefs;
    Struct _structs;
    Unit _units;
    Module _modules;
    Const _consts;
} NamespaceNode;

typedef struct {
    size_t namespace_c;
    NamespaceNode *namespace_v;
} SymbolTable;

#endif