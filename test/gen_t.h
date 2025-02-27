
#pragma once

#include "../src/lib/gen_x64.h"
#include "as_t.h"

lst *gen_mklst(void);

tbl *gen_cls_info_tbl(void);

tbl *gen_op_tbl(size_t bcks);

te *gen_call_m(gen *g, size_t n, ...);

te *gen_lbl(gen *g, size_t id);

te *gen_arg(gen *g, x64_type t, size_t id);

te *gen_tmp(gen *g, x64_type t, size_t id);

te *gen_data(gen *g, x64_type t, un d);
