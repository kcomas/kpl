
#pragma once

#include "kpl.h"
#include "ast.h"
#include "er.h"

typedef struct _var_tsv var_tsv;

var_sg *var_sg_i(al *const a, size_t size);

var_sg *var_sg_i_str(al *const a, const char *const str);

void var_sg_rci(var_sg *const sg);

void var_sg_rcd(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

var_sg *var_sg_cnct_sg_sg(al *const a, const var_sg *const l, const var_sg *const r);

var_sg *var_sg_cnct_sg_tsv(al *const a, const var_sg *const l, const var_tsv *const r);

// var_sg_f in kpl.h

void var_sg_d(var_sg *sg);

void var_sg_er(mod *const m, ast *const a, var_sg *const sg);

typedef union _var {
    int64_t i6;
    uint64_t u6;
    int fd;
    jit_fn *jf;
    var_sg *sg;
    var_tsv *tsv;
    er_itm *ei;
} var;

ssize_t var_rcf(var v, type t);

bool var_zoo_u6(uint64_t v);
bool var_zoo_i6(int64_t v);

var_sg *var_u6_sg(al *const a, uint64_t u6);
var_sg *var_i6_sg(al *const a, uint64_t i6);

#define VAR_FN_BOP_T(N, T, CT) CT var_##N##_##T(CT l, CT r)

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

bool var_not(bool v);

typedef struct _var_tsv {
    RC;
    size_t len, size;
    jit_fn *gc;
    var v[];
} var_tsv; // tuple, struct, vec

var_tsv *var_tsv_i(al *const a, size_t size, jit_fn *gc);

var_tsv *var_ts_i(al *const a, size_t size, jit_fn *gc);

var_tsv *var_ts_fm(al *const a, mod *const m, jit_fn *gc);

void var_tsv_rci(var_tsv *const tsv);

void var_tsv_rcd(var_tsv *const tsv);

jit_fn *var_tsv_gc(var_tsv *const tsv);

var var_tsv_gidx(var_tsv *const tsv, size_t idx);

void var_tsv_sidx(var_tsv *const tsv, size_t idx, var v);

void var_tsv_d(var_tsv *tsv);
