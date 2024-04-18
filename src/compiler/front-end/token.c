#include "headers/token.h"

#include "../../headers/error_codes.h"
#include "../../headers/utils.h"

#include <stdio.h>

Token* tk_create(int type, size_t line) {
    return tk_create_op(type, line, "", 1);
}

Token* tk_create_op(int type, size_t line, char value[], size_t value_s) {
    Token *token = malloc(sizeof(Token));

    if (token == NULL) {
        tPrintError("can't alloc memory for a token", TK_MEMORY_ALLOC_ERROR);
    }

    token->type = type;
    token->value = value;
    token->value_s = value_s;
    token->line = line;

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
