#include "../include/defs.h"
#define extern_
#include "../include/data.h"
#undef extern_
#include "../include/decl.h"
#include <errno.h>

static void init();
static void usage(char *s);
static void scanFile();

int main(int argc, char** argv) {
    if (argc != 2) usage(argv[0]);
    init();

    if ((g_infile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open infile %s: %s", argv[1], strerror(errno));
        exit(1);
    }

    // TODO: Allow user to pass in outfile
    if ((g_outfile = fopen("out.s", "w")) == NULL) {
        fprintf(stderr, "Unable to open outfile out.s: %s", strerror(errno));
        exit(1);
    }

    scan(&g_token);
    gen_preamble();
    statements();
    gen_postamble();
    fclose(g_outfile);
    fclose(g_infile);

    return EXIT_SUCCESS;
}

static void init() {
    g_line = 1;
    g_putback = '\n';
}

static void usage(char *s) {
    fprintf(stderr, "usage: %s <infile>\n", s);
    exit(1);
}