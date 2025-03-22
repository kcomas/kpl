
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
INST_XX(addsd);
INST_XX(subsd);

as *as_x_b(as *a) {
    as_op_a(a, AS_X64(MOVQ), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_movq_xx, NULL);
    as_op_a(a, AS_X64(ADDSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_addsd_xx, NULL);
    as_op_a(a, AS_X64(SUBSD), ARG_ID(X), ARG_ID(X), ARG_ID(N), ARG_ID(N), as_subsd_xx, NULL);
    return a;
}
