
#pragma once

#include "tbl.h"

#define AS_STAT(N) AS_STAT_##N

typedef enum {
    AS_STAT(OK),
    AS_STAT(INV),
    AS_STAT(END)
} as_stat;

// arg te[id;data]

te *arg_i(size_t id, un d, alfn *aa, frfn *af);

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

#define LABEL(N) LABEL_##N

typedef enum {
    LABEL(UN) = 0,
    LABEL(_),
    LABEL(USER) = 1000
} label;

#define OP(N) OP_##N

typedef tbl *op_tbl_i(void);

typedef struct _as {
    ssize_t r;
    size_t lc; // label counter
    alfn *aa;
    frfn *af, *lef, *oef, *cf;
    op_tbl_i *oti;
    tbl *lbls, *ops;
    lst *code;
} as;

typedef bool as_code_fn(as *const a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4);

typedef bool as_lbl_fn(as *const a, uint8_t *m, te *restrict lc, te *restrict fc);

as *as_i(alfn *aa, frfn *af, frfn *lef, frfn *oef, frfn *cf, op_tbl_i *oti, tbl *lbls, lst *code);

// add and register label, LABEL(UN) for new label id at code
size_t as_lbl_a(as *const a, size_t lbl_id);

// get lbl entry
te *as_lbl_g_c(as *const a, size_t lbl_id);

// set lbl code
as_stat as_lbl_s_c(as *const a, size_t lbl_id, te *const c);

// register op, not using varardic to avoid extra checks
as_stat as_op_a(as *const a, size_t op_id, size_t ai1, size_t ai2, size_t ai3, size_t ai4, as_code_fn *fn, as_lbl_fn *lbl_fn);

// add op
as_stat as_a(as *const a, size_t op_id, te *const arg1, te *const arg2, te *const arg3, te *const arg4);

as_stat as_n(as *const a, uint8_t *m);

void as_f(as *a);
