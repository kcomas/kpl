
#include "../as_x64.h"

// gen regs

#define INST_R(N) static bool as_##N##_r(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_r(p, m, arg1->d[1].u3) == X64_STAT(OK); \
}

INST_R(push);
INST_R(pop);
INST_R(call);
INST_R(inc);
INST_R(dec);
INST_R(neg);
INST_R(mul);
INST_R(imul);
INST_R(div);
INST_R(idiv);

#define INST_RM(N) static bool as_##N##_rm(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rm(p, m, arg1->d[1].u3) == X64_STAT(OK); \
}

INST_RM(inc);
INST_RM(dec);
INST_RM(imul);

#define INST_RR(N) static bool as_##N##_rr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rr(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RR(mov);
INST_RR(add);
INST_RR(sub);
INST_RR(and);
INST_RR(xor);
INST_RR(cmp);

#define INST_RB(N) static bool as_##N##_rb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rb(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RB(add);
INST_RB(sub);
INST_RB(and);
INST_RB(cmp);

#define INST_RMB(N) static bool as_##N##_rmb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rmb(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RMB(cmp);
INST_RMB(inc);
INST_RMB(dec);
INST_RMB(add);
INST_RMB(imul);

#define INST_RMBB(N) static bool as_##N##_rmbb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbb(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMBB(cmp);
INST_RMBB(add);

#define INST_RMBD(N) static bool as_##N##_rmbd(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbd(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u5) == X64_STAT(OK); \
}

INST_RMBD(mov);
INST_RMBD(cmp);

#define INST_RD(N) static bool as_##N##_rd(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rd(p, m, arg1->d[1].u3, arg2->d[1].u5) == X64_STAT(OK); \
}

INST_RD(add);
INST_RD(sub);
INST_RD(cmp);

#define INST_RMR(N) static bool as_##N##_rmr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rmr(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RMR(mov);
INST_RMR(cmp);
INST_RMR(add);

#define INST_RMBR(N) static bool as_##N##_rmbr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbr(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMBR(mov);
INST_RMBR(cmp);
INST_RMBR(add);

#define INST_RMDR(N) static bool as_##N##_rmdr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmdr(p, m, arg1->d[1].u3, arg2->d[1].u5, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMDR(cmp);

#define INST_RRM(N) static bool as_##N##_rrm(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rrm(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RRM(mov);
INST_RRM(add);
INST_RRM(cmp);

#define INST_RRMB(N) static bool as_##N##_rrmb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rrmb(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RRMB(mov);
INST_RRMB(lea);
INST_RRMB(add);
INST_RRMB(cmp);

#define INST_RRMD(N) static bool as_##N##_rrmd(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rrmd(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u5) == X64_STAT(OK); \
}

INST_RRMD(mov);
INST_RRMD(cmp);

#define INST_RI(N) static bool as_##N##_ri(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) arg3; \
    (void) arg4; \
    as_dq_a(a, ci, sizeof(uint64_t), arg2->d[1], as_x64_dq); /* TODO fn to get sizeof*/ \
    return x64_##N##_ri(p, m, arg1->d[1].u3, 0) == X64_STAT(OK); \
}

INST_RI(cmp);

#define INST_IR(N) static bool as_##N##_ir(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) arg3; \
    (void) arg4; \
    as_dq_a(a, ci, sizeof(uint64_t), arg1->d[1], as_x64_dq); /* TODO fn to get sizeof*/ \
    return x64_##N##_ir(p, m, 0, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_IR(cmp);

#define INST_RS(N) bool as_##N##_rs(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) arg3; \
    (void) arg4; \
    mc *s = arg2->d[1].p; \
    as_dq_a(a, ci, s->l, P(s->d), as_x64_dqs); \
    return x64_##N##_ri(p, m, arg1->d[1].u3, 0) == X64_STAT(OK); \
}

INST_RS(mov);
INST_RS(lea);

bool as_mov_rv(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) ci;
    (void) arg3;
    (void) arg4;
    return x64_mov_rq(p, m, arg1->d[1].u3, arg2->d[1]) == X64_STAT(OK);
}

// only can be used after the lbl is defined
bool as_mov_rl(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) ci;
    (void) arg3;
    (void) arg4;
    te *lblc = as_lbl_g_c(a, arg2->d[1].u5);
    if (!lblc || !lblc->d[9].u6) return false;
    return x64_mov_rq(p, m, arg1->d[1].u3, P(&m[lblc->d[8].u6])) == X64_STAT(OK);
}

void as_r_b(as *a) {
    as_op_a(a, AS_X64(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_push_r, NULL);
    as_op_a(a, AS_X64(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_pop_r, NULL);
    as_op_a(a, AS_X64(CALL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_call_r, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_mov_rr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(L), ARG_ID(N), ARG_ID(N), as_mov_rl, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(S), ARG_ID(N), ARG_ID(N), as_mov_rs, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_mov_rmr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(B), ARG_ID(R), ARG_ID(N), as_mov_rmbr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(B), ARG_ID(DW), ARG_ID(N), as_mov_rmbd, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_mov_rrm, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_mov_rrmb, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(DW), ARG_ID(N), as_mov_rrmd, NULL);
    as_op_a(a, AS_X64(LEA), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_lea_rrmb, NULL);
    as_op_a(a, AS_X64(LEA), ARG_ID(R), ARG_ID(S), ARG_ID(N), ARG_ID(N), as_lea_rs, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_add_rr, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(RM), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_add_rmr, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(RM), ARG_ID(B), ARG_ID(R), ARG_ID(N), as_add_rmbr, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_add_rrm, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_add_rrmb, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_add_rb, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(DW), ARG_ID(N), ARG_ID(N), as_add_rd, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(RM), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_add_rmb, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(RM), ARG_ID(B), ARG_ID(B), ARG_ID(N), as_add_rmbb, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_sub_rr, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_sub_rb, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(DW), ARG_ID(N), ARG_ID(N), as_sub_rd, NULL);
    as_op_a(a, AS_X64(NEG), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_neg_r, NULL);
    as_op_a(a, AS_X64(MUL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_mul_r, NULL);
    as_op_a(a, AS_X64(IMUL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_imul_r, NULL);
    as_op_a(a, AS_X64(IMUL), ARG_ID(RM), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_imul_rm, NULL);
    as_op_a(a, AS_X64(IMUL), ARG_ID(RM), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_imul_rmb, NULL);
    as_op_a(a, AS_X64(DIV), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_div_r, NULL);
    as_op_a(a, AS_X64(IDIV), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_idiv_r, NULL);
    as_op_a(a, AS_X64(INC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_inc_r, NULL);
    as_op_a(a, AS_X64(INC), ARG_ID(RM), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_inc_rm, NULL);
    as_op_a(a, AS_X64(INC), ARG_ID(RM), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_inc_rmb, NULL);
    as_op_a(a, AS_X64(DEC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_dec_r, NULL);
    as_op_a(a, AS_X64(DEC), ARG_ID(RM), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_dec_rm, NULL);
    as_op_a(a, AS_X64(DEC), ARG_ID(RM), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_dec_rmb, NULL);
    as_op_a(a, AS_X64(AND), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_and_rr, NULL);
    as_op_a(a, AS_X64(AND), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_and_rb, NULL);
    as_op_a(a, AS_X64(XOR), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_xor_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_cmp_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_cmp_rrm, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_cmp_rrmb, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(RM), ARG_ID(DW), ARG_ID(N), as_cmp_rrmd, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_cmp_rmr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(B), ARG_ID(R), ARG_ID(N), as_cmp_rmbr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(DW), ARG_ID(R), ARG_ID(N), as_cmp_rmdr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_cmp_rb, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_cmp_rmb, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(B), ARG_ID(B), ARG_ID(N), as_cmp_rmbb, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(RM), ARG_ID(B), ARG_ID(DW), ARG_ID(N), as_cmp_rmbd, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(DW), ARG_ID(N), ARG_ID(N), as_cmp_rd, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_cmp_ri, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(QW), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_cmp_ir, NULL);
}
