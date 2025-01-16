
#pragma once

#include <stdio.h>
#include "../../src/lib/jit.h"
#include "../../src/lib/as.h"

lst *as_mklst(void);

tbl *as_mktbl(void);

void as_code_p(const as *const a);

void label_entry_f(void *p);

void op_entry_f(void *p);

void code_entry_f(void *p);
