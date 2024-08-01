#include "error.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void err_print(const enum EErrorCode code, const char *error, ...) {
    va_list args;

    va_start(args, error); 

    fprintf(stderr, "Error: ");
    vfprintf(stderr, error, args);
    fprintf(stderr, "\nCode: %d\n", code);
}

void err_warn(const enum EWarnCode code, const char *warn, ...) {
    va_list args;

    va_start(args, warn);

    fprintf(stderr, "Warn: ");
    vfprintf(stderr, warn, args);
    fprintf(stderr, "\nCode: %d\n", code);

    va_end(args);
}

void err_printExit(const enum EErrorCode code, const char *error, ...) {

    va_list args;

    va_start(args, error);

    err_print(code, error, args);
    exit(code);
}
