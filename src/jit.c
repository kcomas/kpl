
#include "jit.h"

bool reg_is_upper(reg r) {
    return (r >= R(8) && r <= R(15)) || (r >= XMM(8) && r <= XMM(0));
}

jit_stat jit_a(size_t *p, uint8_t *m, uint8_t b) {
    m[(*p)++] = b;
    return JIT_STAT(OK);
}

jit_stat jit_b(size_t *p, uint8_t *m, size_t len, ...) {
    va_list args;
    va_start(args, len);
    for (size_t i = 0; i < len; i++) jit_a(p, m, (uint8_t) va_arg(args, int));
    va_end(args);
    return JIT_STAT(OK);
}

jit_stat jit_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]) {
    for (size_t i = 0; i < len; i++) jit_a(p, m, b[i]);
    return JIT_STAT(OK);
}

jit_stat jit_ret(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC3);
}

jit_stat jit_leave(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC9);
}

jit_stat jit_push(size_t *p, uint8_t *m, reg r) {
    if (r <= R(DI)) return jit_b(p, m, 2, REX(W), 0x50 + r);
    else if (r >= R(8) && r <= R(15)) return jit_b(p, m, 2, REX(W) | REX(R), 0x50 + r);
    return JIT_STAT(INV_REG);
}

jit_stat jit_pop(size_t *p, uint8_t *m, reg r) {
    if (r <= R(DI)) return jit_b(p, m, 2, REX(W), 0x58 + r);
    else if (r >= R(8) && r <= R(15)) return jit_b(p, m, 2, REX(W) | REX(R), 0x58 + r);
    return JIT_STAT(INV_REG);
}

jit_stat jit_movrr(size_t *p, uint8_t *m, reg d, reg s) {
    uint8_t rex = REX(W);
    if (d > R(15) || s > R(15)) return JIT_STAT(INV_REG);
    if (reg_is_upper(d)) rex |= REX(B);
    if (reg_is_upper(s)) rex |= REX(R);
    return jit_b(p, m, 3, rex, 0x89, 0xC0 + 8 * s + d);
}

jit_stat jit_addrb(size_t *p, uint8_t *m, reg r, int8_t b) {
    uint8_t rex = REX(W);
    if (reg_is_upper(r)) rex |= REX(R);
    return jit_b(p, m, 4, rex, 0x83, 0xC0 + r, b);
}

jit_stat jit_subrb(size_t *p, uint8_t *m, reg r, int8_t b) {
    uint8_t rex = REX(W);
    if (reg_is_upper(r)) rex |= REX(R);
    return jit_b(p, m, 4, rex, 0x83, 0xE8 + r, b);
}
