
#pragma once

#include "./lib/fld.h"
#include "ast.h"
#include "opt.h"
#include "z.h"

fld_stat fld_err(const fld *f, te *an, err **e, const char *m);

void fld_s_st_et_f(void *p);

fld *fld_b(fld *f);
