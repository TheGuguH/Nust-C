#ifndef MAIN_H
#define MAIN_H

#include <stdlib.h>

#define CMP_INVALID_FILE_ASCII printError("Sorry, but the files need to have only ASCII characters", CMP_INVALID_FILE_FOR_THIS);

void compileFiles(int argc, char *argv[]);

void showHelp(int argc, char *argv[]);

void compileFilesToNIR(int argc, char *argv[]);

int isASCIIString(char _s[], size_t _sSize);

void isACompilableArchive(char file[]);

int convertToAssemblyEnum(char assembly[]);

#endif