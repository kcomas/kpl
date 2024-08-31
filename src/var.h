
#pragma once

#include "kpl.h"

typedef struct _var_te_vr var_te_vr;

#define RC ssize_t rc

typedef struct {
    RC;
    size_t len, size; // len does not inc null term
    char str[]; // null term
} var_sg;

var_sg *var_sg_i(size_t size);

var_sg *var_sg_i_str(const char *const str);

void var_sg_rci(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

var_sg *var_sg_cnct_sg_sg(const var_sg *const l, const var_sg *const r);

var_sg *var_sg_cnct_sg_te_vr(const var_sg *const l, const var_te_vr *const r);

void var_sg_f(var_sg *sg);

typedef union _var {
    int64_t i6;
    uint64_t u6;
    jit_fn *jf;
    var_sg *sg;
    var_te_vr *te, *vr;
} var;

ssize_t var_rcd(var v, type t);

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

// eq
VAR_FN_BOP_T(eq, i6, int64_t);
VAR_FN_BOP_T(eq, u6, uint64_t);

// gt
VAR_FN_BOP_T(gt, i6, int64_t);
VAR_FN_BOP_T(gt, u6, uint64_t);

// lt
VAR_FN_BOP_T(lt, i6, int64_t);
VAR_FN_BOP_T(lt, u6, uint64_t);

bool var_not(bool v);

typedef struct _var_te_vr {
    RC;
    size_t len, size;
    jit_fn *gc;
    var v[];
} var_te_vr;

var_te_vr *var_te_vr_i(size_t size, jit_fn *gc);

var_te_vr *var_te_i(size_t size, jit_fn *gc);

jit_fn *var_te_vr_gc(var_te_vr *const vtv);

var var_te_vr_gidx(var_te_vr *const vtv, size_t idx);

void var_te_vr_sidx(var_te_vr *const vtv, size_t idx, var v);

void var_te_vr_f(var_te_vr *vtv);
