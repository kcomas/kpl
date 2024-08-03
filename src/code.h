
#pragma once

#include "kpl.h"
#include "ast.h"

#define CODE_STAT(N) CODE_STAT_##N

typedef enum {
    CODE_STAT(OK)
} code_stat;

#define OP_C(N) OP_C_##N

typedef enum {
    // data
    OP_C(SG), // store global
    OP_C(LG), // load global
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

typedef union {
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
} op_d;

#define OP_D(T, V) (op_d) { .T = V }

typedef struct {
    op_c oc;
    type ot;
    uint16_t jidx, jlen; // idx and len in jit code of this op
    op_d od;
    const ast *a; // freed in mod
} op;

#define OP(OC, OT, OD, A) (op) {OC, OT, OD, A}

typedef struct {
    size_t len, size;
    op ops[];
} code;

#ifndef REC_CODE_I_SIZE
    #define REC_CODE_I_SIZE 50
#endif

inline code *code_i(size_t size) {
    code *c = calloc(1, sizeof(code) + sizeof(op) * size);
    c->size = size;
    return c;
}

inline void code_a(code **c, op o) {
    // TODO resize
    (*c)->ops[(*c)->len++] = o;
}

code_stat code_gen(const ast *const a, code *const c);
