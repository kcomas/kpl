
#pragma once

#include <stdio.h>
#include "../../src/lib/x64.h"
#include "../../src/lib/as.h"

#define AS_INST(N) AS_INST_##N

typedef enum {
    AS_INST(_START),
    AS_INST(NOP),
    AS_INST(RET),
    AS_INST(LEAVE),
    AS_INST(PUSH),
    AS_INST(POP),
    AS_INST(MOV),
    AS_INST(CALL),
    AS_INST(XOR),
    AS_INST(CMP),
    // jmps
    AS_INST(JMP),
    AS_INST(JNL),
    AS_INST(JGE),
    AS_INST(_END)
} as_inst;

te *as_arg_r(size_t rid);

te *as_arg_l(size_t lid);

te *as_arg_qw(un v);

te *as_arg_b(uint8_t b);

lst *as_mklst(void);

tbl *as_mktbl(void);

void as_code_p(const as *const a, const uint8_t *const m);

void as_op_p(tbl *const ot, bool args, size_t idnt);

as *as_b(void);

void label_entry_f(void *p);

void op_entry_f(void *p);

void code_entry_f(void *p);
