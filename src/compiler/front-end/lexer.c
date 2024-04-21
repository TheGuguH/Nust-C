#include "headers/lexer.h"

#include "headers/token.h"
#include "../../headers/error_codes.h"
#include "../../headers/utf8_compatibility.h"
#include "../../headers/utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LX_SKIPED if (lx_skip(_lexer) == 0) \
            lPrintError("early EOF", LX_EARLY_EOF, _lexer->line); \

#define LX_SKIPED_OP(text) if (lx_skip(_lexer) == 0) \
            lPrintError("early EOF, " #text, LX_EARLY_EOF, _lexer->line); \

#define LX_GET_BASE(sequence, token, verify) char *sequence = calloc(1, sizeof(char)); \
    size_t sequence##_s = 1; \
 \
    sequence[0] = _lexer->_char; \
 \
    LX_SKIPED; \
 \
    while (verify) { \
        sequence##_s++; \
 \
        sequence = realloc(sequence, sequence##_s); \
 \
        sequence[sequence##_s - 1] = _lexer->_char; \
 \
        LX_SKIPED; \
    } \
 \
    return tk_create_op(token, _lexer->line, sequence, sequence##_s); \

Lexer* lx_create(FILE *file) {
    Lexer *_lexer = malloc(sizeof(Lexer));

    if (_lexer == NULL) {
        lPrintError("can't alloc memory for a _lexer", LX_MEMORY_ALLOC_ERROR, 0);
    }

    _lexer->file = file;
    _lexer->fileCursor = 0;
    _lexer->bufferCursor = 0;

    _lexer->buffer_s = fread(_lexer->buffer, 1, BUFFER_SIZE, _lexer->file);

    if (_lexer->buffer_s <= 0) {
        lPrintError("a totally empty file founded", LX_EMPTY_FILE_PASSED, 0);
    }

    _lexer->_char = _lexer->buffer[0];
    _lexer->line = 1;

    return _lexer;
}

void lx_free(Lexer *_lexer) {
    free(_lexer);
    _lexer = NULL;
}

int lx_skip(Lexer *_lexer) {
    _lexer->fileCursor++;
    _lexer->bufferCursor++;

    if (_lexer->_char == '\r' && lx_peek(_lexer) == '\n') {
        _lexer->line++;

        _lexer->_char = '\0';

        lx_skip(_lexer);

    } else if (_lexer->_char == '\n' || _lexer->_char == '\r')
            _lexer->line++;

    if (_lexer->bufferCursor == _lexer->buffer_s) {
        _lexer->buffer_s = fread(_lexer->buffer, 1, BUFFER_SIZE, _lexer->file);

        _lexer->bufferCursor = 0;

        _lexer->_char = _lexer->buffer[0];

        if (_lexer->buffer_s == 0) {
            _lexer->_char = EOF;
            return 0;
        }
    } else {
        _lexer->_char = _lexer->buffer[_lexer->bufferCursor];
    }

    if (feof(_lexer->file) == 0) {
        _lexer->_char = EOF;
        return 0;
    }

    return 1;
}

int lx_skipEmpty(Lexer *_lexer) {
    while (isspace(_lexer->_char))
        if (lx_skip(_lexer) == 0) 
            return 0;

    return 1;
}

int lx_skipBlank(Lexer *_lexer) {
    while (isblank(_lexer->_char))
        if (lx_skip(_lexer) == 0)
            return 0;

    return 1;
}

void lx_skipComment(Lexer *_lexer) {
    lx_skip(_lexer); //skiping the first '/'

    if (_lexer->_char == '/') { //if the comment start with "//"
        lx_skip(_lexer); //skiping the second '/'

        while (_lexer->_char != '\n' && _lexer->_char != '\r' && feof(_lexer->file) != 0) { //if the line end
            lx_skip(_lexer); //skiping the character
        }

    } else if (_lexer ->_char == '*') { //if the comment start with "/*" (multi line comment)
        LX_SKIPED_OP("bad formated multi line comment"); //skiping the '*'

        while (_lexer->_char != '*' && lx_peek(_lexer) != '/') { //skip the character ultil reach the end of comment
            LX_SKIPED_OP("bad formated multi line comment"); //skip the caracter, but if reach the end of file, this is an error
        }

        lx_skip(_lexer); //skip the '*'
        lx_skip(_lexer); //skip the '/'
    }

    lx_skipEmpty(_lexer); //skip all empty character after the comments
}

char lx_peek(Lexer *_lexer) {
    if (_lexer->bufferCursor + 1 < _lexer->buffer_s) {
        return _lexer->buffer[_lexer->bufferCursor + 1];

    } else if (feof(_lexer->file)) {
        return EOF;

    }

    size_t oldCursor = _lexer->fileCursor;

    char nextChar;

    fseek(_lexer->file, 1, SEEK_CUR);

    fread(&nextChar, 1, 1, _lexer->file);

    fseek(_lexer->file, oldCursor, SEEK_SET);

    return nextChar;
}

Token* lx_getNextToken(Lexer *_lexer) {
    if (lx_skipEmpty(_lexer) == 0) 
        return tk_create(TOKEN_EOF, _lexer->line);

    return lx_identifyToken(_lexer);
}

Token* lx_getNextTokenInSameLine(Lexer *_lexer) {
    if (lx_skipBlank(_lexer) == 0) 
        return tk_create(TOKEN_EOF, _lexer->line);

    return lx_identifyToken(_lexer);
}

Token* lx_identifyToken(Lexer *_lexer) {
    if (isgraph(_lexer->_char)) {

        if (_lexer->_char == '/') {
            lx_skipComment(_lexer);
            if (feof(_lexer->file) == 0)
                return tk_create(TOKEN_EOF, _lexer->line);
        }

        if (isdigit(_lexer->_char)) 
            return lx_getDigit(_lexer);

        if (isalpha(_lexer->_char) || _lexer->_char == '_')
            return lx_getIdentifier(_lexer);

        if (_lexer->_char == '\'')
            return lx_getChar(_lexer);

        if (_lexer->_char == '"')
            return lx_getString(_lexer);
        
        if (_lexer->_char == '@')
            return lx_getCompilerDirective(_lexer);

        if (_lexer->_char == '$')
            return lx_getAssemblerDirective(_lexer);

        return lx_getSymbol(_lexer);
    }

    return tk_create(TOKEN_INVALID_TOKEN, _lexer->line);
}

Token* lx_getDigit(Lexer *_lexer) {
    LX_GET_BASE(digits, TOKEN_INT_NUMBER, isdigit(_lexer->_char));
}

Token* lx_getIdentifier(Lexer *_lexer) {
    LX_GET_BASE(identifier, lx_getKeywordType(identifier, identifier_s), isalnum(_lexer->_char));
}

Token* lx_getChar(Lexer *_lexer) {
    rune_t _char = '\0';

    LX_SKIPED_OP("invalid char");

    char string[] = {'\0', '\0', '\0', '\0'};

    string[0] = _lexer->_char;

    int charProbalySize = charUTF8Lenght(string[0]);
    
    switch (_lexer->_char) {
        case '\'':
            LX_SKIPED;
            return tk_create_op(TOKEN_CHAR_LITERAL, _lexer->line, (char*)&_char, 4);
        break;
        case '\\':
            _char = lx_convertScapeSequence(_lexer);
        break;
        default:
            for (int i = 1; i < charProbalySize; i++) {

                LX_SKIPED_OP("invalid charr");

                string[i] = _lexer->_char;

                if (_lexer->_char == '\'') 
                    lPrintError("invalid character of a char", LX_INVALID_CHAR, _lexer->line);
            }

            _char = rt_create((unsigned char*)&string, charProbalySize);

            LX_SKIPED;
        break;
    }

    if (_lexer->_char != '\'')
        lPrintError("invalid format of a char", LX_INVALID_CHAR_FORMAT, _lexer->line);

    LX_SKIPED;

    return tk_create_op(TOKEN_CHAR_LITERAL, _lexer->line, (char *)&_char, sizeof(rune_t));
}

Token* lx_getString(Lexer *_lexer) {
    char string[] = {'\0', '\0', '\0', '\0'};

    RuneString *_runeString = rs_create();

    LX_SKIPED_OP("invalid string");;

    while (_lexer->_char != '"') {
        for (int i = 0; i < 4; i++) {
            string[i] = '\0';
        }

        if (_lexer->_char == '\\') {
            rs_add(_runeString, lx_convertScapeSequence(_lexer));
            continue;
        }

        string[0] = _lexer->_char;

        int charProbalySize = charUTF8Lenght(string[0]);

        for (int i = 1; i < charProbalySize; i++) {

            LX_SKIPED_OP("invalid string");

            string[i] = _lexer->_char;

            if (_lexer->_char == '"') 
                lPrintError("invalid character of a string", LX_INVALID_CHARACTER, _lexer->line);
        }

        rs_addChar(_runeString, string, charProbalySize);

        LX_SKIPED;
    }

    LX_SKIPED;

    return tk_create_op(TOKEN_STRING_LITERAL, _lexer->line, rs_convertToString(_runeString), _runeString->runeQuantity * 4);
}

Token* lx_getCompilerDirective(Lexer *_lexer) {
    LX_GET_BASE(directive, TOKEN_INT_NUMBER, isdigit(_lexer->_char));
}

Token* lx_getAssemblerDirective(Lexer *_lexer) {
    LX_GET_BASE(directive, TOKEN_ASSEMBLER_DIRECTIVE, isalnum(_lexer->_char) || _lexer->_char == '_');
}

Token* lx_getSymbol(Lexer *_lexer) {

#define RETURN_AND_SKIP(token ) lx_skip(_lexer); \
            return tk_create(token, _lexer->line); \

    switch (_lexer->_char) {
        case '!':
            RETURN_AND_SKIP(TOKEN_NOT);
        break;
        case '%':
            RETURN_AND_SKIP(TOKEN_REST);
        break;
        case '&':
            lx_skip(_lexer);

            if (_lexer->_char == '&') {
                lx_skip(_lexer);
                return tk_create(TOKEN_AND, _lexer->line);
            }
            if (_lexer->_char == '=') {
                lx_skip(_lexer);
                return tk_create(TOKEN_AND, _lexer->line);
            }

            return tk_create(TOKEN_BITW_AND, _lexer->line);
        break;
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

            if (_lexer->_char == ':') {
                lx_skip(_lexer);
                return tk_create(TOKEN_DOUBLE_COLON, _lexer->line);
            }

            return tk_create(TOKEN_COLON, _lexer->line);
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

    return tk_create(TOKEN_INVALID_TOKEN, _lexer->line);
}

rune_t lx_convertScapeSequence(Lexer *_lexer) {

#define LX_SKIP_VERIFY_INVALID_SSEQUENCE LX_SKIPED_OP("invalid scape sequence")

#define SET_HEXA_SCAPE(index0, index1) hexaChars[index0] = _lexer->_char; \
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE; \
                        hexaChars[index1] = _lexer->_char; \
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE; \

    rune_t rune;

    LX_SKIP_VERIFY_INVALID_SSEQUENCE;

    switch (_lexer->_char) {
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

            if (_lexer->_char == 'x') {
                LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                if (_lexer->_char == 'x') {
                    LX_SKIP_VERIFY_INVALID_SSEQUENCE;
                    if (_lexer->_char == 'x') {
                        LX_SKIP_VERIFY_INVALID_SSEQUENCE;

                        SET_HEXA_SCAPE(0, 1);
                    }

                    SET_HEXA_SCAPE(2, 3);
                }

                SET_HEXA_SCAPE(4, 5);
            }
            SET_HEXA_SCAPE(6, 7);

            rune = (rune_t)hexaToRune(hexaChars, 8, _lexer->line);

            return rune;
        break;
        case '\\':
            rune = (rune_t)'\\';
        break;
        default:
            lPrintError("can't identify the scape sequence", LX_CANT_GET_SSEQUENCE, _lexer->line);
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
        char hexaChars[] = {_chars[i * 2], _chars[(i * 2) + 1]};
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

int lx_getKeywordType(char identifier[], size_t identifier_s) {
    switch (identifier_s) {
        case 2:
            if (STR_N_EQUALS(identifier, "if", identifier_s))
                return TOKEN_IF;
        break;
        case 3:
            if (STR_N_EQUALS(identifier, "def", identifier_s))
                return TOKEN_DEF;
            if (STR_N_EQUALS(identifier, "int", identifier_s))
                return TOKEN_INT;
        break;
        case 4:
            if (STR_N_EQUALS(identifier, "else", identifier_s))
                return TOKEN_ELSE;
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
