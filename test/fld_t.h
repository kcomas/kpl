
#pragma once

#include "../src/fld.h"
#include "ast_t.h"

extern fld *bfld;

typedef fld *fld_build_fn(fld *f);

void fast(_tests *_t, ast *a, te **an, const fld *bf, bool rr);

tbl *fld_lst_tbl_i(ast *a, size_t n, ...);

#define FLG(I, ...) U6((uint64_t) I << 32 | __VA_ARGS__)

// char*, te[type] id ....
tbl *fld_type_tbl_i(size_t n, ...);

#define LT(N, ...) fld_lst_tbl_i(a, N, __VA_ARGS__)

#define TF(FT, RT) type_f_i(&ast_am, NULL, TYPE(FT), RT, NULL)

#define TFN(FT, RT, N, ...) type_f_i(&ast_am, NULL, TYPE(FT), RT, fld_type_tbl_i(N, __VA_ARGS__))

#define TS(T) type_s_i(&ast_am, NULL, TYPE(T))
