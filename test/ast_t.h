
#include "../src/ast.h"
#include "psr_t.h"

extern const alfr ast_am;

ast_stat pig(te *pn, size_t *pid);

tbl *ati(void);

lst *ali(void);

tbl *mktbl(size_t size);

void bast(_tests *_t, ast *a, const char *pgm, te **an);

void ast_verify(_tests *_t, ast *a, te *restrict an, te *restrict tn);

#define NN(C, T, ...) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T))

#define RN(N) ast_an_i(a, NULL, NULL, AST_CLS(R), P(NULL), N)

#define IN(S) ast_an_i(a, NULL, NULL, AST_CLS(I), P(NULL), mc_i_cstr(S, &ast_am))

#define TN(V) ast_an_i(a, NULL, NULL, AST_CLS(T), P(type_i(&ast_am, TYPE(V))))

#define SN(T, V) ast_an_i(a, NULL, NULL, AST_CLS(S), P(type_i(&ast_am, TYPE(T))), V)

#define ON(T, C, L, R) ast_an_i(a, NULL, NULL, AST_CLS(O), T, OC(C), L, R)

#define L(n, ...) lst_i_v(&ast_am, &ast_am, (void*) te_f, n, __VA_ARGS__)

#define AN(T, TGT, LST) ast_an_i(a, NULL, NULL, AST_CLS(A), T, TGT, LST)

#define ZN(S, TGT) ast_an_i(a, NULL, NULL, AST_CLS(Z), P(type_i(&ast_am, TYPE(SL))), mc_i_cstr(S, &ast_am), TGT)

#define LN(LST) ast_an_i(a, NULL, NULL, AST_CLS(L), P(NULL), LST)
