
#pragma once

#include "kpl.h"

#define RC ssize_t rc

typedef struct {
    RC;
    size_t len, size;
    char str[]; // null term
} var_sg;

typedef struct {
    RC;
    size_t len;
    var *v[];
} var_te;

var_sg *var_sg_i(size_t size);

var_sg *var_sg_i_str(const char *const str);

void var_sg_rc(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

void var_sg_f(var_sg *sg);

typedef union _var {
    int64_t i6;
    uint64_t u6;
    var_sg *sg;
    var_te *te;
} var;

bool var_zoo_u6(uint64_t v);
bool var_zoo_i6(int64_t v);

var_sg *var_u6_sg(uint64_t u6);
var_sg *var_i6_sg(uint64_t i6);

#define VAR_FN_BOP_T(N, T, CT) CT var_##N##_##T(CT l, CT r)

// add
VAR_FN_BOP_T(add, i6, int64_t);
VAR_FN_BOP_T(add, u6, uint64_t);

// sub
VAR_FN_BOP_T(sub, i6, int64_t);
VAR_FN_BOP_T(sub, u6, uint64_t);

bool var_not(bool v);
