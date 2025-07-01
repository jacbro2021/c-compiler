#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int alloc_register();
static void free_register(int r);

static int free_reg[4];
static char *reg_list[4] = { "%r8", "%r9", "%r11", "%r11"};

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
        "\n"
        "\t.globl\tmain\n"
        "\t.type\tmain, @function\n"
        "main:\n"
        "\tpushq\t%rbp\n"
        "\tmovq	%rsp, %rbp\n",
    g_outfile);
}

void cg_postamble() {
    fputs(
        "\tmovl	$0, %eax\n"
        "\tpopq	%rbp\n"
        "\tret\n",
    g_outfile);
}

int cg_load_int(int value) {
    int r = alloc_register();
    fprintf(g_outfile, "\tmovq\t$%d, %s\n", value, reg_list[r]);
    return r;
}

int cg_load_glob(char *identifier) {
    int r = alloc_register();
    fprintf(g_outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reg_list[r]);
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

int cg_store_glob(int r, char *identifier) {
    fprintf(g_outfile, "\tmovq\t%s, %s(\%%rip)\n", reg_list[r], identifier);
    return r;
}

void cg_glob_sym(char *sym) {
    fprintf(g_outfile, "\t.comm\t%s,8,8\n", sym);
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