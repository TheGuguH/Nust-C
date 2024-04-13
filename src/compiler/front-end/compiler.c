#include "headers/compiler.h"

#include "../../headers/error_codes.h"
#include "../../headers/utils.h"

#include <stdio.h>
#include <string.h>

void startCompiling(size_t filec, char *filev[], CompilerContext context) {
    if (filec < 1) {
        cPrintError("don't have enough files", CMP_NO_INPUT_FILES);
    }

    NCTFile *files[filec];

    for (size_t i = 0; i < filec; i++) {
        // if (isCompilable(filev[i])) {
        //     if (isNIRFile(filev[i]) && filec > 1) {
        //         cPrint("nir files don't link with other archives!");
        //     }
        //     continue;
        // }

        if (isSourceFile(filev[i])) {
            files[i]->file = fopen(filev[i], "r");
            files[i]->type = FILE_SOURCE;

            if (files[i] == NULL) {
                cPrintError("one or more passed input files don't exists", CMP_FILE_DONT_EXISTS);
            }

            continue;
        }

        cPrintError("invalid input file", CMP_INVALID_FILE_FOR_THIS);
    }

    
}

int isSourceFile(char file[]) {

    char *extension = strrchr(file, '.');

    if (extension != NULL && (STR_N_EQUALS(extension, ".nc", 4) || STR_N_EQUALS(extension, ".nustc", 7) || 
        STR_N_EQUALS(extension, ".nust", 6)))
        return 1;

    return 0;
}

int isNLIBFile(char file[]) {

    char *extension = strrchr(file, '.');

    if (extension != NULL && (STR_N_EQUALS(extension, ".nlib", 6)))
        return 1;

    return 0;
}

int isNIRFile(char file[]) {

    char *extension = strrchr(file, '.');

    if (extension != NULL && (STR_N_EQUALS(extension, ".nir", 5)))
        return 1;

    return 0;
}

int isCompilable(char file[]) {
    return isSourceFile(file) || isNLIBFile(file) || isNIRFile(file);
}

void cPrint(char _s[]) {
    printf("[Compiler]: %s\n", _s);
}

void cPrintError(char _s[], int errorCode) {
    P_ERROR_FORMAT("[Compiler]: ");
}
