#include "headers/syntaxer.h"

#include "headers/lexer.h"
#include "headers/symbol_table.h"

#include "../../headers/error_codes.h"
#include "../../headers/utils.h"
#include "headers/token.h"

#include <stdio.h>

Syntaxer* sx_create(char *fileName, size_t fileName_s, FILE *file, SymbolTable *_symbolTable) {
    Syntaxer *_syntaxer = malloc(sizeof(Syntaxer));

    if (_syntaxer == NULL)
        syPrintError("can't alloc memory for a syntaxer", SY_MEMORY_ALLOC_ERROR);

    _syntaxer->fileName = fileName;
    _syntaxer->fileName_s = fileName_s;
    _syntaxer->_lexer = lx_create(file);
    _syntaxer->_symbolTable = _symbolTable;

    return _syntaxer;
}

void sx_free(Syntaxer *_syntaxer) {
    lx_free(_syntaxer->_lexer);
    _syntaxer->_lexer = NULL;

    st_free(_syntaxer->_symbolTable);
    _syntaxer->_symbolTable = NULL;

    free(_syntaxer);
    _syntaxer = NULL;
}

Token* sx_assert(Syntaxer *_syntaxer, int type) {
    Token *_token = lx_getNextToken(_syntaxer->_lexer);

    if (_token->type != type) {
        char *typeExpected = tk_getValueOfAType(type);
        char *typePassed = tk_getValueOfAType(_token->type);
        char buffer[120];
        sprintf(buffer, "expected token '%s', and not a '%s', in the line %lu", typeExpected, typePassed, _token->line);
        syPrintError(buffer, SY_UNEXPECTED_TOKEN);
    }

    return _token;
}

void sx_getNextAST(Syntaxer *syntaxer) {

}

void sx_getAttribute(Syntaxer *_syntaxer) {

}

void syPrint(char string[]) {
    printf("[Syntaxer]: %s\n", string);
}

void syPrintError(char string[], int errorCode) {
    P_ERROR_FORMAT("[Syntaxer]: ");
}
