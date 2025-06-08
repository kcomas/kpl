
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

#define INST_RMR(N) static bool as_##N##_rmr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rmr(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RMR(mov);

#define INST_RMBR(N) static bool as_##N##_rmbr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbr(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMBR(mov);

#define INST_RRM(N) static bool as_##N##_rrm(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rrm(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RRM(mov);
INST_RRM(cmp);

#define INST_RRMB(N) static bool as_##N##_rrmb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rrmb(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RRMB(mov);

bool as_mov_rv(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) ci;
    (void) arg3;
    (void) arg4;
    return x64_mov_rq(p, m, arg1->d[1].u3, arg2->d[1]) == X64_STAT(OK);
}

as *as_r_b(as *a) {
    as_op_a(a, AS_X64(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_push_r, NULL);
    as_op_a(a, AS_X64(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_pop_r, NULL);
    as_op_a(a, AS_X64(CALL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_call_r, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_mov_rr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_mov_rmr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(B), ARG_ID(R), ARG_ID(N), as_mov_rmbr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_mov_rrm, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_mov_rrmb, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_add_rr, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_add_rb, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_sub_rr, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), as_sub_rb, NULL);
    as_op_a(a, AS_X64(NEG), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_neg_r, NULL);
    as_op_a(a, AS_X64(INC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_inc_r, NULL);
    as_op_a(a, AS_X64(DEC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_dec_r, NULL);
    as_op_a(a, AS_X64(AND), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_and_rr, NULL);
    as_op_a(a, AS_X64(XOR), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_xor_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_cmp_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_cmp_rrm, NULL);
    return a;
}
