
#pragma once

#include "kpl.h"
#include "ast.h"
#include "tbl.h"
#include "er.h"

typedef struct _var_tsv var_tsv;

typedef struct _var_hh var_hh;

typedef struct _var_fd var_fd;

var_sg *var_sg_i(al *const a, size_t size);

var_sg *var_sg_i_str(al *const a, const char *const str);

void var_sg_rci(var_sg *const sg);

void var_sg_rcd(var_sg *const sg);

size_t var_sg_len(var_sg *const sg);

const char *var_sg_str(var_sg *const sg);

var_sg *var_sg_cnct_sg_sg(al *const a, const var_sg *const l, const var_sg *const r);

var_sg *var_sg_cnct_sg_tsv(al *const a, const var_sg *const l, const var_tsv *const r);

void var_sg_d(var_sg *sg);

// var_sg_f in kpl.h

void var_sg_er(mod *const m, ast *const a, var_sg *const sg);

void er_rt(mod *const m, ast *const a, er_itm *const ei);

typedef union _var {
    int64_t i6;
    uint64_t u6;
    double f6;
    int fd;
    void *vd;
    jit_fn *jf;
    var_sg *sg;
    var_tsv *tsv;
    var_hh *hh;
    er_itm *ei;
    var_td *td;
} var;

ssize_t var_rcf(var v, type t);

bool var_nvd(var v);

bool var_zoo_u6(uint64_t v);
bool var_zoo_i6(int64_t v);
bool var_zoo_er(er_itm *const ei);

var_sg *var_u6_sg(al *const a, uint64_t u6);
var_sg *var_i6_sg(al *const a, int64_t i6);
var_sg *var_f6_sg(al *const a, double f6);

#define VAR_FN_BOP_T(N, T, CT) CT var_##N##_##T(CT l, CT r)

// add
VAR_FN_BOP_T(add, bl, bool);
VAR_FN_BOP_T(add, i6, int64_t);
VAR_FN_BOP_T(add, u6, uint64_t);
VAR_FN_BOP_T(add, f6, double);

// sub
VAR_FN_BOP_T(sub, bl, bool);
VAR_FN_BOP_T(sub, i6, int64_t);
VAR_FN_BOP_T(sub, u6, uint64_t);
VAR_FN_BOP_T(sub, f6, double);

// mul
VAR_FN_BOP_T(mul, bl, bool);
VAR_FN_BOP_T(mul, i6, int64_t);
VAR_FN_BOP_T(mul, u6, uint64_t);
VAR_FN_BOP_T(mul, f6, double);

// div
VAR_FN_BOP_T(div, bl, bool);
VAR_FN_BOP_T(div, i6, int64_t);
VAR_FN_BOP_T(div, u6, uint64_t);
VAR_FN_BOP_T(div, f6, double);

// eq
VAR_FN_BOP_T(eq, bl, bool);
VAR_FN_BOP_T(eq, i6, int64_t);
VAR_FN_BOP_T(eq, u6, uint64_t);
VAR_FN_BOP_T(eq, f6, double);

// gt
VAR_FN_BOP_T(gt, bl, bool);
VAR_FN_BOP_T(gt, i6, int64_t);
VAR_FN_BOP_T(gt, u6, uint64_t);
VAR_FN_BOP_T(gt, f6, double);

// lt
VAR_FN_BOP_T(lt, bl, bool);
VAR_FN_BOP_T(lt, i6, int64_t);
VAR_FN_BOP_T(lt, u6, uint64_t);
VAR_FN_BOP_T(lt, f6, double);

// or
VAR_FN_BOP_T(or, bl, bool);
VAR_FN_BOP_T(or, i6, int64_t);
VAR_FN_BOP_T(or, u6, uint64_t);
VAR_FN_BOP_T(or, f6, double);

bool var_not(bool v);

typedef struct _var_tsv {
    RC;
    size_t len, size;
    jit_fn *gc;
    var *v;
} var_tsv; // tuple, struct, vec

var_tsv *var_tsv_i(al *const a, size_t size, size_t len, jit_fn *gc);

var_tsv *var_ts_fm(al *const a, mod *const m, jit_fn *gc);

size_t var_tsv_len(var_tsv *const tsv);

void var_tsv_rci(var_tsv *const tsv);

void var_tsv_rcd(var_tsv *const tsv);

jit_fn *var_tsv_gc(var_tsv *const tsv);

var var_tsv_gidx(var_tsv *const tsv, size_t idx);

var var_tsv_vr_gidx_i6(mod *const m, ast *const a, var_tsv *const tsv, int64_t idx);

var var_tsv_vr_gidx_u6(mod *const m, ast *const a, var_tsv *const tsv, uint64_t idx);

void var_tsv_sidx(var_tsv *const tsv, size_t idx, var v);

void var_tsv_vr_sidx_i6(mod *const m, ast *const a, var_tsv *const tsv, int64_t idx, var v);

void var_tsv_vr_sidx_u6(mod *const m, ast *const a, var_tsv *const tsv, uint64_t idx, var v);

void var_tsv_add(al *const a, var_tsv *const tsv, var v);

var var_tsv_sub(mod *const m, ast *const a, var_tsv *const tsv);

void var_tsv_d(var_tsv *tsv);

typedef struct _var_hh {
    RC;
    jit_fn *gc;
    tbl *tl; // data freed in git with gc fn
} var_hh;

var_hh *var_hh_i(al *const a, size_t len, jit_fn *gc);

void var_hh_rci(var_hh *const hh);

void var_hh_rcd(var_hh *const hh);

jit_fn *var_hh_gc(var_hh *const hh);

var var_hh_gk(mod *const m, ast *const a, var_sg *sg, var_hh *const hh, bool throw); // if throw error

void var_hh_sk(mod *const m, ast *const a, var_sg *sg, var v, var_hh *const hh);

void var_hh_d(var_hh *hh);

typedef struct _var_td {
    RC;
    mod *m; // has code
    var_tsv *te; // te->v[te->len - 1] is return value
} var_td; // thread

var_td *var_td_i(mod *const m, var_tsv *const te, code *const c);

void var_td_rci(var_td *const td);

void var_td_rcd(var_td *const td);

var_tsv *var_td_te(var_td *const td);

void var_td_d(var_td *td);

void var_td_f(var_td *td);

var var_fd_i(mod *const m, ast *const a, var_sg *const sg);

void var_fd_sg(mod *const m, ast *const a, int fd, var_sg *const sg);
