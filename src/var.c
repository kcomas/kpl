
#include "var.h"

var_sg *var_sg_i(const char *const str) {
    size_t len = strlen(str);
    var_sg *sg = calloc(1, sizeof(var_sg) + sizeof(char) * len + sizeof(char));
    sg->len = len;
    strcpy(sg->str, str);
    return sg;
}

void var_sg_rc(var_sg *const sg) {
    sg->rc++;
}

size_t var_sg_len(var_sg *const sg) {
    return sg->len;
}

const char *var_sg_str(var_sg *const sg) {
    return sg->str;
}

void var_sg_f(var_sg *sg) {
    if (--sg->rc > 0) return;
    free(sg);
}

#define VAR_OP_T(N, OP, T, CT) VAR_FN_T(N, T, CT) { \
    return l OP r; \
}

// add
VAR_OP_T(add, +, i6, int64_t)
VAR_OP_T(add, +, u6, uint64_t)

// sub
VAR_OP_T(sub, -, i6, int64_t)
VAR_OP_T(sub, -, u6, uint64_t)
