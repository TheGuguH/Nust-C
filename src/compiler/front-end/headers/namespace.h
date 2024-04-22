#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <stdio.h>

typedef struct {

} Const;

typedef struct {

} Macro;

typedef struct {

} Variable;

typedef struct {

} Struct;

typedef struct {

} Unit;

typedef struct {

} Module;

typedef struct {

} Function;

typedef struct {
    char *name;
    size_t name_s;
    Const *_consts;
    size_t _consts_s;
    Macro *_macros;
    size_t _macros_s;
    Variable *_variables;
    size_t _variables_s;
    Struct *_structs;
    size_t _structs_s;
    Unit *_units;
    size_t _units_s;
    Module *_modules;
    size_t _modules_s;
    Function *_functions;
    size_t _functions_s;
} Namespace;

Namespace* ns_create(char name[], size_t name_s);

void ns_free(Namespace *_namespace);

void ns_addGlobal();

#endif