#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int alloc_register();
static void free_register(int r);
static int cg_compare(int r1, int r2, char *how);

static int free_reg[4];
static char *reg_list[4] = { "%r8", "%r9", "%r10", "%r11" };
static char *breg_list[4] = { "%r8b", "%r9b", "%r10b", "%r11b" };

void free_all_registers() {
    free_reg[0] = 1;
    free_reg[1] = 1;
    free_reg[2] = 1;
    free_reg[3] = 1;
}

void cg_preamble() {
    free_all_registers();
    fputs(
        "\t.section\t.note.GNU-stack,\"\",@progbits\n"
        "\t.text\n"
        ".LC0:\n"
        "\t.string\t\"%d\\n\"\n"
        "printint:\n"
        "\tpushq\t%rbp\n"
        "\tmovq\t%rsp, %rbp\n"
        "\tsubq\t$16, %rsp\n"
        "\tmovl\t%edi, -4(%rbp)\n"
        "\tmovl\t-4(%rbp), %eax\n"
        "\tmovl\t%eax, %esi\n"
        "\tleaq	.LC0(%rip), %rdi\n"
        "\tmovl	$0, %eax\n"
        "\tcall	printf@PLT\n"
        "\tnop\n"
        "\tleave\n"
        "\tret\n"
        "\n",
    g_outfile);
}

void cg_postamble() {
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
    g_outfile);
}

void cg_function_preamble(char *name) {
    fprintf(g_outfile,
        "\t.text\n"
        "\t.globl\t%s\n"
        "\t.type\t%s, @function\n"
        "%s:\n"
        "\tpushq\t%%rbp\n"
        "\tmovq\t%%rsp, %%rbp\n",
        name, name, name);
}

void cg_function_postamble() {
    fputs("\tmovl $0, %eax\n"
          "\tpopq\t%rbp\n"
          "\tret\n",
          g_outfile);
}

int cg_load_int(int value) {
    int r = alloc_register();
    fprintf(g_outfile, "\tmovq\t$%d, %s\n", value, reg_list[r]);
    return r;
}

int cg_load_glob(int id) {
    int r = alloc_register();

    if (g_sym[id].type == P_CHAR) {
        fprintf(g_outfile, "\tmovzbq\t%s(\%%rip), %s\n", g_sym[id].name, reg_list[r]);
    } else if (g_sym[id].type == P_INT) {
        fprintf(g_outfile, "\tmovq\t%s(\%%rip), %s\n", g_sym[id].name, reg_list[r]);
    } else {
        fatal("Unrecognized primitive type in cg_load_glob");
    }

    return r;
}

int cg_add(int r1, int r2) {
    fprintf(g_outfile, "\taddq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    free_register(r2);
    return r1;
}

int cg_sub(int r1, int r2) {
    fprintf(g_outfile, "\tsubq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    free_register(r2);
    return r1;
}

int cg_mult(int r1, int r2) { 
    fprintf(g_outfile, "\timulq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    free_register(r2);
    return r1;
} 

int cg_div(int r1, int r2) {
    fprintf(g_outfile, "\tmovq\t%s,%%rax\n", reg_list[r1]);
    fprintf(g_outfile, "\tcqo\n");
    fprintf(g_outfile, "\tidivq\t%s\n", reg_list[r2]);
    fprintf(g_outfile, "\tmovq\t%%rax,%s\n", reg_list[r1]);
    free_register(r2);
    return r1;
}

void cg_print_int(int r) {
    fprintf(g_outfile, "\tmovq\t%s, %%rdi\n", reg_list[r]);
    fprintf(g_outfile, "\tcall\tprintint\n");
    free_register(r);
}

int cg_store_glob(int r, int id) {
    if (g_sym[id].type == P_CHAR) {
        fprintf(g_outfile, "\tmovb\t%s, %s(\%%rip)\n", breg_list[r], g_sym[id].name);
    } else if (g_sym[id].type == P_INT) {
        fprintf(g_outfile, "\tmovq\t%s, %s(\%%rip)\n", reg_list[r], g_sym[id].name);
    } else {
        fatal("Unrecognized primitive in cg_store_glob");
    }

    return r;
}

void cg_glob_sym(int id) {
    if (g_sym[id].type == P_CHAR) {
        fprintf(g_outfile, "\t.comm\t%s,1,1\n", g_sym[id].name);
    } else if (g_sym[id].type == P_INT) {
        fprintf(g_outfile, "\t.comm\t%s,8,8\n", g_sym[id].name);
    } else {
        fatal("Unrecognized primitive type in cg_glob_sym");
    }
}

static char *cmp_list[] = { "sete", "setne", "setl", "setg", "setle", "setge" };
int cg_compare_and_set(int ast_op, int r1, int r2) {
    if (ast_op < A_EQ || ast_op > A_GE) {
        fatal("Invalid AST operation in cg_compare_and_set");
    }

    fprintf(g_outfile, "\tcmpq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    fprintf(g_outfile, "\t%s\t%s\n", cmp_list[ast_op - A_EQ], breg_list[r2]);
    fprintf(g_outfile, "\tmovzbq\t%s, %s\n", breg_list[r2], reg_list[r2]);
    free_register(r1);
    return r2;
}

static char *inverted_cmp_list[] = { "jne", "je", "jge", "jle", "jg", "jl" };
int cg_compare_and_jump(int ast_op, int r1, int r2, int label) {
    if (ast_op < A_EQ || ast_op > A_GE) {
        fatal("Invalid AST operation in cg_compare_and_jump");
    }

    fprintf(g_outfile, "\tcmpq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    fprintf(g_outfile, "\t%s\tL%d\n", inverted_cmp_list[ast_op - A_EQ], label);
    free_all_registers();
    return NOREG;
}

void cg_jump(int l) {
    fprintf(g_outfile, "\tjmp\tL%d\n", l);
}

void cg_label(int l) {
    fprintf(g_outfile, "L%d:\n", l);
}

int cg_widen_glob(int r, int old_type, int new_type) {
    return r;
}

static int alloc_register() {
    for (int i = 0; i < 4; i++) {
        if (free_reg[i]) {
            free_reg[i] = 0;
            return i;
        }
    }
    fatal("Ran out of registers");
}

static void free_register(int r) {
    if (free_reg[r] != 0) {
        fatal_int("Error freeing register", r);
    }

    free_reg[r] = 1;
}

static int cg_compare(int r1, int r2, char *how) {
    fprintf(g_outfile, "\tcmpq\t%s, %s\n", reg_list[r2], reg_list[r1]);
    fprintf(g_outfile, "\t%s\t%s\n", how, breg_list[r2]);
    fprintf(g_outfile, "\tandq\t$255, %s\n", reg_list[r2]);
    free_register(r1);
    return r2;
}