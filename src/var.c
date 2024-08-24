
#include "var.h"

var_sg *var_sg_i(size_t size) {
    var_sg *sg = calloc(1, sizeof(var_sg) + size * sizeof(char));
    sg->size = size;
    return sg;
}

var_sg *var_sg_i_str(const char *const str) {
    size_t len = strlen(str);
    var_sg *sg = var_sg_i(len + sizeof(char));
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

var_sg *var_sg_cnct_sg_sg(const var_sg *const l, const var_sg *const r) {
    var_sg *sg = var_sg_i(l->len + r->len + sizeof(char));
    memcpy(sg->str, l->str, l->len * sizeof(char));
    sg->len += l->len;
    memcpy(sg->str + l->len * sizeof(char), r->str, r->len * sizeof(char));
    sg->len += r->len;
    return sg;
}

void var_sg_f(var_sg *sg) {
    if (--sg->rc > 0) return;
    free(sg);
}

bool var_zoo_u6(uint64_t v) { return v != 0; }
bool var_zoo_i6(int64_t v) { return v != 0; }

#define VAR_BOP_T(N, OP, T, CT) VAR_FN_BOP_T(N, T, CT) {  return l OP r; }

// add
VAR_BOP_T(add, +, i6, int64_t)
VAR_BOP_T(add, +, u6, uint64_t)

// sub
VAR_BOP_T(sub, -, i6, int64_t)
VAR_BOP_T(sub, -, u6, uint64_t)

#ifndef NUM_TO_SG_SIZE
    #define NUM_TO_SG_SIZE 30
#endif

// TODO check if output fails
#define INT_TO_SG(FMT, ARG)  var_sg *sg = var_sg_i(NUM_TO_SG_SIZE); \
    sg->len = (size_t) snprintf(sg->str, sg->size, FMT, ARG); \
    return sg

var_sg *var_u6_sg(uint64_t u6) { INT_TO_SG("%lu", u6); }

var_sg *var_i6_sg(uint64_t i6) { INT_TO_SG("%ld", i6); }

bool var_not(bool v) { return !v; }
