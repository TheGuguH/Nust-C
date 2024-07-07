#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_ASSIGNMENT,
    NODE_IDENTIFIER,
    NODE_NUMBER
} NodeType;

typedef struct Node {
    NodeType type;
    char *value;
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(NodeType type, const char *value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) {
        perror("Erro ao alocar memória para o nó");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->value = strdup(value);
    node->left = NULL;
    node->right = NULL;
    return node;
}

void freeAST(Node *root) {
    if (root != NULL) {
        freeAST(root->left);
        freeAST(root->right);
        free(root->value);
        free(root);
    }
}

Node *parseAssignment(const char *expression) {
    char variable[100], value[100];
    sscanf(expression, "%s = %s", variable, value);

    Node *assignmentNode = createNode(NODE_ASSIGNMENT, "=");
    assignmentNode->left = createNode(NODE_IDENTIFIER, variable);
    assignmentNode->right = createNode(NODE_NUMBER, value);

    return assignmentNode;
}

void printAST(Node *root) {
    if (root != NULL) {
        printf("(");
        switch (root->type) {
            case NODE_ASSIGNMENT:
                printf("Assignment: ");
                break;
            case NODE_IDENTIFIER:
                printf("Identifier: ");
                break;
            case NODE_NUMBER:
                printf("Number: ");
                break;
        }
        printf("%s", root->value);
        printAST(root->left);
        printAST(root->right);
        printf(")");
    }
}

int main() {
    const char *expression = "x = 20;";
    Node *root = parseAssignment(expression);

    printf("AST para a expressão \"%s\":\n", expression);
    printAST(root);
    printf("\n");

    freeAST(root);

    return 0;
}