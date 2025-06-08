
#pragma once

#include "./lib/ast.h"
#include "psr.h"

#define OC(N) OC_##N

typedef enum {
    OC(_START),
    OC(DFN),
    OC(AGN),
    OC(CST),
    OC(LOOP),
    OC(IF),
    OC(MTCH),
    OC(ADD),
    OC(ADDA),
    OC(SUB),
    OC(SUBA),
    OC(MUL),
    OC(MULA),
    OC(DIV),
    OC(EQ),
    OC(GT),
    OC(GTE),
    OC(LT),
    OC(LTE),
    OC(AND),
    OC(CSG),
    OC(CNCT),
    OC(CNCTA),
    OC(DUMP),
    OC(_END)
} oc; // op code

const char *ast_oc_str(oc o);

#define CC(N) CC_##N

typedef enum {
    CC(_START),
    CC(E),
    CC(L),
    CC(P1),
    CC(_END)
} cc; // cmd code

const char *ast_cc_str(cc c);

#define LTE_FLG(N) LTE_FLG_##N

typedef enum {
    LTE_FLG(E) = 1 << 0, // export
    LTE_FLG(A) = 1 << 1, // arg
    LTE_FLG(L) = 1 << 2, // local
    LTE_FLG(T) = 1 << 3, // type
    LTE_FLG(F) = 1 << 4, // func, id becomes the code label
    LTE_FLG(O) = 1 << 5, // outside, used in another scope
    LTE_FLG(S) = 1 << 6, // passed by scope
    LTE_FLG(D) = 1 << 7 // data only, not jitted
} lte_flg; // list tbl entry

#define LTE_FLGS 8

// lst tbl entry te[mc;u5(id)|u5(flgs);type;ast_node]
te *ast_lst_tbl_e_i(const ast *a, mc *s, un flgs, te *t);

uint32_t ast_lst_tbl_e_g_f(te *ent);

void ast_lst_tbl_e_s_f(te *ent, uint32_t flg);

uint32_t ast_lst_tbl_e_g_i(te *ent);

void ast_lst_tbl_e_s_i(te *ent, uint32_t id);

void node_err_p(void *d);

void node_err_f(void *d);

void ast_err_p(void *d);

void ast_err_f(void *d);

ast *ast_b(ast *a);

void ast_p(const te *an, size_t idnt);

bool ast_eq(const te *restrict a, const te *restrict b);

te *ast_cpy(const te *an);
