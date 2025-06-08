
#include "../as_x64.h"

// sib x

#define INST_XRMO(N) static bool as_##N##_xrmo(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    reg r = u3_g_o(arg3->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg3->d[1], AS_X64_RS_S); \
    return x64_##N##_xrmo(p, m, arg1->d[1].u3, arg2->d[1].u3, r, x) == X64_STAT(OK); \
}

INST_XRMO(movsd);

#define INST_XRMOB(N) static bool as_##N##_xrmob(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    reg r = u3_g_o(arg3->d[1], AS_X64_RS_R); \
    scale x = u3_g_o(arg3->d[1], AS_X64_RS_S); \
    return x64_##N##_xrmob(p, m, arg1->d[1].u3, arg2->d[1].u3, r, x, arg4->d[1].u3) == X64_STAT(OK); \
}

INST_XRMOB(movsd);

void as_xo_b(as *a) {
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(RM), ARG_ID(RS), ARG_ID(N), as_movsd_xrmo, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(RM), ARG_ID(RS), ARG_ID(B), as_movsd_xrmob, NULL);
}
