
#pragma once

#include "./lib/atg.h"
#include "opt.h"
#include "./lib/gen_x64.h"
#include "./lib/gen_type.h"

void atg_d_p(const atg *t);

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt);

bool atg_x64_enq(const te *an);

x64_type type_g_x64_type(const te *type);

atg_stat atg_err(const atg *t, te *an, err **e, const char *m);

te *atg_te_idx_d(gen *g, uint32_t id);

te *atg_vr_idx_d(gen *g, uint32_t id);

te *var_arg(gen *g, te *lte, x64_type xt);

atg_stat atg_nop(atg *t, gen *g, te *an, err **e);

typedef te *atg_idx_d(gen *g, uint32_t id);

atg_stat v_set_fn(atg *t, gen *g, te *restrict an, err **e, te *restrict n, uint32_t ti, uint32_t eid, atg_idx_d idxfn);

atg_stat atg_te_init(atg *t, gen *g, te *restrict an, err **e, te *restrict type, size_t l, uint32_t ti);

atg_stat aply_te_g_idx(atg *t, te *an, err **e, size_t *n, te **lte);

atg_stat z_lte_id(atg *t, te *an, err **e, const te *zn, te **lte, size_t *id);

atg_stat call_npr(gen_op *go, const te *an);

extern const char *atg_un_inv_str;

extern const char *atg_vr_inv_str;

void atg_sg_err_p(void *p);

extern const char *atg_dump_strs[TYPE(_END)];

extern const char *atg_dump_end;

extern const char *atg_dump_idnt;

extern const char *atg_sym_str;

atg *atg_b(atg *t);

typedef err *atg_jit(te *x);

err *atg_z(const atg *t, tbl *volatile et, const uint8_t *m, size_t ep);
