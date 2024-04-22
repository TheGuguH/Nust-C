#include "headers/namespace.h"

#include <stdio.h>
#include <stdlib.h>

Namespace* ns_create(char name[], size_t name_s) {
    Namespace *_namespace = malloc(sizeof(Namespace));

    _namespace->name = name;
    _namespace->name_s = name_s;

    return _namespace;
}

void ns_free(Namespace *_namespace) {
    free(_namespace);
    _namespace = NULL;
}
