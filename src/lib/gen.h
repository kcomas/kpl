
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
    GEN_CLS(M), // multiple, array of vars
    GEN_CLS(A), // arg
    GEN_CLS(V), // var local
    GEN_CLS(T), // tmp
    GEN_CLS(L), // label
    GEN_CLS(D) // data
} gen_cls;

const char* gen_cls_str(gen_cls cls);

// var te[cls;info;id]

te *gen_var(alfn *ga, frfn *gf, gen_cls cls, un info, un id);

typedef tbl *cls_tbl_i(void);

// oci entry te[op_id;fn;tbl[te[info<<5+cls;cls;info;fn;tbl]]]

// code te[op_id;aci;ac2;ac3;fn;as_code_s;as_code_e]

typedef struct {
    ssize_t r;
    alfn *ga;
    frfn *gf, *ocef, *cef;
    cls_tbl_i *cti;
    tbl *oci; // op cls op tbl given at start
    lst *code;
} gen;

gen *gen_i(alfn *ga, frfn *gf, frfn *ocef, frfn *cef, cls_tbl_i *cti, tbl *oci, lst *code);

typedef gen_stat gen_fn(alfn *aa, frfn *fr, gen *g, void *s, te *ci, as *a); // state

gen_stat gen_op_a(gen *g, size_t op_id, gen_cls cls1, un info1, gen_cls cls2, un info2, gen_cls cls3, un info3, gen_fn *fn);

#define GEN_OP_A3(g, op_id, cls1, info1, cls2, info2, cls3, info3, fn) gen_op_a(g, op_id, cls1, info1, cls2, info2, cls3, info3, fn)
#define GEN_OP_A2(g, op_id, cls1, info1, cls2, info2, fn) GEN_OP_A3(g, op_id, cls1, info1, cls2, info2, GEN_CLS(N), U3(0), fn)
#define GEN_OP_A1(g, op_id, cls1, info1, fn) GEN_OP_A2(g, op_id, cls1, info1, GEN_CLS(N), U3(0), fn)
#define GEN_OP_A0(g, op_id, fn) GEN_OP_A1(g, op_id, GEN_CLS(N), U3(0), fn)

gen_stat gen_a(gen *g, size_t op_id, te *restrict ac1, te *restrict ac2, te *restrict ac3);

gen_stat gen_n(alfn *al, frfn *fr, gen *g, void *st, as *a);

void gen_f(gen *g);
