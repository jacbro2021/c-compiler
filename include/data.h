// Global variables

#ifndef extern_
 #define extern_ extern
#endif

extern_ int     g_line;
extern_ int     g_putback;
extern_ FILE    *g_infile;
extern_ FILE    *g_outfile;
extern_ Token   g_token;
extern_ char    g_text[TEXT_LEN + 1];
extern_ SymbolTable g_sym[NUM_SYMBOLS];