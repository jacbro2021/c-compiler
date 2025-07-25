#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int parse_type(int token_type);

void var_declaration() {
   int id, type;

   type = parse_type(g_token.type);
   scan(&g_token);

   ident();
   id = add_glob(g_text, type, S_VARIABLE);
   gen_glob_sym(id);
   semi();
}

ASTNode *function_declaration() {
   int nameIndex;
   ASTNode *tree;

   match(T_VOID, "void");
   ident();
   nameIndex = add_glob(g_text, P_VOID, S_FUNCTION);
   lparen();
   rparen();
   tree = compound_statement();

   return make_ast_unary(A_FUNCTION, P_VOID, tree, nameIndex);
}

static int parse_type(int token_type) {
   switch (token_type) {
   case T_CHAR:
      return P_CHAR;

   case T_INT:
      return P_INT;

   case T_VOID:
      return P_VOID;
   }
   fatal_int("illegal type, token", token_type);
}