
#pragma once

#include "tbl.h"

#define ARG_ID(N) ARG_ID_##N

typedef enum {
    ARG_ID(R), // reg
    ARG_ID(RM), // reg mem
    ARG_ID(L), // label
    ARG_ID(M), // mem
    ARG_ID(B), // byte
    ARG_ID(W), // word
    ARG_ID(DW), // double word
    ARG_ID(QW) // quad word
} arg_id;

// arg te[id;data]

// label id

// label entry te[label_id;code;lst[code]] lst needed for backfill

// op te[op_id(must be user defined);arg1;arg2;arg3;arg4]

// op entry te[op_id;tbl_args[te[arg;tbl_args]]]

// code te[lst_or_op;id;counter;pos(in written bytes);arg1;arg2;arg3;arg4]

#define LABEL(N) LABEL_##N

typedef enum {
    LABEL(UN) = 0,
    LABEL(_),
    LABEL(USER) = 1000
} label;

#define OP(N) OP_##N

typedef struct {
    ssize_t r;
    size_t lc, cc; // label counter, code counter
    tbl *lbls, *ops;
    lst *code;
} as;
