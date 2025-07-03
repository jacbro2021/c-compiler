#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static ASTNode *print_statement();
static ASTNode *assignment_statement();
static ASTNode *if_statement();
static ASTNode *while_statement();

ASTNode *compound_statement() {
    ASTNode *left = NULL;
    ASTNode *tree;

    lbrace();

    while (1) {
        switch (g_token.type) {
        case T_PRINT:
            tree = print_statement();
            break;

        case T_INT:
            var_declaration();
            tree = NULL;
            break;

        case T_IDENT:
            tree = assignment_statement();
            break;

        case T_IF:
            tree = if_statement();
            break;

        case T_WHILE:
            tree = while_statement();
            break;
        
        case T_RBRACE:
            rbrace();
            return left;

        default:
            fatal_int("Syntax error, token", g_token.type);
        } 

        if (tree) {
            if (left == NULL)  {
                left = tree;
            } else {
                left = make_ast_node(A_GLUE, left, NULL, tree, 0);
            }
        }
    }
}

static ASTNode *print_statement() {
    ASTNode *tree;

    match(T_PRINT, "print");
    tree = binary_expression(0);
    tree = make_ast_unary(A_PRINT, tree, 0);
    semi();

    return tree;
}

static ASTNode *assignment_statement() {
    ASTNode *tree, *left, *right;
    int id;

    ident();

    if ((id = find_glob(g_text)) == -1) {
        fatal_str("undeclared identifier", g_text);
    }
    right = make_ast_leaf(A_LVIDENT, id);

    match(T_ASSIGN, "=");

    left = binary_expression(0);
    tree = make_ast_node(A_ASSIGN, left, NULL, right, 0);

    semi();

    return tree;
}

static ASTNode *if_statement() {
    ASTNode *cond, *true_node, *false_node = NULL;

    match(T_IF, "if");
    lparen();
    cond = binary_expression(0);
    rparen();
    if (cond->op < A_EQ || cond->op > A_GE) {
        fatal("Invalid comparison operator");
    }

    true_node = compound_statement();
    
    if (g_token.type == T_ELSE) {
        scan(&g_token);
        false_node = compound_statement();
    }

    return make_ast_node(A_IF, cond, true_node, false_node, 0);
}

static ASTNode *while_statement() {
    ASTNode *cond, *true_node;

    match(T_WHILE, "while");
    lparen();
    cond = binary_expression(0);
    rparen();
    if (cond->op < A_EQ || cond->op > A_GE) {
        fatal("Invalid comparison operator");
    }

    true_node = compound_statement();

    return make_ast_node(A_WHILE, cond, NULL, true_node, 0);
}