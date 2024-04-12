#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

/*
    Print a non-formated text and stop execution program, showing the code error.
*/

void printError(char _s[], int errorCode);

#define FALSE 0
#define TRUE 1

/*
    Shotcut for defining an formated error message, stop execution of program and showing the code error
*/

#define P_ERROR_FORMAT(prefix) printf(prefix "%s\nError code: %d\n", _s, errorCode); \
    exit(errorCode); \

#define STR_N_EQUALS(_s, comparating, comparatingSize) strncmp(_s, comparating, comparatingSize) == FALSE

#endif