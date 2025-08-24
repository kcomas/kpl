
#include "../src/ast.h"
#include "psr_t.h"

extern const alfr ast_am;

extern ast *bast;

ast_stat pig(te *pn, size_t *pid);

tbl *ati(void);

lst *ali(void);

tbl *mktbl(size_t size);

void astb(_tests *_t, ast *a, const char *pgm, te **an);

te *ast_t_regx(ast *a, te *type, uint32_t id, uint16_t mode, uint16_t flgs, const char *mtch, const char *rplc);

void ast_verify(_tests *_t, ast *a, te *restrict an, te *restrict tn);

#define NN(C, T) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T))

#define NNV(C, T, ...) ast_an_i(a, NULL, NULL, AST_CLS(C), P(T), __VA_ARGS__)

#define RN(N) NNV(R, NULL, N)

#define TN(V) NN(T, type_i(&al_te, NULL, TYPE(V)))

#define EN(S, F, T) NN(E, ast_lst_tbl_e_i(a, mc_i_cstr(S, &al_mc), F, T))

#define IN(S) NN(I, mc_i_cstr(S, &al_mc))

#define SN(T, V) NNV(S, type_i(&al_te, NULL, TYPE(T)), V)

#define CS(CSTR) ast_s_i(a, NULL, NULL, ast_s_cs_f, P(type_i(&al_te, NULL, TYPE(CS))), P(mc_i_cstr(CSTR, &al_mc)))

#define ON(T, C, L, R) NNV(O, T, OC(C), L, R)

#define L(n, ...) lst_i_v(&al_lst, &al_te, (void*) te_f, n, __VA_ARGS__)

#define L0() lst_i(&al_lst, &al_te, (void*) te_f)

#define LE() lst_i(&al_lst, &al_te, (void*) te_f)

#define AN(T, TGT, LST) NNV(A, T, TGT, LST)

#define ZTN(S, T, TGT) NNV(Z, type_v_i(&al_te, NULL, TYPE(SL), T), TGT, mc_i_cstr(S, &al_mc))

#define ZN(S, TGT) ZTN(S, NULL, TGT)

#define LN(TBL, LST) NNV(L, TBL, LST)

#define VN(TYPE, LST) NNV(V, TYPE, LST)

#define CN(CMD, TGT) NNV(C, CC(CMD), TGT)

#define REGX(T, I, M, F, H, R) ast_t_regx(a, T, I, M, F, H, R)
