
#pragma once

#include "as.h"

#define GEN_CLS(N) GEN_CLS_##N

typedef enum {
    GEN_CLS(A), // arg
    GEN_CLS(V), // var local
    GEN_CLS(I), // intermediate
    GEN_CLS(L) // label
} gen_cls;

// var te[cls;type;id]

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, size_t type, size_t id);

typedef tbl *cls_tbl_i(void);

// oc entry te[op_id;fn;tbl[te[cls;fn;tbl]]]

typedef struct {
    ssize_t r;
    alfn *ga;
    frfn *gf, *ocef;
    cls_tbl_i *cti;
    tbl *oc; // op cls op tbl given at start
    lst *code;
} gen;

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, cls_tbl_i *cti, tbl *oc, lst *code);

te *gen_a(gen *g, size_t op_id, te *restrict var1, te *restrict var2, te *restrict var3);

void gen_f(gen *g);
