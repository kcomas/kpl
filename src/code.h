
#pragma once

#include "kpl.h"
#include "ast.h"
#include "type.h"

#define CODE_STAT(N) CODE_STAT_##N

typedef enum {
    CODE_STAT(OK),
    CODE_STAT(NO_OP_FOR_VAL_T), // no type for val, should not happen
    CODE_STAT(ARG_LEN_GT_LOCAL_LEN) // should not happen
} code_stat;

typedef struct {
    const char *str;
} code_st;

#define OP_C(N) OP_C_##N

typedef enum {
    // data
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
    OP_C(CST)
} op_c;

typedef struct _code code;

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

#ifndef REC_CODE_I_SIZE
    #define REC_CODE_I_SIZE 30
#endif

inline code *code_i(size_t size) {
    code *c = calloc(1, sizeof(code) + sizeof(op) * size);
    c->size = size;
    return c;
}

inline void code_a(code **c, op o) {
    if ((*c)->len + 1 == (*c)->size) {
        // TODO resize
    }
    (*c)->ops[(*c)->len++] = o;
}

#define OP_A(C, OC, OT, OD, A) code_a(&C, (op) {OP_C(OC), TYPE(OT), 0, 0, (op_d) OD, A})

code_stat code_gen(code_st *const cs, const ast *const a, code *c);

code_stat code_gen_fn(code_st *const cs, const ast *const afn, code *c);
