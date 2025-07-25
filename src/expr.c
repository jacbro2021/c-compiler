#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static ASTNode *primary();
static int token_to_operator(int type);
static int op_precedence(int token_type);

// Precedence for each token type
static int operator_precedence[] = {
    0,                  // EOF
    10, 10,             // ADD, SUB 
    20, 20,             // MUL, DIV
    30, 30,             // EQ, NEQ 
    40, 40, 40, 40,     // LT, GT, LE, GE 
};
    
ASTNode *binary_expression(int ptp) {
    ASTNode *left, *right;
    int left_type, right_type;
    int token_type;

    left = primary();
    token_type = g_token.type; 
    if (g_token.type == T_SEMI || g_token.type == T_RPAREN) {
        return left;
    }

    while (op_precedence(token_type) > ptp) {
        scan(&g_token);
        right = binary_expression(operator_precedence[token_type]);

        left_type = left->type;
        right_type = right->type;
        if (!type_compatible(&left_type, &right_type, 0)) {
            fatal("Incompatible types");
        }

        if (left_type) {
            make_ast_unary(left_type, right->type, left, 0);
        } 
        if (right_type) {
            make_ast_unary(right_type, left->type, right, 0);
        }

        left = make_ast_node(token_to_operator(token_type), left->type, left, NULL, right, 0);
        token_type = g_token.type;
        if (g_token.type == T_SEMI || g_token.type == T_RPAREN) {
            return left;
        }
    }

    return left;
}

// parse a primary factor.
static ASTNode *primary() {
    ASTNode *n;
    int id;

    switch (g_token.type) {
        case T_INTLIT:
            if (g_token.int_value >= 0 && g_token.int_value < 256) {
                n = make_ast_leaf(A_INTLIT, P_CHAR, g_token.int_value);
            } else {
                n = make_ast_leaf(A_INTLIT, P_INT, g_token.int_value);
            }
            break;

        case T_IDENT:
            id = find_glob(g_text);
            if (id == -1) { fatal_str("Unknown variable", g_text); }  
            n = make_ast_leaf(A_IDENT, g_sym[id].type, id);
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
        case T_EQ:
            return A_EQ;
        case T_NEQ:
            return A_NEQ;
        case T_LT:
            return A_LT;
        case T_GT:
            return A_GT;
        case T_LE:
            return A_LE;
        case T_GE:
            return A_GE;
        default:
            fprintf(stderr, "Unknown token in token_to_operator() on line %d\n", g_line);
            exit(1);
    }
}

static int op_precedence(int token_type) {
    int prec = operator_precedence[token_type];

    if (prec == 0) {
        fatal_int("syntax error, token", token_type);
    }

    return prec;
}