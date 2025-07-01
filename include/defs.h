// Struct and enum definitions.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXT_LEN 512
#define NUM_SYMBOLS 1024

// Arranged from low to high precedence.
enum {
    T_EOF,
    T_PLUS, T_MINUS,
    T_STAR, T_SLASH,
    T_EQ, T_NEQ,
    T_GT, T_LT, T_GE, T_LE,
    // No precedence
    T_INTLIT, T_SEMI, T_ASSIGN, T_IDENT,
    T_PRINT, T_INT,
};

typedef struct _Token {
    int type;
    int int_value;
} Token;

enum {
    A_ADD, A_SUBTRACT,
    A_MULTIPLY, A_DIVIDE,
    A_EQ, A_NEQ,
    A_LT, A_LE, A_GT, A_GE,
    A_INTLIT, 
    A_IDENT, A_LVIDENT, A_ASSIGN,
};

typedef struct _ASTNode ASTNode;
struct _ASTNode {
    int op;
    ASTNode *left;
    ASTNode *right;
    union {
        int int_value;
        int id;
    } v;
};

typedef struct _SymbolTable {
    char *name;
} SymbolTable;