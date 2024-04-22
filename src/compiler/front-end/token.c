#include "headers/token.h"

#include "../../headers/error_codes.h"
#include "../../headers/utf8_compatibility.h"
#include "../../headers/utils.h"

#include <stdio.h>

Token* tk_create(int type, size_t line) {
    char *value = malloc(sizeof(char));
    value[0] = '\0';
    return tk_create_op(type, line, value, 1);
}

Token* tk_create_op(int type, size_t line, char value[], size_t value_s) {
    Token *_token = malloc(sizeof(Token));

    if (_token == NULL) {
        tPrintError("can't alloc memory for a token", TK_MEMORY_ALLOC_ERROR);
    }

    _token->type = type;
    _token->value = value;
    _token->value_s = value_s;
    _token->line = line;

    return _token;
}

void tk_free(Token *_token) {
    if (_token == NULL)
        return;

    free(_token->value);
    _token->value = NULL;

    free(_token);
    _token = NULL;
}

void tk_print(Token *_token) {
    printf("%lu:", _token->line);
    switch (_token->type) {
        case TOKEN_EOF:
            printf("<EOF>");
        break;
        case TOKEN_INVALID_TOKEN:
            printf("<INVALID>");
        break;
        case TOKEN_INT_NUMBER:
        case TOKEN_FLOAT_NUMBER:
        case TOKEN_IDENTIFIER:
            printf("<%d, ", _token->type);
            tk_printValue(_token);
            printf(", %lu>", _token->value_s);
        break;
        case TOKEN_CHAR_LITERAL:
            printf("<%d, '", _token->type);
            tk_printValue(_token);
            printf("', %lu>", _token->value_s);
        break;
        case TOKEN_STRING_LITERAL:
            printf("<%d, \"", _token->type);
            tk_printValue(_token);
            printf("\", %lu>", _token->value_s);
        break;
        default:
            printf("<%d>", _token->type);
        break;
    }
    puts("");
}

void tk_printValue(Token *_token) {
    if (_token->type == TOKEN_STRING_LITERAL || _token->type == TOKEN_CHAR_LITERAL) {
        size_t sequence_s = _token->value_s / 4;

        rune_t *rune = (rune_t*)_token->value;

        for (size_t i = 0; i < sequence_s; i++)
            rt_print(rune[i]);
            
    } else {
        for (size_t i = 0; i < _token->value_s; i++)
            printf("%c", _token->value[i]);
    }
}

char* tk_getValueOfAType(int type) {
    switch (type) {
        case TOKEN_INT_NUMBER:
            return "integer number";
        break;
        case TOKEN_FLOAT_NUMBER:
            return "float number";
        break;
        case TOKEN_STRING_LITERAL:
            return "string literal";
        break;
        case TOKEN_CHAR_LITERAL:
            return "char literal";
        break;
        case TOKEN_IDENTIFIER:
            return "identifier";
        break;
        case TOKEN_LEFT_PARENTHESE:
            return "(";
        break;
        case TOKEN_RIGHT_PARENTHESE:
            return ")";
        break;
        case TOKEN_LEFT_BRACKET:
            return "[";
        break;
        case TOKEN_RIGHT_BRACKET:
            return "]";
        break;
        case TOKEN_LEFT_BRACE:
            return "{";
        break;
        case TOKEN_RIGHT_BRACE:
            return "}";
        break;
        case TOKEN_DOT:
            return ".";
        break;
        case TOKEN_COMMA:
            return ",";
        break;
        case TOKEN_COLON:
            return ":";
        break;
        case TOKEN_DOUBLE_COLON:
            return "::";
        break;
        case TOKEN_SEMICOLON:
            return ";";
        break;
        case TOKEN_BACK_SLASH:
            return "\\";
        break;
        case TOKEN_ASSIGNMENT:
            return "=";
        break;
        case TOKEN_SLASH:
            return "/";
        break;
        case TOKEN_ASTERISK:
            return "*";
        break;
        case TOKEN_PLUS:
            return "+";
        break;
        case TOKEN_MINUS:
            return "-";
        break;
        case TOKEN_REST:
            return "%";
        break;
        case TOKEN_EQ_PLUS:
            return "+=";
        break;
        case TOKEN_EQ_MINUS:
            return "-=";
        break;
        case TOKEN_EQ_MULT:
            return "*=";
        break;
        case TOKEN_EQ_DIVISION:
            return "/=";
        break;
        case TOKEN_EQ_REST:
            return "%=";
        break;
        case TOKEN_INCREMENTATION:
            return "++";
        break;
        case TOKEN_DECREMENTATION:
            return "--";
        break;
        case TOKEN_LESS_THAN:
            return "<";
        break;
        case TOKEN_GREATER_THAN:
            return ">";
        break;
        case TOKEN_EQUALS:
            return "==";
        break;
        case TOKEN_NOT:
            return "!";
        break;
        case TOKEN_QUESTION_MARK:
            return "?";
        break;
        case TOKEN_LESS_OR_EQUALS:
            return "<=";
        break;
        case TOKEN_GREATER_OR_EQUALS:
            return ">=";
        break;
        case TOKEN_DIFFERENT:
            return "!=";
        break;
        case TOKEN_AND:
            return "&&";
        break;
        case TOKEN_OR:
            return "||";
        break;
        case TOKEN_BITW_AND:
            return "&";
        break;
        case TOKEN_BITW_OR:
            return "|";
        break;
        case TOKEN_BITW_NOT:
            return "~";
        break;
        case TOKEN_BITW_XOR:
            return "^";
        break;
        case TOKEN_BITW_LEFT_SHIFT:
            return "<<";
        break;
        case TOKEN_BITW_RIGHT_SHIFT:
            return ">>";
        break;
        case TOKEN_BITW_EQ_AND:
            return "&=";
        break;
        case TOKEN_BITW_EQ_OR:
            return "|=";
        break;
        case TOKEN_BITW_EQ_NOT:
            return "~=";
        break;
        case TOKEN_BITW_EQ_XOR:
            return "^=";
        break;
        case TOKEN_BITW_EQ_LF_SHIFT:
            return "<<=";
        break;
        case TOKEN_BITW_EQ_RT_SHIFT:
            return ">>=";
        break;
        case TOKEN_INT:
            return "int";
        break;
        case TOKEN_STRING:
            return "string";
        break;
        case TOKEN_NAMESPACE:
            return "namespace";
        break;
        case TOKEN_DEF:
            return "def";
        break;
        case TOKEN_RETURN:
            return "return";
        break;
        case TOKEN_IF:
            return "if";
        break;
        case TOKEN_ELSE:
            return "else";
        break;
        case TOKEN_ASSEMBLER_DIRECTIVE:
            return "$__ASSEMBLER_DIRECTIVE__";
        break;
    }

    exit(TOKEN_INVALID_TOKEN);
    return "invalid token";
}

void tPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Token]: ");
}
