
#pragma once

#include "./lib/atg.h"
#include "opt.h"
#include "./lib/gen_x64.h"
#include "./lib/gen_type.h"

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt);

bool atg_x64_enq(const te *an);

x64_type atg_x64_g_t(const te *type);

x64_type type_g_x64_type(te *tn);

atg_stat atg_err(const atg *t, te *an, err **e, const char *m);

te *atg_te_idx_d(gen *g, uint32_t id);

te *atg_vr_idx_d(gen *g, uint32_t id);

te *var_arg(gen *g, te *lte, x64_type xt);

atg_stat call_npr(gen_op *go, const te *an);

extern const char *atg_dump_strs[TYPE(_END)];

extern const char *atg_dump_end;

extern const char *atg_dump_idnt;

atg *atg_b(atg *t);

typedef err *atg_jit(te *x);

err *atg_z(const atg *t, tbl *volatile et, const uint8_t *m, size_t ep);
