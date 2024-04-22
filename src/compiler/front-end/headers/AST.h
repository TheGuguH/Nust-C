#ifndef AST_H
#define AST_H

#include "symbol_table.h"

typedef struct AST {
    Token *_token;
    struct AST *rightBranch;
    struct AST *leftBranch;
} AST;

AST* ast_create(Token *_token);

void ast_free(AST *_ast);

void ast_trim(AST *_ast);

void ast_addLeft(AST *_ast, Token *_token);

void ast_addRight(AST *_ast, Token *_token);

#endif
