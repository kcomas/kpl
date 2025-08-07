
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

bool inloop(te *an);

atg_stat atg_g_un_ev(te *tn, uint64_t *esym, uint64_t *vsym);

te *var_arg(gen *g, te *lte, x64_type xt);

atg_stat atg_an_var(gen *g, te **i);

atg_stat atg_nop(atg *t, gen *g, te *an, err **e);

typedef te *atg_idx_d(gen *g, uint32_t id);

atg_stat v_set_fn(atg *t, gen *g, te *restrict an, err **e, te *restrict n, uint32_t ti, uint32_t eid, atg_idx_d idxfn);

gen_stat inc_ref_u(gen *g, x64_type xt, uint32_t ti);

gen_op atg_gen_g_cond_rv(const te *ci);

atg_stat atg_te_init(atg *t, gen *g, te *restrict an, err **e, te *restrict type, size_t l, uint32_t ti);

atg_stat aply_te_g_idx(atg *t, te *an, err **e, size_t *n, te **lte);

atg_stat z_lte_id(atg *t, te *an, err **e, const te *zn, te **lte, size_t *id);

atg_stat call_npr(gen_op *go, const te *an);

atg_stat atg_aply_acc(atg *t, gen *g, te *restrict an, err **e, te *restrict vlte, te **i, uint32_t erlb);

gen_stat atg_rt_err_u(gen *g, uint32_t ri, uint32_t si, uint32_t ti, mc *fname, uint16_t lno, uint32_t cno, const char *msg);

atg_stat un_inv_a(atg *t, gen *g, te *an, err **e, mc *s, uint32_t ri, uint32_t si, uint32_t ti);

gen_stat aply_e_vr_err_u(gen *g, te *restrict i, te *restrict v, uint32_t erlb, uint32_t glb, uint32_t ei, uint32_t esi, uint32_t si, uint32_t ii, mc *fname, uint16_t lno, uint32_t cno);

gen_stat aply_e_vr_u(gen *g, te *restrict i, te *restrict v, uint32_t erlb, uint32_t glb, uint32_t el, uint32_t ui, uint32_t ei, uint32_t esi, uint32_t si, uint32_t ii, uint32_t vi, mc *fname, uint16_t lno, uint32_t cno, x64_type xt, bool ref, uint64_t esym, uint64_t vsym);

atg_stat atg_err_r(atg *t, gen *g, te *restrict an, err **e, const te *restrict s, const char *msg);

extern const char *atg_un_inv_str;

extern const char *atg_user_inv_str;

extern const char *atg_vr_inv_str;

extern const char *atg_div_inv_str;

void atg_rt_err_p(void *p, uint32_t idnt);

void atg_rt_err_f(void *p);

extern const char *atg_dump_strs[TYPE(_END)];

extern const char *atg_dump_end;

extern const char *atg_dump_idnt;

extern const char *atg_dump_nl;

extern const char *atg_sym_str;

atg *atg_b(atg *t);

typedef err *atg_jit(te *x);

err *atg_y(const atg *t, tbl *volatile et, const uint8_t *m, size_t ep);
