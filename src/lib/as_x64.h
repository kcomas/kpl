
#pragma once

#include <stdio.h>
#include "x64.h"
#include "as.h"

const char *reg_str(size_t rid);

#define ARG_ID(N) ARG_ID_##N

typedef enum {
    ARG_ID(N), // none passed as NULL
    ARG_ID(R), // gen reg
    ARG_ID(RM), // reg mem
    ARG_ID(RS), // reg scale
    ARG_ID(X), // sse
    ARG_ID(L), // label
    ARG_ID(M), // mem
    ARG_ID(B), // byte
    ARG_ID(W), // word
    ARG_ID(DW), // double word
    ARG_ID(QW), // quad word
    ARG_ID(S), // mc
} arg_id;

// arg te[id;data]
te *as_arg_i(as *a, arg_id id, un d);

#define AS_X64(N) AS_X64_##N

typedef enum {
    AS_X64(_START),
    AS_X64(NOP),
    AS_X64(RET),
    AS_X64(LEAVE),
    AS_X64(PUSH),
    AS_X64(POP),
    AS_X64(CALL),
    AS_X64(MOV),
    AS_X64(MOVQ),
    AS_X64(MOVSD),
    AS_X64(LEA),
    AS_X64(ADD),
    AS_X64(ADDSD),
    AS_X64(SUB),
    AS_X64(SUBSD),
    AS_X64(NEG),
    AS_X64(INC),
    AS_X64(DEC),
    AS_X64(MUL),
    AS_X64(IMUL),
    AS_X64(DIV),
    AS_X64(IDIV),
    AS_X64(MULSD),
    AS_X64(DIVSD),
    AS_X64(CVTSI2SD),
    AS_X64(AND),
    AS_X64(XOR),
    AS_X64(PXOR),
    AS_X64(CMP),
    AS_X64(UCOMISD),
    AS_X64(COMISD),
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
    AS_X64(SETB),
    AS_X64(SETNAE),
    AS_X64(SETC),
    AS_X64(SETNB),
    AS_X64(SETAE),
    AS_X64(SETNC),
    AS_X64(SETZ),
    AS_X64(SETE),
    AS_X64(SETNZ),
    AS_X64(SETNE),
    AS_X64(SETBE),
    AS_X64(SETNA),
    AS_X64(SETNBE),
    AS_X64(SETA),
    AS_X64(SETL),
    AS_X64(SETNGE),
    AS_X64(SETNL),
    AS_X64(SETGE),
    AS_X64(SETLE),
    AS_X64(SETNG),
    AS_X64(SETNLE),
    AS_X64(SETG),
    AS_X64(LOOP),
    AS_X64(_END)
} as_inst;

void as_op_p(const tbl *ot, bool args, size_t idnt);

void as_code_i_p(const te *c, const uint8_t *m);

void as_data_p(const as *a, const uint8_t *m);

void as_code_p(const as *a, const uint8_t *m);

err_d_p *as_x64_err_g_p(as_stat stat);

bool as_x64_dq(as *a, size_t *p, uint8_t *m, te *dqe);

bool as_x64_dqs(as *a, size_t *p, uint8_t *m, te *dqe);

#define AS_X64_RS_R 1
#define AS_X64_RS_S 0

un as_x64_rs(size_t rid, size_t sid);

as *as_b(as *a);
