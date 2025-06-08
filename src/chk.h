
#pragma once

#include "./lib/chk.h"
#include "fld.h"

void chk_p(const tbl *ct, size_t idnt);

chk_stat chk_err(const chk *c, te *an, err **e, const char *m);

te *chk_g_pn_lte(te *an, const mc *s);

chk_stat chk_op_l_z_r(chk *c, te *an, err **e);

chk_stat chk_op_lr_teq(chk *c, te *an, err **e);

chk_stat chk_op_lers_us_u(chk *c, te *an, err **e);

chk_stat chk_op_lers_fs_f(chk *c, te *an, err **e);

chk_stat chk_op_mon(chk *c, te *an, err **e);

chk *chk_b(chk *c);
