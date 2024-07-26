
#pragma once

#include "kpl.h"
#include "ast.h"

#define TYPE_STAT(N) TYPE_STAT_##N

typedef enum {
    TYPE_STAT(OK),
    TYPE_STAT(VAL_UT), // val is ast type
    TYPE_STAT(INV_VAR_ASS), // cannot assign to var
    TYPE_STAT(INV_ASS_TO), // cannot assign to node
    TYPE_STAT(INV_CST_L_A), // invalid node for left side of cst
    TYPE_STAT(INV_CST_L_T_N), // left side type for cst null
    TYPE_STAT(INV_CST_R_T_N) // right side type for cst null
} type_stat;

type_stat type_chk(fn_node *const fns, ast *const a);

type_stat type_chk_fn(fn_node *const fn);
