
#pragma once

#include "kpl.h"
#include "ast.h"

#define CODE_STAT(N) CODE_STAT(N)

typedef enum {
    CODE_STAT(OK)
} code_stat;

typedef struct {

} code_st;

#define OP_C(N) OP_C_##N

typedef enum {
    // data
    OP_C(SG), // store global
    OP_C(LG), // load global
    OP_C(SL), // store local
    OP_C(LL), // load local
    OP_C(PS), // push on stack
    OP_C(SA), // store arg
    OP_C(LA), // load arg
    OP_C(CST)
} op_c;

typedef struct {
    op_c oc;
    type t;
    union {

    } arg;
    ast *a; // freed in mod
} op;

code_stat code_gen(const fn_node *const fn, const ast_node *const a);
