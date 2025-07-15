
#include "../as_x64.h"

#define INST_I(N) static bool as_##N##_i(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    as_dq_a(a, ci, sizeof(uint64_t), arg1->d[1], as_x64_dq); /* TODO fn to get sizeof*/ \
    return x64_##N##_i(p, m, 0) == X64_STAT(OK); \
}

INST_I(idiv);

void as_i_b(as *a) {
    as_op_a(a, AS_X64(IDIV), ARG_ID(QW), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_idiv_i, NULL);
}
