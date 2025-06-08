
#pragma once

#include "kpl.h"

typedef struct _var_te_vr var_te_vr;

#define RC ssize_t rc

typedef struct {
    RC;
    size_t len, size; // len does not inc null term
    char str[]; // null term
} var_sg;

var_sg *var_sg_i(al *const a, size_t size);

var_sg *var_sg_i_str(al *const a, const char *const str);

void var_sg_rci(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

var_sg *var_sg_cnct_sg_sg(al *const a, const var_sg *const l, const var_sg *const r);

var_sg *var_sg_cnct_sg_te_vr(al *const a, const var_sg *const l, const var_te_vr *const r);

void var_sg_f(var_sg *sg);

void var_sg_d(var_sg *sg);

typedef union _var {
    int64_t i6;
    uint64_t u6;
    int fd;
    jit_fn *jf;
    var_sg *sg;
    var_te_vr *te, *vr;
} var;

ssize_t var_rcd(var v, type t);

bool OO0 var_zoo_u6(uint64_t v);
bool OO0 var_zoo_i6(int64_t v);

var_sg *var_u6_sg(al *const a, uint64_t u6);
var_sg *var_i6_sg(al *const a, uint64_t i6);

#define VAR_FN_BOP_T(N, T, CT) CT OO0 var_##N##_##T(CT l, CT r)

// add
VAR_FN_BOP_T(add, bl, bool);
VAR_FN_BOP_T(add, i6, int64_t);
VAR_FN_BOP_T(add, u6, uint64_t);

// sub
VAR_FN_BOP_T(sub, bl, bool);
VAR_FN_BOP_T(sub, i6, int64_t);
VAR_FN_BOP_T(sub, u6, uint64_t);

// mul
VAR_FN_BOP_T(mul, bl, bool);
VAR_FN_BOP_T(mul, i6, int64_t);
VAR_FN_BOP_T(mul, u6, uint64_t);

// eq
VAR_FN_BOP_T(eq, bl, bool);
VAR_FN_BOP_T(eq, i6, int64_t);
VAR_FN_BOP_T(eq, u6, uint64_t);

// gt
VAR_FN_BOP_T(gt, bl, bool);
VAR_FN_BOP_T(gt, i6, int64_t);
VAR_FN_BOP_T(gt, u6, uint64_t);

// lt
VAR_FN_BOP_T(lt, bl, bool);
VAR_FN_BOP_T(lt, i6, int64_t);
VAR_FN_BOP_T(lt, u6, uint64_t);

// or
VAR_FN_BOP_T(or, bl, bool);
VAR_FN_BOP_T(or, i6, int64_t);
VAR_FN_BOP_T(or, u6, uint64_t);

bool OO0 var_not(bool v);

typedef struct _var_te_vr {
    RC;
    size_t len, size;
    jit_fn *gc;
    var v[];
} var_te_vr;

var_te_vr *var_te_vr_i(al *const a, size_t size, jit_fn *gc);

var_te_vr *var_te_i(al *const a, size_t size, jit_fn *gc);

jit_fn *var_te_vr_gc(var_te_vr *const vtv);

var var_te_vr_gidx(var_te_vr *const vtv, size_t idx);

void var_te_vr_sidx(var_te_vr *const vtv, size_t idx, var v);

void var_te_vr_d(var_te_vr *vtv);
