
#pragma once

#include "../../src/lib/gen_x64.h"
#include "as_t.h"

lst *gen_mklst(void);

tbl *gen_cls_info_tbl(void);

tbl *gen_op_tbl(size_t bcks);

te *gen_call_m(size_t n, ...);

te *gen_lbl(size_t id);

te *gen_lbl_m(size_t *id);

te *gen_arg(x64_type t, size_t id);

te *gen_tmp(x64_type t, size_t id);

te *gen_data(x64_type t, un d);

void gen_entry_f(void *p);

void gen_code_entry_f(void *p);

void gen_st_atmf(void *p);

void gen_st_latf(void *p);
