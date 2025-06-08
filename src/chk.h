
#pragma once

#include "./lib/chk.h"
#include "fld.h"

void chk_p(const tbl *ct, size_t idnt);

chk_stat chk_err(chk_stat stat, te *an, te **e);

chk *chk_b(chk *c);
