
#pragma once

#include "tbl.h"

#define ARG_ID(N) ARG_ID_##N

typedef enum {
    ARG_ID(R), // reg
    ARG_ID(RM), // reg mem
    ARG_ID(M), // mem
    ARG_ID(O), // op
    ARG_ID(L), // label
    ARG_ID(B), // byte
    ARG_ID(W), // word
    ARG_ID(DW), // double word
    ARG_ID(QW) // quad word
} arg_id;

typedef struct {
    arg_id ai;
    un data;
} arg;

// cmd te[op/label;arg1;arg2;arg3;arg4]

#define LABEL(N) LABEL_##N

typedef enum {
    LABEL(UN) = 0,
    LABEL(_),
    LABEL(USER)
} label;

typedef struct {
    ssize_t r;
    size_t lc; // label counter
    tbl *lbls, *ops;
    lst *code;
} as;
