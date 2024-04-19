#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>

enum ETokenType {
    //Errors
    TOKEN_EOF = -2,
    TOKEN_INVALID_TOKEN,
    //Custom
    TOKEN_INT_NUMBER,          //integer number
    TOKEN_FLOAT_NUMBER,        //float number
    TOKEN_STRING_LITERAL,      //string literals
    TOKEN_CHAR_LITERAL,        //char literals
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
    TOKEN_BACK_SLASH,          //'\'
    TOKEN_ASSIGNMENT,          //=
    TOKEN_SLASH,               //'/'
    TOKEN_ASTERISK,            //*
      //arithmetic
    TOKEN_PLUS,                //+
    TOKEN_MINUS,               //-
    TOKEN_REST,                //%
    TOKEN_EQ_PLUS,             //+=
    TOKEN_EQ_MINUS,            //-=
    TOKEN_EQ_MULT,             //*
    TOKEN_EQ_DIVISION,         //'/='
    TOKEN_EQ_REST,             //%=
    TOKEN_INCREMENTATION,      //++
    TOKEN_DECREMENTATION,      //--
      //condition
    TOKEN_LESS_THAN,           //<
    TOKEN_GREATER_THAN,        //>
    TOKEN_EQUALS,              //==
    TOKEN_NOT,                 //!
    TOKEN_QUESTION_MARK,       //?
    TOKEN_LESS_OR_EQUALS,      //<=
    TOKEN_GREATER_OR_EQUALS,   //>=
    TOKEN_DIFFERENT,           //!=
    TOKEN_AND,                 //&&
    TOKEN_OR,                  //||
      //bitwise
    TOKEN_BITW_AND,            //&
    TOKEN_BITW_OR,             //|
    TOKEN_BITW_NOT,            //~
    TOKEN_BITW_XOR,            //^
    TOKEN_BITW_LEFT_SHIFT,     //<<
    TOKEN_BITW_RIGHT_SHIFT,    //>>
    TOKEN_BITW_EQ_AND,         //&=
    TOKEN_BITW_EQ_OR,          //|=
    TOKEN_BITW_EQ_NOT,         //~=
    TOKEN_BITW_EQ_XOR,         //^=
    TOKEN_BITW_EQ_LF_SHIFT,    //<<=
    TOKEN_BITW_EQ_RT_SHIFT,    //>>=
    //Types
    TOKEN_INT,                 //int
    TOKEN_STRING,              //string
    //Keywords
    TOKEN_NAMESPACE,           //namespace
    TOKEN_DEF,                 //def
      //control
    TOKEN_RETURN,              //return
    TOKEN_IF,                  //if
    TOKEN_ELSE,                //else
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