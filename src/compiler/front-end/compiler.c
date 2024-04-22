#include "headers/compiler.h"

#include "headers/lexer.h"
#include "headers/token.h"
#include "../../headers/error_codes.h"
#include "../../headers/utils.h"

#include <stdio.h>
#include <string.h>

void startCompiling(size_t file_c, char *file_v[], CompilerContext context) {
    if (file_c < 1) {
        cPrintError("don't have enough files", CMP_NO_INPUT_FILES);
    }

    NCTFile files[file_c];

    for (size_t i = 0; i < file_c; i++) {
        if (STR_N_EQUALS(file_v[i], context.output, strlen(context.output) + 1)) {
            cPrintError("a input file is same as output", CMP_SAME_AS_OUTPUT);
        }

        if (isSourceFile(file_v[i])) {
            files[i].file = fopen(file_v[i], "r");
            files[i].type = FILE_SOURCE;
            files[i].name = file_v[i];

        } else if (isNLIBFile(file_v[i])) {
            files[i].file = fopen(file_v[i], "r");
            files[i].type = FILE_NLIB;
            files[i].name = file_v[i];

        } else if (isNIRFile(file_v[i])) {
            files[i].file = fopen(file_v[i], "r");
            files[i].type = FILE_NIR;
            files[i].name = file_v[i];

            cPrint("nir files don't link with other archives!");

        } else {
            cPrintError("invalid input file", CMP_INVALID_FILE_FOR_THIS);
        }

        if (files[i].file == NULL) {
            cPrintError("a input file don't exists", CMP_FILE_DONT_EXISTS);
        }

    }

    Lexer *lexer = lx_create(files[0].file);
    Token *tk;
    printf("<%lu>\n", lexer->buffer_s);

    while ((tk = lx_getNextToken(lexer))->type != TOKEN_EOF && tk->type != TOKEN_INVALID_TOKEN) {
        tk_print(tk);
        tk_free(tk);
    }

    tk_free(tk);
    
    lx_free(lexer);
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

void cPrint(char string[]) {
    printf("[Compiler]: %s\n", string);
}

void cPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Compiler]: ");
}
