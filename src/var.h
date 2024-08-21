
#pragma once

#include "kpl.h"

#define RC size_t rc

typedef struct {
    RC;
    size_t len;
    char str[]; // null term
} var_sg;

var_sg *var_sg_i(const char *const str);

void var_sg_rc(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

void var_sg_f(var_sg *sg);

typedef union {
    int64_t i6;
    uint64_t u6;
    var_sg *sg;
} var;

#define VAR_FN_T(N, T, CT) CT var_##N##_##T(CT l, CT r)

// add
VAR_FN_T(add, i6, int64_t);
VAR_FN_T(add, u6, uint64_t);

// sub
VAR_FN_T(sub, i6, int64_t);
VAR_FN_T(sub, u6, uint64_t);
