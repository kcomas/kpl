
#pragma once

#include "../src/fld.h"
#include "ast_t.h"

typedef fld *fld_build_fn(fld *f);

void fast(_tests *_t, ast *a, te **an, fld_build_fn fn, bool rr);

tbl *fld_lst_tbl_i(ast *a, size_t n, ...);

#define FLG(I, ...) U6((uint64_t) I << 32 | __VA_ARGS__)

// mc, type...
tbl *fld_type_tbl_i(size_t n, ...);

#define LT(N, ...) fld_lst_tbl_i(a, N, __VA_ARGS__)

#define TF(FT, RT, N, ...) type_f_i(&ast_am, NULL, TYPE(FT), RT, fld_type_tbl_i(N, __VA_ARGS__))

#define TS(T) type_s_i(&ast_am, NULL, TYPE(T))
