
#pragma once

#include "../src/lib/as_x64.h"
#include "t.h"

te *as_arg_r(as *a, size_t rid);

te *as_arg_rm(as *a, size_t rmid);

te *as_arg_l(as *a, size_t lid);

te *as_arg_qw(as *a, un v);

te *as_arg_b(as *a, uint8_t b);

lst *as_mklst(void);

tbl *as_arg_tbl(void);

tbl *as_op_tbl(size_t bcks);
