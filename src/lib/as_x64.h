
#pragma once

#include <stdio.h>
#include "x64.h"
#include "as.h"

#define ARG_ID(N) ARG_ID_##N

typedef enum {
    ARG_ID(N), // none passed as NULL
    ARG_ID(R), // reg
    ARG_ID(RM), // reg mem
    ARG_ID(L), // label
    ARG_ID(M), // mem
    ARG_ID(B), // byte
    ARG_ID(W), // word
    ARG_ID(DW), // double word
    ARG_ID(QW) // quad word
} arg_id;

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

void as_code_p(const as *a, const uint8_t *m);

void as_op_p(tbl *ot, bool args, size_t idnt);

as *as_b(as *a);

