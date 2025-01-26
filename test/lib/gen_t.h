
#pragma once

#include "../../src/lib/gen_x64.h"

lst *gen_mklst(void);

tbl *gen_cls_info_tbl(void);

tbl *gen_op_tbl(size_t bcks);

void gen_entry_f(void *p);

void code_entry_f(void *p);
