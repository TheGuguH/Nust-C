#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>

enum {
    ASSEMBLY_X86_64,
    ASSEMBLY_X86,
    ASSEMBLY_ARM_V8,
    ASSEMBLY_ARM_V7,
};

enum {
    OS_WINDOWS,
    OS_LINUX,
};

enum {
    FILE_SOURCE,
    FILE_NLIB,
    FILE_NIR,
};

typedef unsigned char bool_t;

typedef struct {
    bool_t showExtraInfo;
    bool_t createLogFile;
    bool_t makeExecutable;
    char *output;
    int assemblyType;
    int os;
    int prioritizeGlobal;
    int stdlib;
    int globalLib;
    char **sharedLibs;
    size_t sharedLibsQuantity;
} CompilerContext;

typedef struct {
    FILE *file;
    int type;
    char *name;
} NCTFile;

#ifdef WIN32
    #define MACHINE_OS OS_WINDOWS
#elif linux || __linux__
    #define MACHINE_OS OS_LINUX
#endif

#if defined(__x86_64__) || defined(_M_X64)
    #define MACHINE_ASSEMBLY ASSEMBLY_X86_64
#elif defined(__i386__) || defined(_M_IX86)
    #define MACHINE_ASSEMBLY ASSEMBLY_x86
#elif defined(__arm__)
    #ifdef __ARM_ARCH_8__
        #define MACHINE_ASSEMBLY ASSEMBLY_ARM_V8
    #elif
        #define MACHINE_ASSEMBLY ASSEMBLY_ARM_V7
    #endif
#endif

void startCompiling(size_t file_c, char *file_v[], CompilerContext context);

int isSourceFile(char file[]);

int isNLIBFile(char file[]);

int isNIRFile(char file[]);

int isCompilable(char file[]);

void cPrint(char string[]);

void cPrintError(char string[], int errorCode);

#endif
