#include "headers/token.h"

#include "../../headers/error_codes.h"
#include "../../headers/utils.h"

#include <stdio.h>

Token* tk_create(int type) {
    return tk_create_op(type, "", 1);
}

Token* tk_create_op(int type, char value[], size_t value_s) {
    Token *token = malloc(sizeof(Token));

    if (token == NULL) {
        tPrintError("error on memory allocation for a token", TK_MEMORY_ALLOC_ERROR);
    }

    token->type = type;
    token->value = value;
    token->value_s = value_s;

    return token;
}

void tk_free(Token *token) {
    free(token->value);
    token->value = NULL;

    free(token);
    token = NULL;
}

void tPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Token]: ");
}
