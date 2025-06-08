
#include "../src/ast.h"
#include "psr_t.h"

extern const alfr ast_am;

extern ast *bast;

ast_stat pig(te *pn, size_t *pid);

tbl *ati(void);

lst *ali(void);

tbl *mktbl(size_t size);

void astb(_tests *_t, ast *a, const char *pgm, te **an);

void ast_verify(_tests *_t, ast *a, te *restrict an, te *restrict tn);

#define NN(C, T) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T))

#define NNV(C, T, ...) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T), __VA_ARGS__)

#define RN(N) NNV(R, NULL, N)

#define TN(V) NN(T, type_i(&ast_am, NULL, TYPE(V)))

#define EN(S, F, T) NN(E, ast_lst_tbl_e_i(a, mc_i_cstr(S, &ast_am), F, T))

#define IN(S) NN(I, mc_i_cstr(S, &ast_am))

#define SN(T, V) NNV(S, type_i(&ast_am, NULL, TYPE(T)), V)

#define ON(T, C, L, R) NNV(O, T, OC(C), L, R)

#define L(n, ...) lst_i_v(&ast_am, &ast_am, (void*) te_f, n, __VA_ARGS__)

#define AN(T, TGT, LST) NNV(A, T, TGT, LST)

#define ZN(S, TGT) NNV(Z, type_i(&ast_am, NULL, TYPE(SL)), TGT, mc_i_cstr(S, &ast_am))

#define LN(TBL, LST) NNV(L, TBL, LST)

#define CN(CMD, TGT) NNV(C, CC(CMD), TGT)
