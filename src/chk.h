
#pragma once

#include "./lib/chk.h"
#include "fld.h"

void chk_p(const tbl *ct, size_t idnt);

chk_stat chk_err(chk *c, te *an, err **e, const char *m);

te *chk_g_pn_lte(te *an, const mc *s);

chk *chk_b(chk *c);
