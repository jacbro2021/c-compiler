#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

static int skip();
static int next();
static int scan_int(int c);
static int scan_identifier(int c, char *buf, int lim);
static int keyword(char *s);
static int chrpos(char *s, int c);
static int putback(int c);

int scan(Token *t) {
    t->int_value = 0;
    int c, token_type;

    c = skip();

    switch (c) {
        case EOF:
            t->type = T_EOF;
            return 0;

        case '+':
            t->type = T_PLUS;
            break;

        case '-':
            t->type = T_MINUS;
            break;

        case '*':
            t->type = T_STAR;
            break;

        case '/':
            t->type = T_SLASH;
            break;

        case ';':
            t->type = T_SEMI;
            break;

        case '=':
            if ((c = next()) == '=') {
                t->type = T_EQ;
            } else {
                putback(c);
                t->type = T_ASSIGN;
            }
            break;

        case '!':
            if ((c = next()) == '=') {
                t->type = T_NEQ;
            } else {
                fatal_char("Unrecognized character:", c);
            }
            break;

        case '<':
            if ((c = next()) == '=') {
                t->type = T_LE;
            } else {
                putback(c);
                t->type = T_LT;
            }
            break;

        case '>':
            if ((c = next()) == '=') {
                t->type = T_GE;
            } else {
                putback(c);
                t->type = T_GT;
            }
            break;

        case '{':
            t->type = T_LBRACE;
            break;

        case '}':
            t->type = T_RBRACE;
            break;

        case '(':
            t->type = T_LPAREN;
            break;

        case ')':
            t->type = T_RPAREN;
            break;

        default:
            if isdigit(c) {
                t->int_value = scan_int(c);
                t->type = T_INTLIT;
                break;
            } else if (isalpha(c) || c == '_') {
                scan_identifier(c, g_text, TEXT_LEN);
                if (token_type = keyword(g_text)) {
                    t->type = token_type;
                    break;
                }

                t->type = T_IDENT;
                break;
            }

            // Unknown character
            fprintf(stderr, "Unrecognized character %c on line %d\n", c, g_line);
            exit(1);
    }

    return 1;
}

static int skip() {
    int c;

    c = next();
    while(c == ' ' || c == '\n' || c == '\t' || c == '\f' || c == '\r') {
        c = next();
    }

    return c;
}

static int next() {
    int c;

    if (g_putback) {
        c = g_putback;
        g_putback = 0;
        return c;
    }

    c = fgetc(g_infile);
    if (c == '\n') {
        ++g_line;
    }

    return c;
}

static int scan_int(int c) {
    int k, val = 0;

    while((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = next();
    }

    putback(c);
    return val;
}

static int scan_identifier(int c, char *buf, int lim) {
    int i = 0;

    while (isalpha(c) || isdigit(c) || c == '_') {
        if (i == lim - 1) {
            fprintf(stderr, "identifier on line %d too long\n", g_line);
            exit(1);
        }

        buf[i++] = c;
        c = next();
    }

    putback(c);
    buf[i] = '\0';
    return i;
}

static int keyword(char *s) {
    switch (*s) {
    case 'e':
        if (!strcmp(s, "else")) {
            return T_ELSE;
        }
        break;
    
    case 'f':
        if (!strcmp(s, "for")) {
            return T_FOR;
        }
        break;

    case 'i':
        if (!strcmp(s, "int")) {
            return T_INT;
        } 
        if (!strcmp(s, "if")) {
            return T_IF;
        }
        break;

    case 'p':
        if (!strcmp(s, "print")) {
            return T_PRINT;
        }
        break;
    
    case 'v':
        if (!strcmp(s, "void")) {
            return T_VOID;
        }
        break;
    
    case 'w':
        if (!strcmp(s, "while")) {
            return T_WHILE;
        }
        break;
    }

    return 0;
}

static int chrpos(char *s, int c) {
    char *p;

    p = strchr(s, c);
    return (p ? p-s : -1);
}

static int putback(int c) {
    g_putback = c;
}