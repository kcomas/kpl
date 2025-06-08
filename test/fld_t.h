
#pragma once

#include "../src/fld.h"
#include "ast_t.h"

extern fld *bfld;

typedef fld *fld_build_fn(fld *f);

void fast(_tests *_t, ast *a, te **an, const fld *bf, bool rr);

tbl *fld_lst_tbl_i(ast *a, size_t n, ...);

#define FLG(I, ...) U6((uint64_t) I << 32 | __VA_ARGS__)

// char*, te[type] id ....
tbl *fld_type_tbl_i(bool gid, size_t n, ...);

#define LT(N, ...) fld_lst_tbl_i(a, N, __VA_ARGS__)

#define TF(FT, RT) type_f_i(&al_te, NULL, TYPE(FT), RT, NULL, NULL)

#define TFN(FT, RT, N, ...) type_f_i(&al_te, NULL, TYPE(FT), RT, fld_type_tbl_i(true, N, __VA_ARGS__), NULL)

#define TST(N, ...) type_h_i(&al_te, NULL, TYPE(ST), fld_type_tbl_i(false, N, __VA_ARGS__))

#define TFS(FT, RT, A, S) type_f_i(&al_te, NULL, TYPE(FT), RT, A, S)

#define TS(T) type_s_i(&al_te, NULL, TYPE(T))
