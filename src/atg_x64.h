
#pragma once

#include "./lib/atg.h"
#include "opt.h"
#include "./lib/gen_x64.h"

void atg_tbl_p(const tbl *t, ast_cls cls, size_t idnt);

bool atg_x64_enq(const te *an);

x64_type atg_x64_g_t(const te *type);

atg_stat atg_err(atg_stat stat, te *an, te **e);

atg *atg_b(atg *t);
