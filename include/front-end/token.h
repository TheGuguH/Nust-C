#ifndef NUSTC_FRONT_END_TOKEN_H
#define NUSTC_FRONT_END_TOKEN_H

#include <stdio.h>

enum TokenType {
    // End of File
    TOKEN_EOF = -2,
    // Invalid (error)
    TOKEN_NULL,
    // Identifier (\d|_)?\w+
    TOKEN_IDENTIFIER,
    // Constants
    TOKEN_INT_NUMBER,
    TOKEN_FLOAT_NUMBER,
    TOKEN_CHAR_CONSTANT,
    TOKEN_STRING_CONSTANT,
    // Keywords
    //  Symbols
    TOKEN_LPARENTHESES,   //(
    TOKEN_RPARENTHESES,   //)
    TOKEN_LBRACKET,       //[
    TOKEN_RBRACKET,       //]
    TOKEN_LBRACE,         //{
    TOKEN_RBRACE,         //}
    TOKEN_COMMA,          //,
    TOKEN_DOT,            //.
    TOKEN_COLON,          //:
    //   Arithmetic
    TOKEN_ADD,
    TOKEN_SUBTRACT,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    //  Flow control
    TOKEN_GOTO,
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_DO,
    TOKEN_FOR,
    TOKEN_WHILE,
    TOKEN_CONTINUE,
    TOKEN_BREAK,
    TOKEN_RETURN,
    //  Types
    TOKEN_I8,
    TOKEN_I16,
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_F32,
    TOKEN_F64,
    TOKEN_BOOLEAN,
    TOKEN_CHAR,
    TOKEN_STRING,
};

typedef struct Token {
    enum TokenType type;
    char *value;
    size_t value_s;
    size_t line;
} Token;

/*
 * Value need to be dynamic maded
 */
Token* tk_create(size_t line);

Token* tk_create_op(enum TokenType type, char *value, size_t value_s, size_t line);

void tk_print(Token *token);

void tk_free(Token *token);

#endif