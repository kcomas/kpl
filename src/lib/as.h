
#pragma once

#include "tbl.h"

#define AS_STAT(N) AS_STAT_##N

typedef enum {
    AS_STAT(OK),
    AS_STAT(INV),
    AS_STAT(END)
} as_stat;

#define ARG_ID(N) ARG_ID_##N

typedef enum {
    ARG_ID(N), // none passed as NULL
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

te *arg_i(arg_id id, un d, alfn *aa, frfn *af);

// label id

// label entry te[label_id;code;lst[code]] lst needed for backfill

// op te[op_id(must be user defined and unique);arg1;arg2;arg3;arg4]

// op entry te[op_id;fn;lbl_fn;tbl_args[te[arg_id;fn;lbl_fn;tbl_args]]]

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

typedef bool as_code_fn(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4);

as *as_i(alfn *aa, frfn *af, frfn *lef, frfn *oef, frfn *cf, op_tbl_i *oti, tbl *lbls, lst *code);

// add and register label, LABEL(UN) for new label id at code
size_t as_lbl_a(as *const a, size_t lbl_id);

// get lbl entry
as_stat as_lbl_g_i(as *const a, size_t lbl_id, te **lbl);

// register op, not using varardic to avoid extra checks
as_stat as_op_a(as *const a, size_t op_id, arg_id ai1, arg_id ai2, arg_id ai3, arg_id ai4, as_code_fn *fn, as_code_fn *lbl_fn);

// add op
as_stat as_a(as *const a, size_t op_id, te *arg1, te *arg2, te *arg3, te *arg4);

as_stat as_n(as *const a, uint8_t *m);

void as_f(as *a);
