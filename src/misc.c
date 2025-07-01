#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

void match(int i, char *what) {
    if (g_token.type == i) {
        scan(&g_token);
    } else {
        fatal_str("Expected", what);
    }
}

void semi() {
    match(T_SEMI, ";");
}

void ident() {
    match(T_IDENT, "identifier");
}

// Error handling functions.
void fatal(char *s) {
    fprintf(stderr, "%s: fatal error on line %d\n", s, g_line);
    exit(1);
}

void fatal_str(char *s1, char* s2) {
    fprintf(stderr, "%s:%s on line %d\n", s1, s2, g_line);
    exit(1);
}

void fatal_int(char *s, int d) {
    fprintf(stderr, "%s:%d on line %d\n", s, d, g_line);
    exit(1);
}

void fatal_char(char *s, char c) {
    fprintf(stderr, "%s:%c on line %d\n", s, c, g_line);
    exit(1);
}

