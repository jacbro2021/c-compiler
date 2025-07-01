#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

// parse a primary factor.
static ASTNode *primary() {
    ASTNode *n;
    int id;

    switch (g_token.type) {
        case T_INTLIT:
            n = make_ast_leaf(A_INTLIT, g_token.int_value);
            break;

        case T_IDENT:
            id = find_glob(g_text);
            if (id == -1) { fatal_str("Unknown variable", g_text); }  
            n = make_ast_leaf(A_IDENT, id);
            break;

        default:
            fprintf(stderr, "Syntax error on line %d", g_line);
            exit(1);
    }

    scan(&g_token);
    return n;
}

// convert token to AST operation
static int token_to_operator(int type) {
    switch(type) {
        case T_PLUS:
            return A_ADD;
        case T_MINUS:
            return A_SUBTRACT;
        case T_STAR:
            return A_MULTIPLY;
        case T_SLASH:
            return A_DIVIDE;
        default:
            fprintf(stderr, "Unknown token in token_to_operator() on line %d\n", g_line);
            exit(1);
    }
}

// Precedence for each token type
static int operator_precedence[] = {0, 10, 10, 20, 20, 0};

static int op_precedence(int token_type) {
    int prec = operator_precedence[token_type];

    if (prec == 0) {
        fatal_int("syntax error, token", token_type);
    }

    return prec;
}

ASTNode *binary_expression(int ptp) {
    ASTNode *left, *right;
    int token_type;

    left = primary();
    token_type = g_token.type; 
    if (g_token.type == T_SEMI) {
        return left;
    }

    while (op_precedence(token_type) > ptp) {
        scan(&g_token);
        right = binary_expression(operator_precedence[token_type]);
        left = make_ast_node(token_to_operator(token_type), left, right, 0);
        token_type = g_token.type;
        if (g_token.type == T_SEMI) {
            return left;
        }
    }

    return left;
}