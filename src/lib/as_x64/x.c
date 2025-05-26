
#include "../as_x64.h"

// sse

#define INST_XX(N) static bool as_##N##_xx(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_xx(p, m, arg1->d[1].u3, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_XX(movq);
INST_XX(movsd);
INST_XX(addsd);
INST_XX(subsd);
INST_XX(mulsd);
INST_XX(divsd);
INST_XX(pxor);
INST_XX(ucomisd);
INST_XX(comisd);

#define INST_XI(N) static bool as_##N##_xi(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    as_dq_a(a, ci, sizeof(double), arg2->d[1], as_x64_dq); /* TODO fn to get sizeof*/ \
    return x64_##N##_xi(p, m, arg1->d[1].u3, 0) == X64_STAT(OK); \
}

INST_XI(movq);
INST_XI(movsd);
INST_XI(addsd);
INST_XI(subsd);
INST_XI(mulsd);
INST_XI(ucomisd);
INST_XI(comisd);

void as_x_b(as *a) {
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movq_xx, NULL);
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_movq_xi, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_movsd_xi, NULL);
    as_op_a(a, AS_X64(MOVSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movsd_xx, NULL);
    as_op_a(a, AS_X64(ADDSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_addsd_xx, NULL);
    as_op_a(a, AS_X64(ADDSD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_addsd_xi, NULL);
    as_op_a(a, AS_X64(SUBSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_subsd_xx, NULL);
    as_op_a(a, AS_X64(SUBSD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_subsd_xi, NULL);
    as_op_a(a, AS_X64(MULSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_mulsd_xx, NULL);
    as_op_a(a, AS_X64(MULSD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_mulsd_xi, NULL);
    as_op_a(a, AS_X64(DIVSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_divsd_xx, NULL);
    as_op_a(a, AS_X64(PXOR), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_pxor_xx, NULL);
    as_op_a(a, AS_X64(UCOMISD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_ucomisd_xx, NULL);
    as_op_a(a, AS_X64(UCOMISD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_ucomisd_xi, NULL);
    as_op_a(a, AS_X64(COMISD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_comisd_xx, NULL);
    as_op_a(a, AS_X64(COMISD), ARG_ID(X), ARG_ID(QW), ARG_ID(N), ARG_ID(N), as_comisd_xi, NULL);
}
