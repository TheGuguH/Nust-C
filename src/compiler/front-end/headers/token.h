#ifndef TOKEN_H
#define TOKEN_H

enum ETokenType {
    //Custom
    TOKEN_NUMBER,              //nubers
    TOKEN_IDENTIFIER,          //names
    //Symbols
    TOKEN_LEFT_PARENTHESE,     //(
    TOKEN_RIGHT_PARENTHESE,    //)
    TOKEN_LEFT_BRACKET,        //[
    TOKEN_RIGHT_BRACKET,       //]
    TOKEN_LEFT_BRACE,          //{
    TOKEN_RIGHT_BRACE,         //}
    TOKEN_COLON,               //:
    TOKEN_DOBLE_COLON,         //::
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
    int type;
} Token;

Token* tk_create(int type, char value[]);

void tPrintError(char string[], int errorCode);

#endif