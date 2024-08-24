
#pragma once

#include "kpl.h"
#include "ast.h"
#include "type.h"

#define CODE_STAT(N) CODE_STAT_##N

typedef enum {
    CODE_STAT(OK),
    CODE_STAT(INV_L_ASS), // left side : invalid
    CODE_STAT(INV_R_ASS), // right side : invalid
    CODE_STAT(INV_STR_ESC), /* invalid \ */
    CODE_STAT(NO_OP_FOR_VAL_T), // no type for val, should not happen
    CODE_STAT(NO_T_FOR_IF_COND), // cannot not get if conds type
    CODE_STAT(ARG_LEN_GT_LOCAL_LEN), // should not happen
    CODE_STAT(VAR_TYPE_U),
    CODE_STAT(INV_INT_CST_PUSH),
    CODE_STAT(INV_CST_INT_TO_FD),
    CODE_STAT(INV_CST_FD),
    CODE_STAT(INV_CST),
    CODE_STAT(NO_TYPE_COR_INT),
    CODE_STAT(OP_NO_T_L), // op no left type
    CODE_STAT(OP_NO_T_R), // op no right type
    CODE_STAT(INV_SG_CNCT),
    CODE_STAT(INV_CNCT_OP),
    CODE_STAT(INV_FD_OP),
    CODE_STAT(CALL_RES_NOT_SELF),
    CODE_STAT(CALL_VAR_N_FN),
    CODE_STAT(CALL_ARG_T_INV),
    CODE_STAT(CALL_TYPE_T_INV),
    CODE_STAT(INV_CALL_TGT),
    CODE_STAT(NO_OP_FOR_RET_VAL_T)
} code_stat;

typedef struct {
    const char *str;
} code_st;

inline void code_st_i(code_st *const cs, const char *str) {
    cs->str = str;
}

#define OP_C(N) OP_C_##N

typedef enum {
    OP_C(EFN), // enter fn
    OP_C(RFN), // return fn
    OP_C(CFN), // call fn
    OP_C(CS), // call self
    // data
    OP_C(AG), // allocate globals
    OP_C(SG), // store global
    OP_C(LG), // load global
    OP_C(AL), // allocate locals
    OP_C(FL), // free loacls
    OP_C(SL), // store local
    OP_C(LL), // load local
    OP_C(SA), // store arg
    OP_C(LA), // load arg
    OP_C(PV), // push value
    OP_C(CTE), // create tuple from stack u6 is length
    // control
    OP_C(IF),
    OP_C(COND), // jmp if false
    // coalesce
    OP_C(ZOO), // convert to zero or one
    // ops
    OP_C(CST),
    OP_C(CSTSG), // cast type to string
    OP_C(ADD),
    OP_C(SUB),
    OP_C(EQ),
    OP_C(NOT),
    OP_C(OR),
    OP_C(CNCTSG), // sg cnct op type is ethier sg or te
    OP_C(WFD), // OP_T is type to be written
    // GC
    OP_C(GC), // type is base type
    OP_C(GCTEI) //
} op_c;

const char *op_c_get_str(op_c oc);

typedef struct {
   code *cond, *body;
} op_if;

typedef struct _slv {
    uint8_t id;
    type t;
} slv; // store load var

#define SLV(ID, T) .v = (slv) {ID, T}

typedef struct _op_d_te op_d_te;

typedef union _op_d {
    type t;
    bool bl;
    uint8_t u3;
    uint16_t u4;
    uint32_t u5;
    uint64_t u6;
    int8_t i3;
    int16_t i4;
    int32_t i5;
    int64_t i6;
    float f;
    double d;
    int fd;
    slv v;
    code *c;
    op_if *of;
    char *sg; // null term
    op_d_te *te;
} op_d;

typedef struct _op_d_te {
    size_t size; // fixed
    op_d d[];
} op_d_te;

typedef struct {
    op_c oc;
    type ot;
    uint16_t jidx, jlen; // idx and len in jit code of this op
    op_d od;
    const ast *a; // freed in mod
} op;

typedef struct _code {
    size_t len, size;
    jit_fn *jf; // addr of jit code
    op ops[];
} code;

#ifndef CODE_I_SIZE
    #define CODE_I_SIZE 20
#endif

inline code *code_i(size_t size) {
    code *c = calloc(1, sizeof(code) + sizeof(op) * size);
    c->size = size;
    return c;
}

#ifndef CODE_R_SIZE
    #define CODE_R_SIZE 2
#endif

inline void code_a(code **c, op o) {
    if ((*c)->len == (*c)->size) {
        code *nc = code_i((*c)->size * CODE_R_SIZE);
        for (size_t i = 0; i < (*c)->len; i++) nc->ops[nc->len++] = (*c)->ops[i];
        free(*c);
        *c = nc;
    }
    (*c)->ops[(*c)->len++] = o;
}

void code_f(code *c);

inline op_if *op_if_i(size_t size) {
    op_if *of = calloc(1, sizeof(op_if));
    of->cond = code_i(size);
    of->body = code_i(size);
    return of;
}

inline void op_if_f(op_if *of) {
    code_f(of->cond);
    code_f(of->body);
    free(of);
}

void code_p(const code_st *const st, const code *const c, size_t idnt);

#define OP_A(C, OC, OT, OD, A) code_a(C, (op) {OP_C(OC), TYPE(OT), 0, 0, (op_d) OD, A})

code_stat code_gen(code_st *const cs, const ast *const a, code **c);

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code **c);
