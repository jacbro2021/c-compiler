#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

void var_declaration() {
   match(T_INT, "int");
   ident();
   add_glob(g_text);
   gen_glob_sym(g_text);
   semi();
}

ASTNode *function_declaration() {
   int nameIndex;
   ASTNode *tree;

   match(T_VOID, "void");
   ident();
   nameIndex = add_glob(g_text);
   lparen();
   rparen();
   tree = compound_statement();

   return make_ast_unary(A_FUNCTION, tree, nameIndex);
}