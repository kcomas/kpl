
#pragma once

#include "ast.h"

#define FLD_STAT(N) FLD_STAT_##N

typedef enum {
    FLD_STAT(OK),
    FLD_STAT(INV)
} fld_stat;

typedef tbl *fld_tbl_i(void);

typedef lst *fld_lst_i(void);

// fld entry te[ast_cls;lst[te[test_fn;fld_fn]]]

typedef struct {
    int32_t tvc; // tmp var counter
    ssize_t r;
    const alfr *af, *ta, *ea;
    fld_tbl_i *fti;
    fld_lst_i *fli;
    ast *a;
    tbl *ft;
} fld;

fld *fld_i(const alfr *af, const alfr *ta, const alfr *ea, fld_tbl_i *fti, fld_lst_i *fli, ast *a, tbl *ft);

fld *fld_i_fld(const fld *f, ast *a);

typedef bool fld_test_fn(const te *an);

typedef fld_stat fld_fn(fld *f, te **an, err **e);

void fld_a(fld *f, ast_cls cls, fld_test_fn test, fld_fn fn);

fld_stat fld_n(fld *f, te **an, err **e, bool rr); // modifies in place

void fld_f(fld *f);
