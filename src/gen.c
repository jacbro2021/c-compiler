#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

int gen_ast(ASTNode *n, int reg) {
    int left_reg, right_reg;

    if (n->left) left_reg = gen_ast(n->left, -1);
    if (n->right) right_reg = gen_ast(n->right, left_reg);

    switch (n->op) {
        case A_ADD:
            return cg_add(left_reg, right_reg);
        case A_SUBTRACT:
            return cg_sub(left_reg, right_reg);
        case A_MULTIPLY:
            return cg_mult(left_reg, right_reg);
        case A_DIVIDE:
            return cg_div(left_reg, right_reg);
        case A_INTLIT:
            return cg_load_int(n->v.int_value);
        case A_IDENT:
            return cg_load_glob(g_sym[n->v.id].name);
        case A_LVIDENT:
            return cg_store_glob(reg, g_sym[n->v.id].name);
        case A_ASSIGN:
            return right_reg;
        default:
            fatal_int("Unknown AST operator", n->op);
    }
}

void gen_preamble() {
    cg_preamble();
}

void gen_postamble() {
    cg_postamble();
}

void gen_free_regs() {
    free_all_registers();
}

void gen_print_int(int r) {
    cg_print_int(r);
}

void gen_glob_sym(char *s) {
    cg_glob_sym(s);
}