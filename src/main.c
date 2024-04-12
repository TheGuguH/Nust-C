#include "headers/main.h"

#include "headers/error_codes.h"
#include "front-end/headers/compiler.h"
#include "headers/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printError("Sorry, but the compiler need a command, and some commands need arguments", NO_COMMAND);
    }

    if (STR_N_EQUALS(argv[1], "-c", 3) || STR_N_EQUALS(argv[1], "-cmp", 5)) {
        compileFiles(argc, argv);
    } else if (STR_N_EQUALS(argv[1], "-help", 6)) {
        showHelp(argc, argv);
    } else if (STR_N_EQUALS(argv[1], "-nir",  5)) {
        compileFilesToNIR(argc, argv);
    } else {
        puts("Sorry, but the passed command ins't a valid command, use -help command");
    }

    return 0;
}

void compileFiles(int argc, char **argv){
    if (argc < 4) {
        printError("Sorry, but the compiler need the output file, and the input files", CMP_NO_ARGUMENTS);
    }

    if (!isASCIIString(argv[2], strlen(argv[2]))) {
        CMP_INVALID_FILE_ASCII;
    }

    char files[1];
    CompilerContext context = {FALSE, FALSE, TRUE, argv[2], MACHINE_ASSEMBLY, MACHINE_OS};

    for (int i = 3; i < argc; i++) {
        if (STR_N_EQUALS(argv[i], "-asm", 5)) {
            if (i + 1 >= argc) {
                printError("Sorry, but you need to specify a assembly of argument '-asm'", CMP_INVALID_ASM_ARGUMENT_SYNTAX);
            }

            context.assemblyType = convertToAssemblyEnum(argv[i + 1]);
            i++;
        } else if (STR_N_EQUALS(argv[i], "-extra", 7)) {
            context.showExtraInfo = TRUE;
        } else if (STR_N_EQUALS(argv[i], "-logfile", 9)) {
            context.createLogFile = TRUE;
        } else if (STR_N_EQUALS(argv[i], "-asmfile", 9)) {
            context.makeExecutable = FALSE;
        }
    }

    startCompiling(0, NULL, context);
}

void showHelp(int argc, char *argv[]){
    if (argc < 3) {
        puts("Help menu: \n"
        "-c or -cmp OUTPUT INPUTS -> compile input files into executables\n"
        "    -asm asm -> compile to 'asm' assembly, use -help asm to show available assemblies");
    }
}

void compileFilesToNIR(int argc, char **argv) {
    if (argc < 3) {
        printError("Sorry, but the compiler need input files", NO_ARGUMENTS);
    }
}

int isASCIIString(char _s[], size_t _sSize) {
    for (size_t i = 0; i < _sSize; i++) {
        if (_s[i] < 0)
            return 0;
    }

    return 1;
}

int convertToAssemblyEnum(char assembly[]) {
    if (STR_N_EQUALS(assembly, "-x86_64", 8)) {
        return ASSEMBLY_X86_64;
    } else if (STR_N_EQUALS(assembly, "-x86", 5)) {
        return ASSEMBLY_X86;
    } else if (STR_N_EQUALS(assembly, "-arm_v8", 8)) {
        return ASSEMBLY_ARM_V8;
    } else if (STR_N_EQUALS(assembly, "-arm_v7", 8)) {
        return ASSEMBLY_ARM_V7;
    } else {
        printError("Sorry, but the assembly passed ins't a assembly, use -help -asm to show disponible assemblys", INVALID_ASSEMBLY);
    }
    return -1;
}
