
#pragma once

#include "./lib/fld.h"
#include "ast.h"

fld_stat fld_err(const fld *f, te *an, err **e, const char *m);

fld *fld_b(fld *f);
