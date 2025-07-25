#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int gen_if_ast();
static int gen_while_ast();
static int gen_label();

int gen_ast(ASTNode *n, int reg, int parent_ast_op) {
    int left_reg, right_reg;

    // special AST handling for if and glue
    switch (n->op) {
        case A_IF:
            return gen_if_ast(n);

        case A_WHILE:
            return gen_while_ast(n);

        case A_GLUE:
            gen_ast(n->left, NOREG, n->op);
            gen_free_regs();
            gen_ast(n->right, NOREG, n->op);
            gen_free_regs();
            return NOREG;
        
        case A_FUNCTION:
            gen_function_preamble(g_sym[n->v.id].name);
            gen_ast(n->left, NOREG, n->op);
            gen_function_postamble();
            return NOREG;
    }

    // generic AST handling
    if (n->left) left_reg = gen_ast(n->left, NOREG, n->op);
    if (n->right) right_reg = gen_ast(n->right, left_reg, n->op);

    switch (n->op) {
        case A_ADD:
            return cg_add(left_reg, right_reg);

        case A_SUBTRACT:
            return cg_sub(left_reg, right_reg);

        case A_MULTIPLY:
            return cg_mult(left_reg, right_reg);

        case A_DIVIDE:
            return cg_div(left_reg, right_reg);

        case A_EQ:
        case A_NEQ:
        case A_LT:
        case A_GT:
        case A_LE:
        case A_GE:
            if (parent_ast_op == A_IF || parent_ast_op == A_WHILE) {
                return cg_compare_and_jump(n->op, left_reg, right_reg, reg);
            } else {
                return cg_compare_and_set(n->op, left_reg, right_reg);
            }

        case A_INTLIT:
            return cg_load_int(n->v.int_value);

        case A_IDENT:
            return cg_load_glob(n->v.id);

        case A_LVIDENT:
            return cg_store_glob(reg, n->v.id);

        case A_ASSIGN:
            return right_reg;

        case A_PRINT:
            gen_print_int(left_reg);
            gen_free_regs();
            return NOREG;
        
        case A_WIDEN:
            return cg_widen_glob(left_reg, n->left->type, n->type);

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

void gen_function_preamble(char *name) {
    cg_function_preamble(name);
}

void gen_function_postamble() {
    cg_function_postamble();
}

void gen_free_regs() {
    free_all_registers();
}

void gen_print_int(int r) {
    cg_print_int(r);
}

void gen_glob_sym(int id) {
    cg_glob_sym(id);
}

static int gen_if_ast(ASTNode *n) {
    int false_label, end_label;

    false_label = gen_label();
    if (n->right) {
        end_label = gen_label();
    }

    gen_ast(n->left, false_label, n->op);
    gen_free_regs();

    gen_ast(n->mid, NOREG, n->op);
    gen_free_regs();

    if (n->right) {
        cg_jump(end_label);
    }

    cg_label(false_label);

    if (n->right) {
        gen_ast(n->right, NOREG, n->op);
        gen_free_regs();
        cg_label(end_label);
    }

    return NOREG;
}

static int gen_while_ast(ASTNode *n) {
    int start_label, end_label;

    start_label = gen_label();
    end_label = gen_label();

    cg_label(start_label);

    gen_ast(n->left, end_label, n->op);
    gen_free_regs();

    gen_ast(n->right, NOREG, n->op);
    gen_free_regs();
    cg_jump(start_label);

    cg_label(end_label);

    return NOREG;
}

static int gen_label() {
    static int id=1;
    return id++;
}