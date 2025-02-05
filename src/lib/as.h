
#pragma once

#include "tbl.h"

#define AS_STAT(N) AS_STAT_##N

typedef enum {
    AS_STAT(OK),
    AS_STAT(INV)
} as_stat;

// arg te[id;data]

te *as_arg_i(const alfr *af, size_t id, un d);

// label id

// label entry te[label_id;code;lst[code]] lst needed for backfill

// op te[op_id(must be user defined and unique);arg1;arg2;arg3;arg4]

// op entry te[op_id;fn;lbl_fn;tbl_args[te[size_t;fn;lbl_fn;tbl_args]]]

#define CODE_ID(N) CODE_ID_##N

typedef enum {
    CODE_ID(O), // op
    CODE_ID(L) // label
} code_id;

// code te[code_id;op_or_label_id;arg1;arg2;arg3;arg4;fn;lbl_fn;pos(in written bytes);len]

typedef tbl *op_tbl_i(void);

typedef struct _as {
    ssize_t r;
    const alfr *af, *ta, *la;
    frfn *lef, *oef, *cf;
    op_tbl_i *oti;
    tbl *lbls, *ops;
    lst *code;
} as;

typedef bool as_code_fn(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4);

typedef bool as_lbl_fn(as *a, uint8_t *m, te *restrict lc, te *restrict fc);

as *as_i(const alfr *af, const alfr *ta, const alfr *la, frfn *lef, frfn *oef, frfn *cf, op_tbl_i *oti, tbl *lbls, lst *code);

// add and register label
size_t as_lbl_a(as *a, size_t lbl_id);

// get lbl entry
te *as_lbl_g_c(as *a, size_t lbl_id);

// set lbl code
as_stat as_lbl_s_c(as *a, size_t lbl_id, te *c);

// register op, not using varardic to avoid extra checks
as_stat as_op_a(as *a, size_t op_id, size_t ai1, size_t ai2, size_t ai3, size_t ai4, as_code_fn *fn, as_lbl_fn *lbl_fn);

// add op
as_stat as_a(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4);

#define AS_A4(A, OP_ID, ARG1, ARG2, ARG3, ARG4) as_a(A, OP_ID, ARG1, ARG2, ARG3, ARG4)
#define AS_A3(A, OP_ID, ARG1, ARG2, ARG3) AS_A4(A, OP_ID, ARG1, ARG2, ARG3, NULL)
#define AS_A2(A, OP_ID, ARG1, ARG2) AS_A3(A, OP_ID, ARG1, ARG2, NULL)
#define AS_A1(A, OP_ID, ARG1) AS_A2(A, OP_ID, ARG1, NULL)
#define AS_A0(A, OP_ID) AS_A1(A, OP_ID, NULL)

as_stat as_n(as *a, uint8_t *m);

void as_f(as *a);
