#include "headers/AST.h"

#include "../../headers/error_codes.h"

#include <stdio.h>
#include <stdlib.h>

AST* ast_create(Token *_token) {
    AST *_ast = malloc(sizeof(AST));

    if (_ast == NULL) {
        printf("[AST]: can't alloc memory for an AST\nError code: %d\n", AST_MEMORY_ALLOC_ERROR);
        exit(AST_MEMORY_ALLOC_ERROR);
    }

    _ast->_token = _token;
    _ast->leftBranch = NULL;
    _ast->rightBranch = NULL;

    return _ast;
}

void ast_free(AST *_ast) {
    if (_ast == NULL)
        return;

    ast_trim(_ast->leftBranch);
    ast_trim(_ast->rightBranch);

    tk_free(_ast->_token);
    _ast->_token = NULL;

    free(_ast);
    _ast = NULL;
}

void ast_trim(AST *_ast) {
    if (_ast == NULL)
        return;

    if (_ast->leftBranch != NULL)
        ast_trim(_ast->leftBranch);

    if (_ast->rightBranch != NULL)
        ast_trim(_ast->rightBranch);

    _ast->leftBranch = NULL;
    _ast->rightBranch = NULL;
}

void ast_addLeft(AST *_ast, Token *_token) {
    if (_ast->leftBranch != NULL) {
        ast_addLeft(_ast, _token);
        return;
    }

    _ast->leftBranch = ast_create(_token);
}

void ast_addRight(AST *_ast, Token *_token) {
    if (_ast->rightBranch != NULL) {
        ast_addRight(_ast, _token);
        return;
    }

    _ast->rightBranch = ast_create(_token);
}
