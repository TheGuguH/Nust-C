#include "headers/lexer.h"

#include "headers/token.h"
#include "../../headers/error_codes.h"
#include "../../headers/utf8_compatibility.h"
#include "../../headers/utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Lexer* lx_create(FILE *file) {
    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer == NULL) {
        lPrintError("can't alloc memory for a lexer", LX_MEMORY_ALLOC_ERROR);
    }

    lexer->file = file;
    lexer->fileCursor = 0;
    lexer->bufferCursor = 0;

    lexer->buffer_s = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);

    if (lexer->buffer_s <= 0) {
        lPrintError("a totally empty file founded", LX_EMPTY_FILE_PASSED);
    }

    lexer->_char = lexer->buffer[0];
    lexer->line = 1;

    return lexer;
}

void lx_free(Lexer *lexer) {
    free(lexer);
    lexer = NULL;
}

int lx_skip(Lexer *lexer) {
    lexer->fileCursor++;
    lexer->bufferCursor++;

    if (lexer->_char == '\r' && lx_peek(lexer) == '\n') {
        lexer->line++;

        lexer->_char = '\0';

        lx_skip(lexer);

    } else if (lexer->_char == '\n' || lexer->_char == '\r')
            lexer->line++;

    if (lexer->bufferCursor == lexer->buffer_s) {
        lexer->buffer_s = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);

        lexer->bufferCursor = 0;

        lexer->_char = lexer->buffer[0];

        if (lexer->buffer_s == 0)
            return 0;
    } else {
        lexer->_char = lexer->buffer[lexer->bufferCursor];
    }

    if (feof(lexer->file) == 0)
        return 0;

    return 1;
}

int lx_skipEmpty(Lexer *lexer) {
    while (isspace(lexer->_char))
        if (lx_skip(lexer) == 0) 
            return 0;

    return 1;
}

int lx_skipBlank(Lexer *lexer) {
    while (isblank(lexer->_char))
        if (lx_skip(lexer) == 0)
            return 0;

    return 1;
}

void lx_skipComment(Lexer *lexer) {
    lx_skip(lexer); //skiping the first '/'

    if (lexer->_char == '/') { //if the comment start with "//"
        lx_skip(lexer); //skiping the second '/'

        while (lexer->_char != '\n' && lexer->_char != '\r' && feof(lexer->file) != 0) { //if the line end
            lx_skip(lexer); //skiping the character
        }

    } else if (lexer ->_char == '*') { //if the comment start with "/*" (multi line comment)
        LX_SKIPED_OP("bad formated multi line comment"); //skiping the '*'

        while (lexer->_char != '*' && lx_peek(lexer) != '/') { //skip the character ultil reach the end of comment
            LX_SKIPED_OP("bad formated multi line comment"); //skip the caracter, but if reach the end of file, this is an error
        }

        lx_skip(lexer); //skip the '*'
        lx_skip(lexer); //skip the '/'
    }

    lx_skipEmpty(lexer); //skip all empty character after the comments
}

char lx_peek(Lexer *lexer) {
    if (lexer->bufferCursor + 1 < lexer->buffer_s) {
        return lexer->buffer[lexer->bufferCursor + 1];

    } else if (feof(lexer->file)) {
        return EOF;

    }

    size_t oldCursor = lexer->fileCursor;

    char nextChar;

    fseek(lexer->file, 1, SEEK_CUR);

    fread(&nextChar, 1, 1, lexer->file);

    fseek(lexer->file, oldCursor, SEEK_SET);

    return nextChar;
}

Token* lx_getNextToken(Lexer *lexer) {

    if (lx_skipEmpty(lexer) == 0) 
        return tk_create(TOKEN_EOF, lexer->line);

    if (isgraph(lexer->_char)) {

        if (lexer->_char == '/') {
            lx_skipComment(lexer);
            if (feof(lexer->file) == 0)
                return tk_create(TOKEN_EOF, lexer->line);
        }

        if (isdigit(lexer->_char)) 
            return lx_getDigit(lexer);

        if (isalpha(lexer->_char) || lexer->_char == '_')
            return lx_getIdentifier(lexer);

        if (lexer->_char == '\'')
            return lx_getChar(lexer);

        if (lexer->_char == '"')
            return lx_getString(lexer);
        
        if (lexer->_char == '@')
            return lx_getCompilerDirective(lexer);

        if (lexer->_char == '$')
            return lx_getAssemblerDirective(lexer);

        return lx_getSymbol(lexer);
    }

    return tk_create(TOKEN_INVALID_TOKEN, lexer->line);
}

Token* lx_getDigit(Lexer *lexer) {
    LX_GET_BASE(digits, TOKEN_INT_NUMBER, isdigit(lexer->_char));
}

Token* lx_getIdentifier(Lexer *lexer) {
    LX_GET_BASE(identifier, lx_getKeywordType(lexer, identifier, identifier_s), isalnum(lexer->_char));
}

Token* lx_getChar(Lexer *lexer) {
    LX_GET_BASE(_char, TOKEN_CHAR_LITERAL, isdigit(lexer->_char));
}

Token* lx_getString(Lexer *lexer) {
    char string[] = {'\0', '\0', '\0', '\0'};

    RuneString *_runeString = rs_create();

    LX_SKIPED_OP("string don't have a end");;

    while (lexer->_char != '"') {
        
        string[0] = lexer->_char;

        if (lexer->_char == '\\') {
            rs_add(_runeString, lx_convertScapeSequence(lexer));
            continue;
        }

        LX_SKIPED;
    }

    return tk_create_op(TOKEN_STRING_LITERAL, lexer->line, rs_converToString(_runeString), _runeString->runeQuantity * 4);
}

Token* lx_getCompilerDirective(Lexer *lexer) {
    LX_GET_BASE(directive, TOKEN_INT_NUMBER, isdigit(lexer->_char));
}

Token* lx_getAssemblerDirective(Lexer *lexer) {
    LX_GET_BASE(directive, TOKEN_ASSEMBLER_DIRECTIVE, isalnum(lexer->_char) || lexer->_char == '_');
}

Token* lx_getSymbol(Lexer *lexer) {

#define RETURN_AND_SKIP(token ) lx_skip(lexer); \
            return tk_create(token, lexer->line); \

    switch (lexer->_char) {
        case '(':
            RETURN_AND_SKIP(TOKEN_LEFT_PARENTHESE);
        break;
        case ')':
            RETURN_AND_SKIP(TOKEN_RIGHT_PARENTHESE);
        break;
        case ',':
            RETURN_AND_SKIP(TOKEN_COMMA);
        break;
        case ':':
            LX_SKIPED_OP("the character ':' need always an other character after it");

            if (lexer->_char == ':') {
                LX_SKIPED_OP("the character '::' need always an other character after it");
                return tk_create(TOKEN_DOUBLE_COLON, lexer->line);
            }

            return tk_create(TOKEN_COLON, lexer->line);
        break;
        case ';':
            RETURN_AND_SKIP(TOKEN_SEMICOLON);
        break;
        case '[':
            RETURN_AND_SKIP(TOKEN_LEFT_BRACKET);
        break;
        case ']':
            RETURN_AND_SKIP(TOKEN_RIGHT_BRACKET);
        break;
        case '{':
            RETURN_AND_SKIP(TOKEN_LEFT_BRACE);
        break;
        case '}':
            RETURN_AND_SKIP(TOKEN_RIGHT_BRACE);
        break;
    }

    return tk_create(TOKEN_INVALID_TOKEN, lexer->line);
}

rune_t lx_convertScapeSequence(Lexer *lexer) {

#define LX_SKIP_VERIFY_INVALID_SSEQUENCE LX_SKIPED_OP("invalid scape sequence")

    rune_t rune;

    LX_SKIP_VERIFY_INVALID_SSEQUENCE;

    switch (lexer->_char) {
        case '"':
            return (rune_t)'"';
        break;
        case '\'':
            return (rune_t)'\'';
        break;
        case '0':
            return (rune_t)'\0';
        break;
        case '?':
            return (rune_t)'\?';
        break;
        case 'A':
        case 'a':
            return (rune_t)'\a';
        break;
        case 'B':
        case 'b':
            return (rune_t)'\b';
        break;
        case 'F':
        case 'f':
            return (rune_t)'\f';
        break;
        case 'N':
        case 'n':
            return (rune_t)'\n';
        break;
        case 'R':
        case 'r':
            return (rune_t)'\r';
        break;
        case 'T':
        case 't':
            return (rune_t)'\t';
        break;
        case 'V':
        case 'v':
            return (rune_t)'\v';
        break;
        case 'X':
        case 'x':
            LX_SKIP_VERIFY_INVALID_SSEQUENCE;

            //TODO: repart the code for more modular way

            if (lexer->_char == 'x') {
                LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                if (lexer->_char == 'x') {
                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                    if (lexer->_char == 'x') {
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                        if (lexer->_char == 'x')
                            lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT);

                        char oldChars1Hexa[2];

                        oldChars1Hexa[0] = lexer->_char;

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars1Hexa[1] = lexer->_char;

                        char oldChars2Hexa[2];

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars2Hexa[0] = lexer->_char;

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars2Hexa[1] = lexer->_char;

                        char oldChars3Hexa[2];

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars3Hexa[0] = lexer->_char;

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars3Hexa[1] = lexer->_char;

                        char oldChars4Hexa[2];

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars4Hexa[0] = lexer->_char;

                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                        oldChars4Hexa[1] = lexer->_char;

                        uint8_t value[4];
                        value[0] = charToHexa(oldChars1Hexa, 2);
                        value[1] = charToHexa(oldChars2Hexa, 2);
                        value[2] = charToHexa(oldChars3Hexa, 2);
                        value[3] = charToHexa(oldChars4Hexa, 2);

                        rune = value[0];
                        rune <<= 8;

                        rune = value[1];
                        rune <<= 8;

                        rune = value[2];
                        rune <<= 8;
                        
                        rune |= value[3];

                        return rune;
                    }

                    char oldChars1Hexa[2];

                    oldChars1Hexa[0] = lexer->_char;

                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    oldChars1Hexa[1] = lexer->_char;

                    char oldChars2Hexa[2];

                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    oldChars2Hexa[0] = lexer->_char;

                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    oldChars2Hexa[1] = lexer->_char;

                    char oldChars3Hexa[2];

                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    oldChars3Hexa[0] = lexer->_char;

                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    oldChars3Hexa[1] = lexer->_char;

                    uint8_t value[3];
                    value[0] = charToHexa(oldChars1Hexa, 2);
                    value[1] = charToHexa(oldChars2Hexa, 2);
                    value[2] = charToHexa(oldChars3Hexa, 2);

                    rune = value[0];
                    rune <<= 8;

                    rune = value[1];
                    rune <<= 8;
                    
                    rune |= value[2];

                    return rune;
                }

                LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                char oldChars1Hexa[2];

                oldChars1Hexa[0] = lexer->_char;

                LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                oldChars1Hexa[1] = lexer->_char;

                char oldChars2Hexa[2];

                LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                oldChars2Hexa[0] = lexer->_char;

                LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                oldChars2Hexa[1] = lexer->_char;

                uint8_t value[2];
                value[0] = charToHexa(oldChars1Hexa, 2);
                value[1] = charToHexa(oldChars2Hexa, 2);

                rune = value[0];
                rune <<= 8;
                
                rune |= value[1];

                return rune;
            } 

            char oldChar = lexer->_char;

            LX_SKIP_VERIFY_INVALID_SSEQUENCE;

            char _chars[2] = {oldChar, lexer->_char};

            uint8_t value = charToHexa(_chars, 2);

            rune = value;

            return rune;
        break;
        case '\\':
            return (rune_t)'\a';
        break;
    }

    lPrintError("can't identify the scape sequence", LX_CANT_GET_SSEQUENCE);

    return rune;
}

uint8_t charToHexa(char _chars[], size_t _chars_s) {
    if (_chars_s != 2)
        lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT);

    uint8_t value[2] = {0, 0};
    
    for (int i = 0; i < 2; i++) {
        if (_chars[i] >= '0' && _chars[i] <= '9')
            value[i] = _chars[i] - '0';
        else if (_chars[i] >= 'a' && _chars[i] <= 'f')
            value[i] = _chars[i] - 'a' + 10;
        else if (_chars[i] >= 'A' && _chars[i] <= 'F')
            value[i] = _chars[i] - 'A' + 10;
        else 
            lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT);
    }

    uint8_t returnValue = value[0];

    returnValue <<= 4;

    returnValue |= value[1];

    return returnValue;
}

int lx_getKeywordType(Lexer *lexer, char identifier[], size_t identifier_s) {
    switch (identifier_s) {
        case 3:
            if (STR_N_EQUALS(identifier, "def", identifier_s))
                return TOKEN_DEF;
            if (STR_N_EQUALS(identifier, "int", identifier_s))
                return TOKEN_INT;
        break;
        case 6:
            if (STR_N_EQUALS(identifier, "return", identifier_s))
                return TOKEN_DEF;
            if (STR_N_EQUALS(identifier, "string", identifier_s))
                return TOKEN_DEF;
        break;
        case 9:
            if (STR_N_EQUALS(identifier, "namespace", identifier_s))
                return TOKEN_DEF;
        break;
    }

    return TOKEN_IDENTIFIER;
}

void lPrint(char string[]) {
    printf("[Lexer]: %s\n", string);
}

void lPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Lexer]: ");
}
