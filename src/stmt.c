#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static void print_statement();
static void assignment_statement();

void statements() {
    while (1) {
        switch (g_token.type) {
        case T_PRINT:
            print_statement();
            break;
        case T_INT:
            var_declaration();
            break;
        case T_IDENT:
            assignment_statement();
            break;
        case T_EOF:
            return;
        default:
            fatal_int("Syntax error, token", g_token.type);
        } 
    }
}

static void print_statement() {
    ASTNode *tree;
    int reg;

    match(T_PRINT, "print");
    tree = binary_expression(0);
    reg = gen_ast(tree, -1);
    gen_print_int(reg);
    gen_free_regs();

    semi();
}

static void assignment_statement() {
    ASTNode *tree, *left, *right;
    int id;

    ident();

    if ((id = find_glob(g_text)) == -1) {
        fatal_str("undeclared identifier", g_text);
    }
    right = make_ast_leaf(A_LVIDENT, id);

    match(T_EQUALS, "=");

    left = binary_expression(0);
    tree = make_ast_node(A_ASSIGN, left, right, 0);

    gen_ast(tree, -1);
    gen_free_regs();

    semi();
}
