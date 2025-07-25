#include "../include/defs.h"
#include "../include/data.h"
#include "../include/decl.h"

int type_compatible(int *lhs, int *rhs, int only_right) {
    if (*lhs == P_VOID || *rhs == P_VOID) { return 0; }

    if (*lhs == *rhs) {
        *lhs = 0;
        *rhs = 0;
        return 1;
    }

    if (*lhs == P_CHAR && *rhs == P_INT) {
        *lhs = A_WIDEN;
        *rhs = 0;
        return 1;
    } 

    if (*lhs == P_INT && *rhs == P_CHAR) {
        if (only_right) { return 0; }

        *lhs = 0;
        *rhs = A_WIDEN;
        return 1;
    }

    *lhs = 0;
    *rhs = 0;
    return 1;
}