
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

#define SET(N, ...) ast_s_i(a, NULL, NULL, fld_s_st_et_f, P(type_h_i(&al_te, NULL, TYPE(ET), fld_lst_tbl_i(a, N, __VA_ARGS__))), P(NULL))

#define TF(FT, RT) type_f_i(&al_te, NULL, TYPE(FT), RT, NULL, NULL)

#define TFN(FT, RT, N, ...) type_f_i(&al_te, NULL, TYPE(FT), RT, fld_type_tbl_i(true, N, __VA_ARGS__), NULL)

#define TH(T, N, ...) type_h_i(&al_te, NULL, TYPE(T), fld_type_tbl_i(false, N, __VA_ARGS__))

#define TFS(FT, RT, A, S) type_f_i(&al_te, NULL, TYPE(FT), RT, A, S)

#define TS(T) type_s_i(&al_te, NULL, TYPE(T))

#define TV(T, V) type_v_i(&al_te, NULL, TYPE(T), V)
