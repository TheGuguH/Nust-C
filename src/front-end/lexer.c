#include "front-end/lexer.h"

#include "error.h"
#include "front-end/token.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Heap manipulation

Lexer* lx_create(char *file) {

    Lexer *lx = malloc(sizeof(Lexer));

    if (lx == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Lexer");

    // Making token memory pool
    lx->_token = malloc(sizeof(Token));

    if (lx->_token == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Token pool in Lexer");

    // Making token string memory pool (with 32 of defalt size)
    lx->strPool = malloc(STR_POOL_DEFAULT_SIZE * sizeof(char));

    if (lx->_token == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to malloc a Token string pool in Lexer");

    // Open file and set offset and line to 0
    lx->_file = fopen(file, "r");
    lx->line = 0;
    lx->fileOffset = 0;

    // If file equals null, that means the file don't exists (or we can't access)
    if (lx->_file == NULL)
        err_printExit(ERROR_TRY_TO_USE_NULL, "The file \"%s\"don't exists or we can't access", file);

    // Read a maximum of 4096 bytes of file (the size of bytes readed will go in buffer_s variable)
    lx->buffer_s = fread(lx->buffer, 1, BUFFER_SIZE, lx->_file);

    // If the buffer_s is less than 1 (0) or we reach the EOF (end of file), it means the file is empty
    if (lx->buffer_s < 1 || feof(lx->_file))
        err_warn(WARN_EMPTY_FILE, "The file \"%s\" is empty", file);

    // Set the buffer offset to 0
    lx->bufferOffset = 0;

    return lx;
}

void lx_free(Lexer *lx) {

    // Freeing all heap memory
    free(lx->strPool);
    lx->strPool = NULL;

    free(lx->_token);
    lx->_token = NULL;

    fclose(lx->_file);
    lx->_file = NULL;

    free(lx);
    lx = NULL;
}

void lx_addChar(Lexer *lx, char _char) {

    // Realloc new size of string pool
    if ((lx->strPool_s - 1) == lx->_token->value_s)
        lx->strPool = realloc(lx->strPool, sizeof(char) * (lx->strPool_s * STR_POOL_MULTIPLIER));

    // Set the next char (lx->_token->value_s)
    lx->strPool[lx->_token->value_s] = _char;
    
    // Add one to lenght
    lx->_token->value_s++;
    
    // Set the next char to null character (i.e. string terminator)
    lx->strPool[lx->_token->value_s] = '\0';
}

// Token related functions

void lx_tk_reset(Lexer *lx) {
    
    // Reset token to defaults
    lx->_token->type = TOKEN_INVALID;
    lx->_token->value = lx->strPool;
    lx->_token->value_s = 0;

    // String pool too!
    lx->strPool[0] = '\0';
}

Token* lx_tk_copy(Lexer *lx) {
    
    // Alloc heap memory for token (is not recommended to use heap pointers in stack memory!) and string
    Token *tk = malloc(sizeof(Token));

    if (tk == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to alloc memory for a Token");

    // It will set ALL bytes to 0. It's used for arrays or strings
    tk->value = calloc(sizeof(char), lx->_token->value_s + 1);

    if (tk->value == NULL)
        err_printExit(ERROR_MALLOC_FAILED, "Failed to alloc memory for a Token value");

    // Because 'calloc' insted 'malloc', this line is not necessary
    // tk->value[0] = '\0';

    // Apprend n string pools chars to token value
    strncat(tk->value, lx->strPool, lx->_token->value_s);

    // Set the values...
    tk->type = lx->_token->type;
    tk->value_s = lx->_token->value_s;
    tk->line = lx->line;
    tk->collum = lx->collum;

    // And now the token has been copied! Basically a ctrl + c and ctrl + v
    return tk;
}

// Utils

/*
 This functions has few possible cases:

 1º: if we reach the end of file, set the char to 'EOF' and return
 2º: we reach the end of buffer
  In this case we need to treat some others cases:
   We read the next buffer
   If the new buffer size is greater than 0 and we don't reach the eof, get the next char
   Else, set char to 'EOF' and return
 3º: if we not reach the end of buffer, get the next char

 EOF represents the end of file, a char type, so it can be:
  signed char: EOF = -1
  unsigned char: EOF = 255
 ALWAYS 0xff!
*/
void lx_skip(Lexer *lx) {

    // Add one to buffer and file offset (aka cursor)
    lx->fileOffset++;
    lx->bufferOffset++;

    // Every 'new line' char is a new line and a new collum!
    if (lx->_char == '\n') {

        lx->line++;
        lx->collum = 0;
    } else
        lx->collum++;

    // 1º case (eof)
    if (feof(lx->_file) || (lx->buffer_s < BUFFER_SIZE && lx->bufferOffset == lx->buffer_s)) {

        lx->_char = EOF;

        // return... we don't have more to work ¯\_(ツ)_/¯
        return;
    }

    // 2º case (end of buffer)
    if (lx->bufferOffset == lx->buffer_s) {

        lx->buffer_s = fread(lx->buffer, 1, BUFFER_SIZE, lx->_file);

        // 1º case (buffer is greater than 0 and we don't reach the end of file) of 2º (end of buffer)...
        if (lx->buffer_s > 0 && !feof(lx->_file)) {

            lx->bufferOffset = 0;
            lx->_char = lx->buffer[0];
        } else { // 2º case (we reach end of file or the buffer is less or equals than 0) of 2º (end of buffer)...

            lx->_char = EOF;

            // return... we don't have more to work ¯\_(ツ)_/¯
            return;
        }
    } else // 3º case (not end of buffer)
        lx->_char = lx->buffer[lx->bufferOffset];
}

void lx_skipSpace(Lexer *lx) {

    // C "spaces" are new line (\n), tabs (\v and \t), feed (\f), carriege return (\r) and space ( )
    while (isspace(lx->_char))
        lx_skip(lx);
}

void lx_skipBlank(Lexer *lx) {

    // C "blanks" are space ( ) and horizontal tabs (\t)
    while (isblank(lx->_char))
        lx_skip(lx);
}

/*
 This function get the token based on _char from Lexer *lx

 Has some types of tokens:
 1º: '#' -> its means the token will be a preprocessor
 2º: '$' -> its means the token will be a assembler directive (or assembler define)
 3º: '0-9' -> its means the token will be a number constant
 4º: '_' -> its mens the token will be a identifier
 5º: 'a-z|A-Z' its means the token will be a keyword or a identifier (we need to understand what will be)
 6º: otherwise, it will be a symbol
*/
Token* lx_getToken(Lexer *lx) {
    
    lx_tk_reset(lx); // reset the previus token

    if (lx->_char == '#')
        lx_getPreProcessor(lx);
    else if (lx->_char == '$')
        lx_getAssembly(lx);
    else if (isdigit(lx->_char))
        lx_getNumber(lx);
    else if (lx->_char == '_')
        lx_getIdentifier(lx);
    else if (isalpha(lx->_char))
        lx_getWord(lx);
    else
        lx_getSymbol(lx);

    return lx->_token;
}

Token* lx_getNext(Lexer *lx) {

    lx_skipSpace(lx);

    return lx_getToken(lx);
}

Token* lx_getNextInLine(Lexer *lx) {

    lx_skipBlank(lx);

    return lx_getToken(lx);
}

/*
 A preprocessor start with a '#' and ALWAYS have at last a alpha character and NEVER a digit
*/
void lx_getPreProcessor(Lexer *lx) {

    // Skip the '#'
    lx_skip(lx);

    while(isalpha(lx->_char)) {

        lx_addChar(lx, lx->_char);

        lx_skip(lx);
    }

    // ALWAYS need to have a charracter
    if (lx->_token->value_s == 0)
        lx->_token->type = TOKEN_INVALID;

    // Never can have a digit
    if (isdigit(lx->_char)) 
        lx->_token->type = TOKEN_INVALID;
}

/*
 A assembler preprocessor/macro start with a "$_", ALWAYS upper characters and end with '_'
*/
void lx_getAssembly(Lexer *lx) {

    // Skip the '$'
    lx_skip(lx);

    // Start with a '_'
    if (lx->_char != '_') {

        lx->_token->type = TOKEN_INVALID;

        return;
    }

    // Skip the '_'
    lx_skip(lx);

    // Only upper alpha characters
    while(isalpha(lx->_char)) {

        // Only UPPER characters
        if (islower(lx->_char)) {

            lx->_token->type = TOKEN_INVALID;

            return;
        }

        lx_addChar(lx, lx->_char);

        lx_skip(lx);
    }

    // End with a '_'
    if (lx->_char != '_')
        lx->_token->type = TOKEN_INVALID;

    lx_skip(lx);

    // ALWAYS need to have a charracter
    if (lx->_token->value_s == 0)
        lx->_token->type = TOKEN_INVALID;

    // Never can have a digit or text after, only symbols
    if (isalpha(lx->_char)) 
        lx->_token->type = TOKEN_INVALID;
}

void lx_getNumber(Lexer *lx) {
    
    while(isdigit(lx->_char)) {

        lx_addChar(lx, lx->_char);

        lx_skip(lx);

        // A number can't be the end of file by the language rules and syntax
        if (lx->_char == EOF) {

            lx->_token->type = TOKEN_INVALID;

            return;
        }
    }

    // By the language rules and syntax, a digit cannot be followed by a identifier or keyword
    // Exemple of invalid:
    //  90787hello (hello is a identifier or keyword)
    // Exemple of correct (for Lexer, but is not for semantic/syntax analyser):
    //  90787 hello (hello is a identifier or keyword)
    if (isalpha(lx->_char) || lx->_char == '_') 
        lx->_token->type = TOKEN_INVALID;
}

void lx_getIdentifier(Lexer *lx) {

    lx_getWord(lx);

    lx->_token->type = TOKEN_IDENTIFIER;
}

void lx_getWord(Lexer *lx) {

    while(isalnum(lx->_char) || lx->_char == '_') {

        lx_addChar(lx, lx->_char);

        lx_skip(lx);

        // A word can't be the end of file by the language rules and syntax
        if (lx->_char == EOF) {

            lx->_token->type = TOKEN_INVALID;

            return;
        }
    }
}

void lx_getSymbol(Lexer *lx) {

    switch(lx->_char) {
        case '=':
            lx_skip(lx);

            if (lx->_char == '=')
                lx->_token->type = TOKEN_EQUALS;
            else {
                return;
                lx->_token->type = TOKEN_RECEIVE;
            }
        break;
        case ';':
            lx->_token->type = TOKEN_SEMICOLON;
        break;
    }

    lx_skip(lx);
}
