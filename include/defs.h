// Struct and enum definitions.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXT_LEN 512
#define NUM_SYMBOLS 1024
#define NOREG -1

// Arranged from low to high precedence.
enum {
    T_EOF,
    T_PLUS, T_MINUS,
    T_STAR, T_SLASH,
    T_EQ, T_NEQ,
    T_GT, T_LT, T_GE, T_LE,
    T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,
    // No precedence
    T_INTLIT, T_SEMI, T_ASSIGN, T_IDENT,
    T_PRINT, T_INT, T_IF, T_ELSE,
};

typedef struct _Token {
    int type;
    int int_value;
} Token;

enum {
    A_ADD = 1, A_SUBTRACT,
    A_MULTIPLY, A_DIVIDE,
    A_EQ, A_NEQ,
    A_LT, A_GT, A_LE, A_GE,
    A_INTLIT, 
    A_IDENT, A_LVIDENT, A_ASSIGN,
    A_PRINT, A_GLUE, A_IF,
};

typedef struct _ASTNode ASTNode;
struct _ASTNode {
    int op;
    ASTNode *left;
    ASTNode *mid;
    ASTNode *right;
    union {
        int int_value;
        int id;
    } v;
};

typedef struct _SymbolTable {
    char *name;
} SymbolTable;