
#include "jit.h"

static bool reg_is_upper(reg r) {
    return (r >= R(8) && r <= R(15)) || (r >= XMM(8) && r <= XMM(15));
}

static uint8_t rid(reg r) {
    return r % 8;
}

// if the source can be memory address they need to be swapped
static uint8_t modrm(mod m, reg d, reg s) {
    return m + 8 * rid(s) + rid(d);
}

static size_t pg_algn(size_t size) {
    size_t mod = size % getpagesize();
    if (mod) size = size - mod + getpagesize();
    return size;
}

uint8_t *jit_mmap(size_t size) {
    return mmap(NULL, pg_algn(size), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

void jit_munmap(size_t size, uint8_t *m) {
    munmap(m, pg_algn(size));
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

jit_stat jit_d(size_t *p, uint8_t *m, un v) {
    memset(m + *p, 0, sizeof(un));
    memcpy(m + *p, &v, sizeof(un));
    *p += sizeof(un);
    return JIT_STAT(OK);
}

jit_stat jit_nop(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0x90);
}

jit_stat jit_ret(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC3);
}

jit_stat jit_leave(size_t *p, uint8_t *m) {
    return jit_a(p, m, 0xC9);
}

#define VALID_R(REG) if (REG > R(15)) return JIT_STAT(INV_REG);
#define VALID_X(X) if (X < XMM(0)) return JIT_STAT(INV_REG);

static uint8_t set_rex(reg r) {
    uint8_t rex = REX(W);
    if (reg_is_upper(r)) rex |= REX(B);
    return rex;
}

static uint8_t set_rex2(reg s, reg d) {
    uint8_t rex = REX(W);
    if (reg_is_upper(s)) rex |= REX(B);
    if (reg_is_upper(d)) rex |= REX(R);
    return rex;
}

jit_stat jit_push(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 2, set_rex(r), 0x50 + rid(r));
}

jit_stat jit_pop(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 2, set_rex(r), 0x58 + rid(r));
}

jit_stat jit_call_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 3, set_rex(r), 0xFF, 0xD0 + rid(r));
}

jit_stat jit_mov_rq(size_t *p, uint8_t *m, reg r, un u) {
    VALID_R(r);
    jit_b(p, m, 2, set_rex(r), 0xB8 + rid(r));
    return jit_d(p, m, u);
}

jit_stat jit_mov_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(11), d, s));
}

jit_stat jit_mov_rar(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    jit_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(00), d, s));
    if (d == R(SP)) return jit_a(p, m, 0x24);
    return JIT_STAT(OK);
}

jit_stat jit_mov_rabr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    VALID_R(d);
    VALID_R(s);
    jit_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(01), d, s));
    if (d == R(SP)) return jit_a(p, m, 0x24);
    return jit_a(p, m, dsp);
}

jit_stat jit_mov_rra(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    jit_b(p, m, 3, set_rex2(s, d), 0x8B, modrm(MOD(00), s, d));
    if (s == R(SP)) jit_a(p, m, 0x24);
    return JIT_STAT(OK);
}

jit_stat jit_mov_rrab(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    VALID_R(d);
    VALID_R(s);
    jit_b(p, m, 3, set_rex2(s, d), 0x8B, modrm(MOD(01), s, d));
    if (s == R(SP)) jit_a(p, m, 0x24);
    return jit_a(p, m, dsp);
}

jit_stat jit_lea_rrb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    VALID_R(d);
    VALID_R(s);
    jit_b(p, m, 3, set_rex2(s, d), 0x8D, modrm(MOD(01), s, d));
    if (s == R(SP)) jit_a(p, m, 0x24);
    return jit_a(p, m, dsp);
}

jit_stat jit_inc_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 3, set_rex(r), 0xFF, MOD(11) + rid(r));
}

jit_stat jit_add_rb(size_t *p, uint8_t *m, reg r, int8_t b) {
    VALID_R(r);
    return jit_b(p, m, 4, set_rex(r), 0x83, MOD(11) + rid(r), b);
}

jit_stat jit_add_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d)
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x01, modrm(MOD(11), d, s));
}

jit_stat jit_addsd_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    return jit_b(p, m, 5, set_rex2(s, d), 0xF2, 0xF, 0x58, modrm(MOD(11), s, d));
}

jit_stat jit_dec_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 3, set_rex(r), 0xFF, 0xC8 + rid(r));
}

jit_stat jit_sub_rb(size_t *p, uint8_t *m, reg r, int8_t b) {
    VALID_R(r);
    return jit_b(p, m, 4, set_rex(r), 0x83, 0xE8 + rid(r), b);
}

jit_stat jit_sub_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x29, modrm(MOD(11), d, s));
}

jit_stat jit_subsd_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    return jit_b(p, m, 5, set_rex2(s, d), 0xF2, 0xF, 0x5C, modrm(MOD(11), s, d));
}

jit_stat jit_xor_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x31, modrm(MOD(11), d, s));
}

jit_stat jit_cmp_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x39, modrm(MOD(11), d, s));
}

jit_stat jit_test_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return jit_b(p, m, 3, set_rex2(d, s), 0x85, modrm(MOD(11), d, s));
}

uint8_t jit_jmpu_lblb(size_t from, size_t to) {
    return to - from - 2;
}

uint8_t *jit_lb(size_t p, uint8_t *m) {
    return &m[p - 1];
}

void jit_jmpd_lblb(uint8_t *byte, size_t from, size_t to) {
    *byte = (uint8_t) to - from;
}

jit_stat jit_jmp_b(size_t *p, uint8_t *m, uint8_t b) {
    return jit_b(p, m, 2, 0xEB, b);
}

jit_stat jit_jc_b(size_t *p, uint8_t *m, uint8_t b) {
    return jit_b(p, m, 2, 0x72, b);
}

jit_stat jit_jne_b(size_t *p, uint8_t *m, uint8_t b) {
    return jit_b(p, m, 2, 0x75, b);
}

jit_stat jit_jna_b(size_t *p, uint8_t *m, uint8_t b) {
    return jit_b(p, m, 2, 0x76, b);
}

jit_stat jit_setl_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return jit_b(p, m, 4, set_rex(r), 0x0F, 0x9C, MOD(11) + rid(r));
}
