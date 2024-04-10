
#include "headers/utf-8_algorithm_test.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define CHAR_SEQUENCE_LENGHT 3

int main() {

    unsigned char s[] = "\xF6\xBF\xBF\xBF";

    RuneString *string = convertToRuneString((unsigned char*)s, CHAR_SEQUENCE_LENGHT);

    for (size_t i = 0; i < string->runeQuantity; i++) {
        rt_print(string->runeSequence[i]);
    }

    printf("\n\n");

    free(string->runeSequence);
    free(string);
    return 0;
}