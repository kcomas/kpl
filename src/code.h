
#pragma once

#include "kpl.h"
#include "ast.h"

#define CODE_STAT(N) CODE_STAT(N)

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
    OP_C(CST)
} op_c;

typedef struct {
    op_c oc;
    type t;
    union {

    } arg;
    ast *a; // freed in mod
} op;

typedef struct {
    size_t len, size;
    op ops[];
} bcode;

code_stat code_gen(const ast_node *const a, bcode *const bc);
