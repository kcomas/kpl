
#pragma once

#include "./lib/atg.h"
#include "opt.h"
#include "./lib/gen_x64.h"

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt);

bool atg_x64_enq(const te *an);

x64_type atg_x64_g_t(const te *type);

atg_stat atg_err(const atg *t, te *an, err **e, const char *m);

te *var_arg(gen *g, te *lte, x64_type xt);

extern const char *atg_dump_strs[TYPE(_END)];

atg *atg_b(atg *t);

typedef err *atg_jit(te *x);

err *atg_z(const atg *t, tbl *volatile et, const uint8_t *m, size_t ep);
