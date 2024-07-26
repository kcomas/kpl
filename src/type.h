
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
    TYPE_STAT(INV_CST_R_T_N), // right side type for cst null
    TYPE_STAT(INV_FN_CST), // invalid case for fn
    TYPE_STAT(FN_CST_T_NN), // fn node type defined
    TYPE_STAT(INV_FN_CST_ARGS_LEN), // length of args does not match type
    TYPE_STAT(INV_FN_ARG_T), // fn arg is not a var
    TYPE_STAT(INV_FN_T_ARG), // type for fn arg is invalid
    TYPE_STAT(INV_FN_T_RET) // type for fn ret is invalid
} type_stat;

type_stat type_chk(fn_node *const fns, ast *const a);

type_stat type_chk_fn(fn_node *const fn);
