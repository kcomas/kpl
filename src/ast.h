
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

#define LTE_FLAG(N) LTE_FLAG_##N

typedef enum {
    LTE_FLAG(A) = 1 << 0,
    LTE_FLAG(L) = 1 << 1,
    LTE_FLAG(T) = 1 << 2
} lte_flag; // list tbl entry

// lst tbl entry te[mc;u5(id)|u5(flags);type;ast_node]
te *ast_lst_tbl_e_i(const ast *a, mc *s, te *t);

// get first parent node of cls
ast_stat ast_g_pn(ast_cls cls, te *an, te **pn);

// get type, not null
ast_stat ast_g_t(te *an, te **type);

ast *ast_b(ast *a);

void ast_p(const te *an, size_t idnt);

bool ast_eq(const te *restrict a, const te *restrict b);
