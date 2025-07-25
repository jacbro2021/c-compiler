#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int Globs = 0;

static int new_glob();

int find_glob(char *s) {
    for (int i = 0; i < Globs; i++) {
        if (*s == *g_sym[i].name && !strcmp(s, g_sym[i].name)) {
            return i;
        }
    }

    return -1;
}

int add_glob(char *name, int type, int s_type) {
    int i;

    if ((i = find_glob(name)) != -1) {
        return i;
    }

    i = new_glob();
    g_sym[i].name = strdup(name);
    g_sym[i].type = type;
    g_sym[i].s_type = s_type;
    return i;
}

static int new_glob() {
    int p;
    if ((p == Globs++) >= NUM_SYMBOLS) { fatal("Too many symbols"); }
    return p;
}