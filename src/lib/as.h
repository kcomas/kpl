
#pragma once

#include "tbl.h"
#include "err.h"

#define AS_STAT(N) AS_STAT_##N

typedef enum {
    AS_STAT(OK),
    AS_STAT(INV),
    AS_STAT(CODE),
    AS_STAT(LBL),
    AS_STAT(DATA)
} as_stat;

// label entry te[label_id;code;lst[code]] lst needed for backfill

// op te[op_id(must be user defined and unique);arg1;arg2;arg3;arg4]

// op entry te[op_id;fn;lbl_fn;tbl_args[te[size_t;fn;lbl_fn;tbl_args]]]

#define CODE_ID(N) CODE_ID_##N

typedef enum {
    CODE_ID(O), // op
    CODE_ID(L) // label
} code_id;

// code te[code_id;op_or_label_id;arg1;arg2;arg3;arg4;fn;lbl_fn;pos(in written bytes);len]

// data queue te[code;size;data;fn;pos]

typedef tbl *op_tbl_i(void);

typedef err_d_p *as_err_g_p(as_stat stat);

typedef void as_mem_fn(size_t ms, uint8_t *m);

typedef struct _as {
    ssize_t r;
    const alfr *af, *ta, *la, *ea;
    as_mem_fn *msf, *mef; // mem start fn, mem end fn
    as_err_g_p *gep;
    op_tbl_i *oti;
    tbl *lbls, *ops;
    lst *dq, *code; // data queue
} as;

typedef bool as_code_fn(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4);

typedef bool as_lbl_fn(as *a, uint8_t *m, te *restrict lc, te *restrict fc);

as *as_i(const alfr *af, const alfr *ta, const alfr *la, const alfr *ea, as_mem_fn msf, as_mem_fn mef, as_err_g_p gep, op_tbl_i oti, tbl *lbls, lst *code);

as *as_i_as(const as *a);

// add and register label
size_t as_lbl_a(as *a, size_t lbl_id);

// get lbl entry as weak ref
te *as_lbl_g_c(as *a, size_t lbl_id);

// get lbl entry index in buffer
ssize_t as_lbl_g_c_i(as *a, size_t lbl_id);

// set lbl code
as_stat as_lbl_s_c(as *a, size_t lbl_id, te *c);

// update pos at end
typedef bool as_dq_fn(as *a, size_t *p, uint8_t *m, te *dqe);

void as_dq_a(as *a, te *ci, size_t size, un v, as_dq_fn dq_fn);

// register op, not using varardic to avoid extra checks
as_stat as_op_a(as *a, size_t op_id, size_t ai1, size_t ai2, size_t ai3, size_t ai4, as_code_fn *fn, as_lbl_fn lbl_fn);

// add op
as_stat as_a(as *a, size_t op_id, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4);

#define AS_A4(A, OP_ID, ARG1, ARG2, ARG3, ARG4) as_a(A, OP_ID, ARG1, ARG2, ARG3, ARG4)
#define AS_A3(A, OP_ID, ARG1, ARG2, ARG3) AS_A4(A, OP_ID, ARG1, ARG2, ARG3, NULL)
#define AS_A2(A, OP_ID, ARG1, ARG2) AS_A3(A, OP_ID, ARG1, ARG2, NULL)
#define AS_A1(A, OP_ID, ARG1) AS_A2(A, OP_ID, ARG1, NULL)
#define AS_A0(A, OP_ID) AS_A1(A, OP_ID, NULL)

as_stat as_n(as *a, size_t *p, size_t ms, uint8_t *m, err **e);

void as_f(as *a);
