#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

ASTNode *make_ast_node(int op, ASTNode *left, ASTNode *right, int int_value) {
    ASTNode *n;

    n = (ASTNode*)malloc(sizeof(ASTNode));
    if (n == NULL) { fatal("unable to malloc in mkastnode()"); }

    n->op = op;
    n->left = left;
    n->right = right;
    n->v.int_value = int_value;

    return n;
}

ASTNode *make_ast_leaf(int op, int int_value) {
    return make_ast_node(op, NULL, NULL, int_value);
}

ASTNode *make_ast_unary(int op, ASTNode *left, int int_value) {
    return make_ast_node(op, left, NULL, int_value);
}