
#include "jit.h"

static bool reg_is_upper(reg r) {
    return (r >= R(8) && r <= R(15)) || (r >= XMM(8) && r <= XMM(0));
}

static uint8_t rid(reg r) {
    return r % 8;
}

static uint8_t modrm(uint8_t mod, reg d, reg s) {
    return mod + 8 * rid(s) + rid(d);
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

jit_stat jit_d(size_t *p, uint8_t *m, void *v) {
    memset(m + *p, 0, sizeof(void*));
    memcpy(m + *p, &v, sizeof(void*));
    *p += sizeof(void*);
    return JIT_STAT(OK);
}

jit_stat jit_ret(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC3);
}

jit_stat jit_leave(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC9);
}

#define SET_REX uint8_t rex = REX(W); \
    if (r > R(15)) return JIT_STAT(INV_REG); \
    if (reg_is_upper(r)) rex |= REX(B)

#define SET_REX2 uint8_t rex = REX(W); \
    if (d > R(15) || s > R(15)) return JIT_STAT(INV_REG); \
    if (reg_is_upper(d)) rex |= REX(B); \
    if (reg_is_upper(s)) rex |= REX(R)

jit_stat jit_push(size_t *p, uint8_t *m, reg r) {
    SET_REX;
    return jit_b(p, m, 2, rex, 0x50 + rid(r));
}

jit_stat jit_pop(size_t *p, uint8_t *m, reg r) {
    SET_REX;
    return jit_b(p, m, 2, rex, 0x58 + rid(r));
}

jit_stat jit_callr(size_t *p, uint8_t *m, reg r) {
    SET_REX;
    return jit_b(p, m, 3, rex, 0xFF, 0xD0 + rid(r));
}

jit_stat jit_movrq(size_t *p, uint8_t *m, reg r, void *v) {
    SET_REX;
    jit_b(p, m, 2, rex, 0xB8 + rid(r));
    return jit_d(p, m, v);
}

jit_stat jit_movrr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x89, modrm(0xC0, d, s));
}

jit_stat jit_movrar(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    if (d == R(SP)) return jit_b(p, m, 4, rex, 0x89, 0x04 + rid(s) * 8, 0x24);
    return jit_b(p, m, 3, rex, 0x89, modrm(0x00, d, s));
}

jit_stat jit_movrabr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    SET_REX2;
    if (d == R(SP)) return jit_b(p, m, 5, rex, 0x89, 0x04 + rid(s) * 8, 0x24, dsp);
    return jit_b(p, m, 4, rex, 0x89, modrm(0x40, d, s), dsp);
}

jit_stat jit_movrra(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    if (s == R(SP)) {
        rex = REX(W);
        if (reg_is_upper(d)) rex |= REX(R);
        return jit_b(p, m, 4, rex, 0x8B, 0x04 + rid(d) * 8, 0x24);
    }
    return jit_b(p, m, 3, rex, 0x8B, modrm(0x00, d, s));
}

jit_stat jit_movrrab(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    SET_REX2;
    if (s == R(SP)) {
        rex = REX(W);
        if (reg_is_upper(d)) rex |= REX(R);
        return jit_b(p, m, 5, rex, 0x8B, 0x44 + rid(d) * 8, 0x24, dsp);
    }
    return jit_b(p, m, 4, rex, 0x8B, modrm(0x40, d, s), dsp);
}

jit_stat jit_incr(size_t *p, uint8_t *m, reg r) {
    SET_REX;
    return jit_b(p, m, 3, rex, 0xFF, 0xC0 + rid(r));
}

jit_stat jit_addrb(size_t *p, uint8_t *m, reg r, int8_t b) {
    SET_REX;
    return jit_b(p, m, 4, rex, 0x83, 0xC0 + rid(r), b);
}

jit_stat jit_addrr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x01, modrm(0xC0, d, s));
}

jit_stat jit_decr(size_t *p, uint8_t *m, reg r) {
    SET_REX;
    return jit_b(p, m, 3, rex, 0xFF, 0xC8 + rid(r));
}

jit_stat jit_subrb(size_t *p, uint8_t *m, reg r, int8_t b) {
    SET_REX;
    return jit_b(p, m, 4, rex, 0x83, 0xE8 + rid(r), b);
}

jit_stat jit_subrr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x29, modrm(0xC0, d, s));
}

jit_stat jit_xorrr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x31, modrm(0xC0, d, s));
}

jit_stat jit_cmprr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x39, modrm(0xC0, d, s));
}

jit_stat jit_testrr(size_t *p, uint8_t *m, reg d, reg s) {
    SET_REX2;
    return jit_b(p, m, 3, rex, 0x85, modrm(0xC0, d, s));
}

uint8_t jit_jmp_lblb(size_t from, size_t to) {
    if (from > to) return to - from - 2;
    return (uint8_t) from - to;
}

jit_stat jit_jneb(size_t *p, uint8_t *m, uint8_t b) {
    return jit_b(p, m, 2, 0x75, b);
}
