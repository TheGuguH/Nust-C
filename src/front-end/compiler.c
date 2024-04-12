#include "headers/compiler.h"

#include "../headers/error_codes.h"
#include "../headers/utils.h"

#include <stdio.h>

void startCompiling(int filec, FILE *filev[], CompilerContext context) {
    if (filec < 1) {
        cPrintError("don't have enough files", CMP_NO_INPUT_FILES);
    }


}

void cPrint(char _s[]) {
    printf("[Compiler]: %s\n", _s);
}

void cPrintError(char _s[], int errorCode) {
    P_ERROR_FORMAT("[Compiler]: ");
}
