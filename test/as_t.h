
#pragma once

#include "../src/lib/as_x64.h"
#include "t.h"

#define UNLOCK() x64_mp_w(JIT_P, &p, m)

#define LOCK() x64_mp_rx(JIT_P, &p, m)

extern const alfr am;

extern as *ba;

te *as_arg_r(as *a, size_t rid);

te *as_arg_rm(as *a, size_t rmid);

te *as_arg_rs(as *a, size_t rid, size_t sid);

te *as_arg_l(as *a, size_t lid);

te *as_arg_s(as *a, mc *s);

te *as_arg_qw(as *a, un v);

te *as_arg_b(as *a, uint8_t b);

lst *as_mklst(void);

tbl *as_arg_tbl(void);

tbl *as_op_tbl(size_t bcks);
