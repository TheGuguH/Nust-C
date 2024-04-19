#include "headers/lexer.h"

#include "headers/token.h"
#include "../../headers/error_codes.h"
#include "../../headers/utf8_compatibility.h"
#include "../../headers/utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LX_SKIPED if (lx_skip(lexer) == 0) \
            lPrintError("early EOF", LX_EARLY_EOF, lexer->line); \

#define LX_SKIPED_OP(text) if (lx_skip(lexer) == 0) \
            lPrintError("early EOF, " #text, LX_EARLY_EOF, lexer->line); \

#define LX_GET_BASE(sequence, token, verify) char *sequence = calloc(1, sizeof(char)); \
    size_t sequence##_s = 1; \
 \
    sequence[0] = lexer->_char; \
 \
    LX_SKIPED; \
 \
    while (verify) { \
        sequence##_s++; \
 \
        sequence = realloc(sequence, sequence##_s); \
 \
        sequence[sequence##_s - 1] = lexer->_char; \
 \
        LX_SKIPED; \
    } \
 \
    return tk_create_op(token, lexer->line, sequence, sequence##_s); \

Lexer* lx_create(FILE *file) {
    Lexer *lexer = malloc(sizeof(Lexer));

    if (lexer == NULL) {
        lPrintError("can't alloc memory for a lexer", LX_MEMORY_ALLOC_ERROR, 0);
    }

    lexer->file = file;
    lexer->fileCursor = 0;
    lexer->bufferCursor = 0;

    lexer->buffer_s = fread(lexer->buffer, 1, BUFFER_SIZE, lexer->file);

    if (lexer->buffer_s <= 0) {
        lPrintError("a totally empty file founded", LX_EMPTY_FILE_PASSED, 0);
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

    LX_SKIPED_OP("invalid string");;

    while (lexer->_char != '"') {
        for (int i = 0; i < 4; i++) {
            string[i] = '\0';
        }

        if (lexer->_char == '\\') {
            rs_add(_runeString, lx_convertScapeSequence(lexer));
            continue;
        }

        string[0] = lexer->_char;

        int charProbalySize = charUTF8Lenght(string[0]);

        for (int i = 1; i < charProbalySize; i++) {

            LX_SKIPED_OP("invalid string");

            string[i] = lexer->_char;

            if (lexer->_char == '"') 
                lPrintError("invalid character of a string", LX_INVALID_CHARACTER, lexer->line);
        }

        rs_addChar(_runeString, string, charProbalySize);

        LX_SKIPED;
    }

    LX_SKIPED;

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
        case '*':
            RETURN_AND_SKIP(TOKEN_ASTERISK);
        break;
        case '+':
            RETURN_AND_SKIP(TOKEN_PLUS); 
        break;
        case '-':
            RETURN_AND_SKIP(TOKEN_PLUS);
        break;
        case '/':
            RETURN_AND_SKIP(TOKEN_SLASH);
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
        case '<':
            RETURN_AND_SKIP(TOKEN_LESS_THAN);
        break;
        case '=':
            RETURN_AND_SKIP(TOKEN_EQUALS);
        break;
        case '>':
            RETURN_AND_SKIP(TOKEN_GREATER_THAN);
        break;
        case '?':
            RETURN_AND_SKIP(TOKEN_QUESTION_MARK);
        break;
        case '[':
            RETURN_AND_SKIP(TOKEN_LEFT_BRACKET);
        break;
        case '\\':
            RETURN_AND_SKIP(TOKEN_BACK_SLASH);
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

#define SET_HEXA_SCAPE(index0, index1) hexaChars[index0] = lexer->_char; \
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE; \
                        hexaChars[index1] = lexer->_char; \
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE; \

    rune_t rune;

    LX_SKIP_VERIFY_INVALID_SSEQUENCE;

    switch (lexer->_char) {
        case '"':
            rune = (rune_t)'"';
        break;
        case '\'':
            rune = (rune_t)'\'';
        break;
        case '0':
            rune = (rune_t)'\0';
        break;
        case '?':
            rune = (rune_t)'\?';
        break;
        case 'A':
        case 'a':
            rune = (rune_t)'\a';
        break;
        case 'B':
        case 'b':
            rune = (rune_t)'\b';
        break;
        case 'F':
        case 'f':
            rune = (rune_t)'\f';
        break;
        case 'N':
        case 'n':
            rune = (rune_t)'\n';
        break;
        case 'R':
        case 'r':
            rune = (rune_t)'\r';
        break;
        case 'T':
        case 't':
            rune = (rune_t)'\t';
        break;
        case 'V':
        case 'v':
            rune = (rune_t)'\v';
        break;
        case 'X':
        case 'x':
            LX_SKIP_VERIFY_INVALID_SSEQUENCE;

            char hexaChars[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};

            if (lexer->_char == 'x') {
                LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                
                if (lexer->_char == 'x') {
                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    if (lexer->_char == 'x') {
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                        SET_HEXA_SCAPE(0, 1);
                    }

                    SET_HEXA_SCAPE(2, 3);
                }

                SET_HEXA_SCAPE(4, 5);
            }
            SET_HEXA_SCAPE(6, 7);

            printf("%c%c%c%c\n", hexaChars[0], hexaChars[1], hexaChars[2], hexaChars[3]);

            rune = (rune_t)hexaToRune(hexaChars, 8, lexer->line);

            printf("%u", rune);
            return rune;
        break;
        case '\\':
            rune = (rune_t)'\a';
        break;
        default:
            lPrintError("can't identify the scape sequence", LX_CANT_GET_SSEQUENCE, lexer->line);
        break;
    }

    LX_SKIPED;

    return rune;
}

uint32_t hexaToRune(char _chars[], size_t _chars_s, size_t line) {
    if (_chars_s != 8)
        lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT, line);

    uint8_t values[4] = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        char hexaChars[] = {_chars[i], _chars[i + 1]};
        values[i] = hexaToValue(hexaChars, 2, line);
    }

    uint32_t returnValue = values[0];

    returnValue <<= 8;

    returnValue |= values[1];

    returnValue <<= 8;

    returnValue |= values[2];

    returnValue <<= 8;

    returnValue |= values[3];

    return returnValue;
}

uint8_t hexaToValue(char _chars[], size_t _chars_s, size_t line) {
    if (_chars_s != 2)
        lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT, line);

    uint8_t value[2] = {0, 0};
    
    for (int i = 0; i < 2; i++) {
        if (_chars[i] >= '0' && _chars[i] <= '9')
            value[i] = _chars[i] - '0';
        else if (_chars[i] >= 'a' && _chars[i] <= 'f')
            value[i] = _chars[i] - 'a' + 10;
        else if (_chars[i] >= 'A' && _chars[i] <= 'F')
            value[i] = _chars[i] - 'A' + 10;
        else 
            lPrintError("invalid hexadecimal format", LX_HEXA_BAD_FORMAT, line);
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

void lPrint(char string[], size_t line) {
    if (line != 0)
        printf("line->%lu: ", line);
    printf("[Lexer]: %s\n", string);
}

void lPrintError(char string[], int errorCode, size_t line) {
    if (line != 0)
        printf("line->%lu: ", line);

    P_ERROR_FORMAT("[Lexer]: ");
}
