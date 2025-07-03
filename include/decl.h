// Function protoypes.

int scan(Token *t);
ASTNode *make_ast_node(int op, ASTNode *left, ASTNode *mid, ASTNode *right, int int_value);
ASTNode *make_ast_leaf(int op, int int_value);
ASTNode *make_ast_unary(int op, ASTNode *left, int int_value);
ASTNode *binary_expression();

ASTNode *compound_statement();

int interpret_ast(ASTNode *n);

int gen_ast(ASTNode *n, int reg, int parent_ast_op);
void gen_preamble();
void gen_postamble();
void gen_function_preamble(char *name);
void gen_function_postamble();
void gen_free_regs();
void gen_print_int(int r);
void gen_glob_sym(char *sym);

void free_all_registers();
void cg_preamble();
void cg_postamble();
void cg_function_preamble(char *name);
void cg_function_postamble();
int cg_load_int(int value);
int cg_load_glob(char *identifier);
int cg_add(int r1, int r2);
int cg_sub(int r1, int r2);
int cg_mult(int r1, int r2);
int cg_div(int r1, int r2);
void cg_print_int(int r);
int cg_store_glob(int register, char* identifier);
void cg_glob_sym(char *sym);
int cg_compare_and_set(int ast_op, int r1, int r2);
int cg_compare_and_jump(int ast_op, int r1, int r2, int label);
void cg_jump(int l);
void cg_label(int l);

void match(int i, char *what);
void semi();
void ident();
void lbrace();
void rbrace();
void lparen();
void rparen();

void var_declaration();
ASTNode *function_declaration();

int find_glob(char *s);
int add_glob(char *name);

void fatal(char *s);
void fatal_str(char *s1, char *s2);
void fatal_int(char *s, int d);
void fatal_char(char *s, char c);