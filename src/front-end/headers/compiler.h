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

typedef unsigned char bool_t;

typedef struct {
    bool_t showExtraInfo;
    bool_t createLogFile;
    bool_t makeExecutable;
    char *finalName;
    int assemblyType;
    int os;
} CompilerContext;

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

void startCompiling(int filec, FILE *filev[], CompilerContext context);

void cPrint(char _s[]);

void cPrintError(char _s[], int errorCode);

#endif