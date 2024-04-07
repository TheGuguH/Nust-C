#include "headers/error_codes.h"

#include <stdio.h>

int main(int argc, char *argv[]){

    if(argc < 2){
        printf("Sorry, but the compiler need a command, and some commands need arguments!\n");
        return NO_COMMAND;
    }

    return 0;
}