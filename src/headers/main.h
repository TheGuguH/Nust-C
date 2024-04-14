#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>

#define CMP_INVALID_FILE_ASCII printError("Sorry, but the files need to have only ASCII characters", CMP_INVALID_FILE_FOR_THIS);

void compileFiles(int arg_c, char *arg_v[]);

void showHelp(int arg_c, char *arg_v[]);

void compileFilesToNIR(int arg_c, char *arg_v[]);

int isASCIIString(char string[], size_t string_s);

void isACompilableArchive(char file[]);

int convertToAssemblyEnum(char assembly[]);

int convertToOSEnum(char os[]);

#endif