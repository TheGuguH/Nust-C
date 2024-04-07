#include "headers/main.h"
#include "headers/error_codes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Sorry, but the compiler need a command, and some commands need arguments!\n");
        return NO_COMMAND;
    }

    if(strncmp(argv[1], "-c", 3)){

    }
    else if(strncmp(argv[1], "-help", 6)){

    }

    return 0;
}

void compileFiles(int argc, char **argv){
    if(argc < 3){
        printf("Sorry, but the compiler need input files!");
        exit(NO_ARGUMENTS);
    }
}

void showHelp(int argc, char *argv[]){
    if(argc < 3){
        printf("Help menu: \n"
        "-c -> compile input files into executables\n"
        "    -asm asm -> compile to 'asm' assembly, use -help asm to show available assemblies");
    }
}