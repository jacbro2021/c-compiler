#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static ASTNode *single_statement();
static ASTNode *print_statement();
static ASTNode *assignment_statement();
static ASTNode *if_statement();
static ASTNode *while_statement();
static ASTNode *for_statement();

ASTNode *compound_statement() {
    ASTNode *left = NULL;
    ASTNode *tree;

    lbrace();

    while (1) {
        tree = single_statement();

        if (tree) {
            if (tree->op == A_PRINT || tree->op == A_ASSIGN) {
                semi();
            }
        }

        if (tree) {
            if (left == NULL)  {
                left = tree;
            } else {
                left = make_ast_node(A_GLUE, left, NULL, tree, 0);
            }
        }

        if (g_token.type == T_RBRACE) {
            rbrace();
            return left;
        }
    }
}

static ASTNode *single_statement() {
    switch (g_token.type) {
        case T_PRINT:
            return print_statement();

        case T_INT:
            var_declaration();
            return NULL;

        case T_IDENT:
            return assignment_statement();

        case T_IF:
            return if_statement();

        case T_WHILE:
            return while_statement();

        case T_FOR:
            return for_statement();

        default:
            fatal_int("Syntax error, token", g_token.type);
    } 
}

static ASTNode *print_statement() {
    ASTNode *tree;

    match(T_PRINT, "print");
    tree = binary_expression(0);
    tree = make_ast_unary(A_PRINT, tree, 0);

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

static ASTNode *for_statement() {
    ASTNode *assignment, *cond, *iteration_assignment, *body;

    match(T_FOR, "for");
    lparen();

    assignment = single_statement();
    semi();

    cond = binary_expression(0);
    if (cond->op < A_EQ || cond->op > A_GE) {
        fatal("Bad comparison operator");
    }
    semi();

    iteration_assignment = single_statement();
    rparen();

    body = compound_statement();

    ASTNode *compound_body = make_ast_node(A_GLUE, body, NULL, iteration_assignment, 0);
    ASTNode *while_node = make_ast_node(A_WHILE, cond, NULL, compound_body, 0);
    return make_ast_node(A_GLUE, assignment, NULL, while_node, 0);
}