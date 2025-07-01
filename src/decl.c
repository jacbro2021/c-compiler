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