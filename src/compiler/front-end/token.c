#include "headers/token.h"

#include "../../headers/error_codes.h"
#include "../../headers/utf8_compatibility.h"
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

void tk_print(Token *token) {
    printf("%lu:", token->line);
    switch (token->type) {
        case TOKEN_EOF:
            printf("<EOF>");
        break;
        case TOKEN_INVALID_TOKEN:
            printf("<INVALID>");
        break;
        case TOKEN_INT_NUMBER:
        case TOKEN_FLOAT_NUMBER:
        case TOKEN_IDENTIFIER:
            printf("<%d, ", token->type);
            tk_printValue(token);
            printf(", %lu>", token->value_s);
        break;
        case TOKEN_CHAR_LITERAL:
            printf("<%d, '", token->type);
            tk_printValue(token);
            printf("', %lu>", token->value_s);
        break;
        case TOKEN_STRING_LITERAL:
            printf("<%d, \"", token->type);
            tk_printValue(token);
            printf("\", %lu>", token->value_s);
        break;
        default:
            printf("<%d>", token->type);
        break;
    }
    puts("");
}

void tk_printValue(Token *token) {
    if (token->type == TOKEN_STRING_LITERAL || token->type == TOKEN_CHAR_LITERAL) {
        size_t sequence_s = token->value_s / 4;

        rune_t *rune = (rune_t*)token->value;

        for (size_t i = 0; i < sequence_s; i++)
            rt_print(rune[i]);
            
    } else {
        for (size_t i = 0; i < token->value_s; i++)
            printf("%c", token->value[i]);
    }
}

void tPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Token]: ");
}
