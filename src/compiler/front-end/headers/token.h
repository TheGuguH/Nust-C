#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

enum ETokenType {
    //Errors
    TOKEN_EOF = -2,
    TOKEN_INVALID_TOKEN,
    //Custom
    TOKEN_INT_NUMBER,              //nubers
    TOKEN_FLOAT_NUMBER,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_IDENTIFIER,          //names
    //Symbols
    TOKEN_LEFT_PARENTHESE,     //(
    TOKEN_RIGHT_PARENTHESE,    //)
    TOKEN_LEFT_BRACKET,        //[
    TOKEN_RIGHT_BRACKET,       //]
    TOKEN_LEFT_BRACE,          //{
    TOKEN_RIGHT_BRACE,         //}
    TOKEN_DOT,                 //.
    TOKEN_COMMA,               //,
    TOKEN_COLON,               //:
    TOKEN_DOUBLE_COLON,        //::
    TOKEN_SEMICOLON,           //;
    TOKEN_PLUS,                //+
    TOKEN_MINUS,               //-
    TOKEN_ASTERISK,            //*
    TOKEN_BACK_SLASH,          //'\'
    TOKEN_SLASH,               //'/'
    TOKEN_LESS_THAN,           //<
    TOKEN_GREATER_THAN,        //>
    TOKEN_EQUALS,              //=
    TOKEN_QUESTION_MARK,       //?
    //Types
    TOKEN_INT,                 //int
    TOKEN_STRING,              //string
    //Keywords
    TOKEN_NAMESPACE,           //namespace
    TOKEN_DEF,                 //def
    TOKEN_RETURN,              //return
    //For compiler
    TOKEN_ASSEMBLER_DIRECTIVE  //$__SOME_TEXT__
};

typedef struct {
    char *value;
    size_t value_s;
    size_t line;
    int type;
} Token;

Token* tk_create(int type, size_t line);

Token* tk_create_op(int type, size_t line, char value[], size_t value_s);

void tk_free(Token *token);

void tk_print(Token *token);

void tk_printValue(Token *token);

void tPrintError(char string[], int errorCode);

#endif