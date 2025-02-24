
#include "../src/ast.h"
#include "psr_t.h"

extern const alfr ast_am;

ast_stat pig(te *pn, size_t *pid);

tbl *ati(void);

lst *ali(void);

tbl *mktbl(size_t size);

void bast(_tests *_t, ast *a, const char *pgm, te **an);

void ast_verify(_tests *_t, ast *a, te *restrict an, te *restrict tn);

#define NN(C, T) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T))

#define NNV(C, T, ...) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T), __VA_ARGS__)

#define RN(N) NNV(R, NULL, N)

#define IN(S) NN(I, mc_i_cstr(S, &ast_am))

#define TN(V) NN(T, type_i(&ast_am, TYPE(V)))

#define SN(T, V) NNV(S, type_i(&ast_am, TYPE(T)), V)

#define ON(T, C, L, R) NNV(O, T, OC(C), L, R)

#define L(n, ...) lst_i_v(&ast_am, &ast_am, (void*) te_f, n, __VA_ARGS__)

#define AN(T, TGT, LST) NNV(A, T, TGT, LST)

#define ZN(S, TGT) NNV(Z, type_i(&ast_am, TYPE(SL)), mc_i_cstr(S, &ast_am), TGT)

#define LN(LST) NNV(L, NULL, LST)
