#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

ASTNode *make_ast_node(int op, int type, ASTNode *left, ASTNode *mid, ASTNode *right, int int_value) {
    ASTNode *n;

    n = (ASTNode*)malloc(sizeof(ASTNode));
    if (n == NULL) { fatal("unable to malloc in mkastnode()"); }

    n->op = op;
    n->type = type;
    n->left = left;
    n->mid = mid;
    n->right = right;
    n->v.int_value = int_value;

    return n;
}

ASTNode *make_ast_leaf(int op, int type, int int_value) {
    return make_ast_node(op, type, NULL, NULL, NULL, int_value);
}

ASTNode *make_ast_unary(int op, int type, ASTNode *left, int int_value) {
    return make_ast_node(op, type, left, NULL, NULL, int_value);
}