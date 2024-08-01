#include "main.h"

#include "error.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc < 4) 
        err_printExit(ERROR_NO_ENOUGH_ARGUMENTS, "Not enough arguments were given\n"
            "use: nustc <asm output> <input's>");

    genAssembly(argc, argv);

    return 0;
}

void genAssembly(int argc, char *argv[]) {

    for (int i = 2; i < argc; i++) {

        if (!fileExists(argv[i]))
            err_printExit(ERROR_INVALID_ARGUMENTS, "Error: The file '%s' don't exists", argv[i]);

        if (!isNustCFile(argv[i]))
            err_printExit(ERROR_INVALID_ARGUMENTS, "File '%s' doesn't appear to be a Nust C file", argv[i]);
    }    
}

int fileExists(char *path) {

    FILE *file = fopen(path, "r");

    if (file != NULL) {
        
        fclose(file);
        return 1;
    }

    return 0;
}

int isNustCFile(char *path) {

    char *extension = strrchr(path, '.');

    if (extension == NULL)
        return 0;

    return STR_EQUALS(extension, ".nc", 3) 
        || STR_EQUALS(extension, ".nust", 5)
        || STR_EQUALS(extension, ".nustc", 6);
}
