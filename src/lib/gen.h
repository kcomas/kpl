
#pragma once

#include "as.h"

#define GEN_STAT(N) GEN_STAT_##N

typedef enum {
    GEN_STAT(OK),
    GEN_STAT(INV)
} gen_stat;

#define GEN_CLS(N) GEN_CLS_##N

typedef enum {
    GEN_CLS(N), // none
    GEN_CLS(A), // arg
    GEN_CLS(V), // var local
    GEN_CLS(I), // intermediate
    GEN_CLS(L) // label
} gen_cls;

// var te[cls;type;id]

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, size_t type, size_t id);

typedef tbl *cls_tbl_i(void);

// oc entry te[op_id;fn;tbl[te[cls;type;fn;tbl]]]

typedef struct {
    ssize_t r;
    alfn *ga;
    frfn *gf, *ocef;
    cls_tbl_i *cti;
    tbl *oct; // op cls op tbl given at start
    lst *code;
} gen;

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, cls_tbl_i *cti, tbl *oct, lst *code);

typedef gen_stat gen_fn(gen *g, as *a, void *st); // state

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, size_t type1, gen_cls cls2, size_t type2, gen_cls cls3, size_t type3, gen_fn *fn);

gen_stat gen_a(gen *g, size_t op_id, te *restrict ac1, te *restrict ac2, te *restrict ac3, te *restrict ac4);

void gen_f(gen *g);
