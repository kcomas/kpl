
#include "../as_x64.h"

// sib r

#define INST_RMOR(N) static bool as_##N##_rmor(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    reg r = u3_g_o(arg2->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg2->d[1], AS_X64_RS_S); \
    return x64_##N##_rmor(p, m, arg1->d[1].u3, r, x, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMOR(mov);

#define INST_RMOBR(N) static bool as_##N##_rmobr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    reg r = u3_g_o(arg2->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg2->d[1], AS_X64_RS_S); \
    return x64_##N##_rmobr(p, m, arg1->d[1].u3, r, x, arg3->d[1].u3, arg4->d[1].u3) == X64_STAT(OK); \
}

INST_RMOBR(mov);

#define INST_RRMO(N) static bool as_##N##_rrmo(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    reg r = u3_g_o(arg3->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg3->d[1], AS_X64_RS_S); \
    return x64_##N##_rrmo(p, m, arg1->d[1].u3, arg2->d[1].u3, r, x) == X64_STAT(OK); \
}

INST_RRMO(mov);
INST_RRMO(lea);

#define INST_RRMOB(N) static bool as_##N##_rrmob(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    reg r = u3_g_o(arg3->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg3->d[1], AS_X64_RS_S); \
    return x64_##N##_rrmob(p, m, arg1->d[1].u3, arg2->d[1].u3, r, x, arg4->d[1].u3) == X64_STAT(OK); \
}

INST_RRMOB(mov);
INST_RRMOB(lea);

void as_ro_b(as *a) {
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(RS), ARG_ID(R), ARG_ID(N), as_mov_rmor, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(RS), ARG_ID(B), ARG_ID(R), as_mov_rmobr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(RS), ARG_ID(N), as_mov_rrmo, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(RS), ARG_ID(B), as_mov_rrmob, NULL);
    as_op_a(a, AS_X64(LEA), ARG_ID(R), ARG_ID(RM), ARG_ID(RS), ARG_ID(N), as_lea_rrmo, NULL);
    as_op_a(a, AS_X64(LEA), ARG_ID(R), ARG_ID(RM), ARG_ID(RS), ARG_ID(B), as_lea_rrmob, NULL);
}
