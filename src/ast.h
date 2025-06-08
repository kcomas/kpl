
#pragma once

#include "./lib/ast.h"
#include "./lib/type.h"
#include "psr.h"

#define OC(N) OC_##N

typedef enum {
    OC(_START),
    OC(DFN),
    OC(AGN),
    OC(CST),
    OC(ADD),
    OC(SUB),
    OC(DUMP),
    OC(_END)
} oc; // op code

const char *ast_oc_str(oc o);

#define CC(N) CC_##N

typedef enum {
    CC(_START),
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
    LTE_FLG(S) = 1 << 5 // used in another scope
} lte_flg; // list tbl entry

#define LTE_FLGS 6

// lst tbl entry te[mc;u5(id)|u5(flgs);type;ast_node]
te *ast_lst_tbl_e_i(const ast *a, mc *s, un flgs, te *t);

uint32_t ast_lst_tbl_e_g_f(te *ent);

void ast_lst_tbl_e_s_f(te *ent, uint32_t flg);

uint32_t ast_lst_tbl_e_g_i(te *ent);

void ast_lst_tbl_e_s_i(te *ent, uint32_t id);

// get first parent node of cls
ast_stat ast_g_pn(ast_cls cls, te *an, te **pn);

// get type, not null
ast_stat ast_g_t(te *an, te **type);

ast *ast_b(ast *a);

void ast_p(const te *an, size_t idnt);

bool ast_eq(const te *restrict a, const te *restrict b);

te *ast_cpy(const te *an);
