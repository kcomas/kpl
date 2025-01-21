
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

#define AS_X64(N) AS_X64_##N

typedef enum {
    AS_X64(_START),
    AS_X64(NOP),
    AS_X64(RET),
    AS_X64(LEAVE),
    AS_X64(PUSH),
    AS_X64(POP),
    AS_X64(MOV),
    AS_X64(CALL),
    AS_X64(INC),
    AS_X64(DEC),
    AS_X64(XOR),
    AS_X64(CMP),
    AS_X64(JMP),
    AS_X64(JB),
    AS_X64(JNAE),
    AS_X64(JC),
    AS_X64(JNB),
    AS_X64(JAE),
    AS_X64(JNC),
    AS_X64(JZ),
    AS_X64(JE),
    AS_X64(JNZ),
    AS_X64(JNE),
    AS_X64(JBE),
    AS_X64(JNA),
    AS_X64(JNBE),
    AS_X64(JA),
    AS_X64(JL),
    AS_X64(JNGE),
    AS_X64(JNL),
    AS_X64(JGE),
    AS_X64(JLE),
    AS_X64(JNG),
    AS_X64(JNLE),
    AS_X64(JG),
    AS_X64(_END)
} as_inst;

void as_code_p(const as *a, const uint8_t *m);

void as_op_p(tbl *ot, bool args, size_t idnt);

as *as_b(as *a);

