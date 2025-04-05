
#include "../as_x64.h"

// ops between gen regs and sse

#define INST_XR(N) static bool as_##N##_xr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_xr(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_XR(movq);
INST_XR(cvtsi2sd);

#define INST_XRM(N) static bool as_##N##_xrm(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_xrm(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_XRM(movq);
INST_XRM(movsd);

#define INST_XRMB(N) static bool as_##N##_xrmb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_xrmb(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_XRMB(movq);
INST_XRMB(movsd);

#define INST_RX(N) static bool as_##N##_rx(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rx(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RX(movq);

#define INST_RMX(N) static bool as_##N##_rmx(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rmx(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RMX(movq);
INST_RMX(movsd);

#define INST_RMBX(N) static bool as_##N##_rmbx(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbx(p, m, arg1->d[1].u3, arg2->d[1].u3, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RMBX(movq);
INST_RMBX(movsd);

as *as_rx_b(as *a) {
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_movq_xr, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_movq_xrm, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(RM), ARG_ID(N), ARG_ID(N), as_movsd_xrm, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_movq_xrmb, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(RM), ARG_ID(B), ARG_ID(N), as_movsd_xrmb, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(R), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movq_rx, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(RM), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movq_rmx, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(RM), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movsd_rmx, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(RM), ARG_ID(B), ARG_ID(X),  ARG_ID(N), as_movq_rmbx, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(RM), ARG_ID(B), ARG_ID(X),  ARG_ID(N), as_movsd_rmbx, NULL);
    as_op_a(a, AS_X64(CVTSI2SD), ARG_ID(X), ARG_ID(R), ARG_ID(N), ARG_ID(N), as_cvtsi2sd_xr, NULL);
    return a;
}
