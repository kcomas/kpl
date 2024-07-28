
#pragma once

#include "kpl.h"
#include "ast.h"

#define TYPE_STAT(N) TYPE_STAT_##N

typedef enum {
    TYPE_STAT(OK),
    TYPE_STAT(VAL_UT), // val is ast type
    TYPE_STAT(INV_VAR_ASS), // cannot assign to var
    TYPE_STAT(VAR_ASS_N_T_M), // var assignment types do not match
    TYPE_STAT(INV_ASS_TO), // cannot assign to node
    TYPE_STAT(INV_CST), // invalid cast
    TYPE_STAT(INV_CST_L_A), // invalid node for left side of cst
    TYPE_STAT(INV_CST_L_T_N), // left side type for cst null
    TYPE_STAT(INV_CST_R_T_N), // right side type for cst null
    TYPE_STAT(INV_FN_CST), // invalid case for fn
    TYPE_STAT(FN_CST_T_NN), // fn node type defined
    TYPE_STAT(INV_FN_CST_ARGS_LEN), // length of args does not match type
    // add
    TYPE_STAT(INV_ADD_L_T_N),
    TYPE_STAT(INV_ADD_R_T_N),
    TYPE_STAT(INV_ADD),
    // sub
    TYPE_STAT(INV_SUB_L_T_N),
    TYPE_STAT(INV_SUB_R_T_N),
    TYPE_STAT(INV_SUB),
    // eq
    TYPE_STAT(INV_EQ_L_T_N),
    TYPE_STAT(INV_EQ_R_T_N),
    TYPE_STAT(INV_EQ),
    // not
    TYPE_STAT(INV_NOT_R_T_N), // right type for not null
    // or
    TYPE_STAT(INV_OR_L_T_N),
    TYPE_STAT(INV_OR_R_T_N),
    TYPE_STAT(INV_FN_ARG_T), // fn arg is not a var
    TYPE_STAT(INV_FN_T_ARG), // type for fn arg is invalid
    TYPE_STAT(INV_FN_T_RET), // type for fn ret is invalid
    TYPE_STAT(INV_ARGS_OP_CALL), // invalid args for op call
    TYPE_STAT(INV_OP_CALL_LRR_N_N), // op node for call l r ret not null
    TYPE_STAT(INV_RET_T), // inv ret type
    TYPE_STAT(INV_RET_FNS), // return type for fn not found
    TYPE_STAT(RET_T_NEQ), // ret type not eq to fn type
    TYPE_STAT(VAR_UT) // var not typed
} type_stat;

type_stat type_chk(fn_node *const fns, ast *const a);

type_stat type_chk_fn(fn_node *const fn);
