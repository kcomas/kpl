
#pragma once

#include "kpl.h"
#include "ast.h"
#include "type.h"

#define CODE_STAT(N) CODE_STAT_##N

typedef enum {
    CODE_STAT(OK),
    CODE_STAT(INV_L_ASS), // left side : invalid
    CODE_STAT(NO_OP_FOR_VAL_T), // no type for val, should not happen
    CODE_STAT(ARG_LEN_GT_LOCAL_LEN), // should not happen
    CODE_STAT(VAR_TYPE_U),
    CODE_STAT(INV_INT_CST_PUSH)
} code_stat;

typedef struct {
    const char *str;
} code_st;

inline void code_st_i(code_st *const cs, const char *str) {
    cs->str = str;
}

#define OP_C(N) OP_C_##N

typedef enum {
    // data
    OP_C(EFN), // enter fn
    OP_C(RFN), // return fn
    OP_C(AG), // allocate globals
    OP_C(SG), // store global
    OP_C(LG), // load global
    OP_C(AL), // allocate locals
    OP_C(FL), // free loacls
    OP_C(SL), // store local
    OP_C(LL), // load local
    OP_C(SA), // store arg
    OP_C(LA), // load arg
    OP_C(PG), // push global
    OP_C(PL), // push local
    OP_C(PV), // push value
    // control
    OP_C(JMPF), // jmp if false
    // ops
    OP_C(CST),
    OP_C(ADD)
} op_c;

const char *op_c_get_str(op_c oc);

typedef struct _code code;

#define OP_D_V(N) OP_D_V_##N

typedef union {
    type t;
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
    code *c;
} op_d;

#define OP_D(T, V) (op_d) { .T = V }

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
    #define CODE_I_SIZE 50
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
        code *tmp = *c;
        size_t ns = (*c)->size * CODE_R_SIZE;
        *c = calloc(1, sizeof(code) + sizeof(op) * ns);
        memcpy(*c, tmp, sizeof(code) + sizeof(op) * tmp->size);
        (*c)->size = ns;
        free(tmp);
    }
    (*c)->ops[(*c)->len++] = o;
}

inline void code_f(code *c) {
    for (size_t i = 0; i < c->len; i++) {
        if (c->ops[i].ot == TYPE(FN)) code_f(c->ops[i].od.c);
    }
    free(c);
}

void code_p(const code_st *const st, const code *const c, size_t idnt);

#define OP_A(C, OC, OT, OD, A) code_a(&C, (op) {OP_C(OC), TYPE(OT), 0, 0, (op_d) OD, A})

code_stat code_gen(code_st *const cs, const ast *const a, code *c);

code_stat code_gen_fn(code_st *const cs, const fn_node *const fn, code *c);
