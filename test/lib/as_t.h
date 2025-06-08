
#pragma once

#include "../../src/lib/as_x64.h"

te *as_arg_r(size_t rid);

te *as_arg_rm(size_t rmid);

te *as_arg_l(size_t lid);

te *as_arg_qw(un v);

te *as_arg_b(uint8_t b);

lst *as_mklst(void);

tbl *as_mktbl(void);

void label_entry_f(void *p);

void op_entry_f(void *p);

void code_entry_f(void *p);
