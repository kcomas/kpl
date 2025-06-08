
#pragma once

#include <stdio.h>
#include "../../src/lib/jit.h"
#include "../../src/lib/as.h"

#define AS_INST(N) AS_INST_##N

typedef enum {
    AS_INST(NOP),
    AS_INST(RET),
    AS_INST(LEAVE),
    AS_INST(PUSH),
    AS_INST(POP),
    AS_INST(MOV)
} as_inst;

te *as_arg_r(size_t rid);

lst *as_mklst(void);

tbl *as_mktbl(void);

void as_code_p(const as *const a, const uint8_t *const m);

void as_op_p(tbl *const ot, bool args, size_t idnt);

bool as_ret(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4);

bool as_push_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4);

bool as_pop_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4);

bool as_mov_rr(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4);

void label_entry_f(void *p);

void op_entry_f(void *p);

void code_entry_f(void *p);
