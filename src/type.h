
#pragma once

#include "kpl.h"
#include "ast.h"

#define TYPE_STAT(N) TYPE_STAT_##N

typedef enum {
    TYPE_STAT(OK),
    TYPE_STAT(SELF_CALL_IN_MOD), // cannot self call in mod
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
    // del
    TYPE_STAT(INV_DEL_R_NN),
    TYPE_STAT(INV_DEL_L_NG),
    // add
    TYPE_STAT(INV_ADD_L_T_N),
    TYPE_STAT(INV_ADD_R_T_N),
    TYPE_STAT(INV_ADD),
    // sub
    TYPE_STAT(INV_SUB_L_T_N),
    TYPE_STAT(INV_SUB_R_T_N),
    TYPE_STAT(INV_SUB),
    // mul
    TYPE_STAT(INV_MUL_L_T_N),
    TYPE_STAT(INV_MUL_R_T_N),
    TYPE_STAT(INV_MUL),
    // eq
    TYPE_STAT(INV_EQ_L_T_N),
    TYPE_STAT(INV_EQ_R_T_N),
    TYPE_STAT(INV_EQ),
    // not
    TYPE_STAT(INV_NOT_R_T_N), // right type for not null
    // gt
    TYPE_STAT(INV_GT_L_T_N),
    TYPE_STAT(INV_GT_R_T_N),
    TYPE_STAT(INV_GT),
    // lt
    TYPE_STAT(INV_LT),
    TYPE_STAT(INV_LT_L_T_N),
    TYPE_STAT(INV_LT_R_T_N),
    // or
    TYPE_STAT(INV_OR_L_T_N),
    TYPE_STAT(INV_OR_R_T_N),
    // cnct
    TYPE_STAT(INV_CNCT_L_T_N),
    TYPE_STAT(INV_CNCT_R_T_N),
    TYPE_STAT(INV_STR_CNCT),
    TYPE_STAT(INV_CNCT),
    // rw
    TYPE_STAT(INV_RW_L_T_N),
    TYPE_STAT(INV_RW_R_T_N),
    TYPE_STAT(INV_RW),
    // fn
    TYPE_STAT(INV_FN_ARG_T), // fn arg is not a var
    TYPE_STAT(INV_FN_T_ARG), // type for fn arg is invalid
    TYPE_STAT(INV_FN_T_RET), // type for fn ret is invalid
    TYPE_STAT(INV_ARGS_OP_CALL), // invalid args for op call
    TYPE_STAT(INV_OP_CALL_LRR_N_N), // op node for call l r ret not null
    TYPE_STAT(INV_CALL_TGT), // cannot get type for call tgt
    TYPE_STAT(INV_CALL_TGT_T), // invalid call target type
    TYPE_STAT(INV_CALL_ARGS_LEN), // too little or too many args
    TYPE_STAT(INV_CALL_TGT_ARG_T), // invalid arg type for call
    TYPE_STAT(INV_CALL_ARG_T), // arg type in call inv
    TYPE_STAT(CALL_ARG_T_NEQ), // ivalid type for call arg
    // ret
    TYPE_STAT(INV_RET_T), // inv ret type
    TYPE_STAT(INV_RET_FNS), // return type for fn not found
    TYPE_STAT(RET_T_NEQ), // ret type not eq to fn type
    // implicit ret
    TYPE_STAT(INV_IRET_T),
    TYPE_STAT(INV_IRET_FNS),
    TYPE_STAT(IRET_T_NEQ),
    TYPE_STAT(VAR_UT) // var not typed
} type_stat;

const char *type_stat_str(type_stat tstat);

typedef struct {
    al *a;
    er *e;
} type_st;

inline void type_st_i(type_st *const ts, al *const a, er *const e) {
    ts->a = a;
    ts->e = e;
}

inline type_stat type_er(type_st *const ts, const char *const fnn, type_stat tstat) {
    if (tstat == TYPE_STAT(OK)) return tstat;
    er_itm *ei = er_itm_i(ts->a, ER(TYPE), fnn, type_stat_str(tstat));
    er_a(ts->e, ei);
    return tstat;
}

type_stat type_chk(type_st *const ts, fn_node *const fns, ast *const a);

type_stat type_chk_fn(type_st *const ts, fn_node *const fn);
