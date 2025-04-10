
#include "x64.h"

#define RIP 5

static bool reg_is_upper(reg r) {
    return (r >= R(8) && r <= R(15)) || (r >= XMM(8) && r <= XMM(15));
}

static uint8_t rid(reg r) {
    return r & 7;
}

// if the source can be memory address regs need to be swapped
static uint8_t modrm(mod m, reg r, reg rm) {
    return m | rid(rm) << 3 | rid(r);
}

static uint8_t sib(mod m, reg r) {
    return modrm(m, r, r);
}

static uint8_t roe(mod m, uint8_t e, reg r) {
    return m | e << 3 | rid(r);
}

static size_t pg_algn(size_t size) {
    size_t mod = size % getpagesize();
    if (mod) size = size - mod + getpagesize();
    return size;
}

uint8_t *x64_mmap(size_t size) {
    return mmap(NULL, pg_algn(size), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

void x64_munmap(size_t size, uint8_t *m) {
    munmap(m, pg_algn(size));
}

x64_stat x64_a(size_t *p, uint8_t *m, uint8_t b) {
    m[(*p)++] = b;
    return X64_STAT(OK);
}

x64_stat x64_b(size_t *p, uint8_t *m, size_t len, ...) {
    va_list args;
    va_start(args, len);
    for (size_t i = 0; i < len; i++) x64_a(p, m, (uint8_t) va_arg(args, int));
    va_end(args);
    return X64_STAT(OK);
}

x64_stat x64_c(size_t *p, uint8_t *m, size_t len, uint8_t b[]) {
    for (size_t i = 0; i < len; i++) x64_a(p, m, b[i]);
    return X64_STAT(OK);
}

x64_stat x64_d(size_t *p, uint8_t *m, un v) {
    memset(m + *p, 0, sizeof(un));
    memcpy(m + *p, &v, sizeof(un));
    *p += sizeof(un);
    return X64_STAT(OK);
}

x64_stat x64_e(size_t *p, uint8_t *m, size_t size, un v) {
    memset(m + *p, 0, size);
    switch (size) {
        case sizeof(uint8_t):
            memcpy(m + *p, &v.u3, size);
            break;
        case sizeof(uint16_t):
            memcpy(m + *p, &v.u4, size);
            break;
        case sizeof(uint32_t):
            memcpy(m + *p, &v.u5, size);
            break;
        case sizeof(uint64_t):
            memcpy(m + *p, &v.u6, size);
            break;
        default:
            return X64_STAT(INV_SIZE);
    }
    *p += size;
    return X64_STAT(OK);
}

x64_stat x64_nop(size_t *p, uint8_t *m) {
    return x64_a(p, m, 0x90);
}

x64_stat x64_ret(size_t *p, uint8_t *m) {
    return x64_a(p, m, 0xC3);
}

x64_stat x64_leave(size_t *p, uint8_t *m) {
    return x64_a(p, m, 0xC9);
}

#define VALID_R(REG) if (REG > R(15)) return X64_STAT(INV_REG);
#define VALID_X(X) if (X < XMM(0)) return X64_STAT(INV_REG);

static uint8_t set_rex(reg r) {
    uint8_t rex = REX(W);
    if (reg_is_upper(r)) rex |= REX(B);
    return rex;
}

static uint8_t set_rex2(reg b, reg r) {
    uint8_t rex = REX(W);
    if (reg_is_upper(b)) rex |= REX(B);
    if (reg_is_upper(r)) rex |= REX(R);
    return rex;
}

// only add rex if REX.R or REX.B is set
static void rex_br(size_t *p, uint8_t *m, reg b, reg r) {
    uint8_t rex = 0;
    if (reg_is_upper(b)) rex |= REX(B);
    if (reg_is_upper(r)) rex |= REX(R);
    if (rex) x64_a(p, m, rex);
}

x64_stat x64_push_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 2, set_rex(r), 0x50 + rid(r));
}

x64_stat x64_pop_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 2, set_rex(r), 0x58 + rid(r));
}

x64_stat x64_call_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xFF, 0xD0 + rid(r));
}

x64_stat x64_call_d(size_t *p, uint8_t *m, uint32_t dw) {
    x64_a(p, m, 0xE8);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_mov_rq(size_t *p, uint8_t *m, reg r, un u) {
    VALID_R(r);
    x64_b(p, m, 2, set_rex(r), 0xB8 + rid(r));
    return x64_d(p, m, u);
}

x64_stat x64_mov_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(11), d, s));
}

x64_stat x64_mov_rmr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(00), d, s));
    if (d == R(SP)) return x64_a(p, m, sib(MOD(00), d));
    return X64_STAT(OK);
}

x64_stat x64_mov_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(d, s), 0x89, modrm(MOD(01), d, s));
    if (d == R(SP)) return x64_a(p, m, sib(MOD(00), d));
    return x64_a(p, m, dsp);
}

x64_stat x64_mov_rrm(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(s, d), 0x8B, modrm(MOD(00), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return X64_STAT(OK);
}

x64_stat x64_mov_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(s, d), 0x8B, modrm(MOD(01), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return x64_a(p, m, dsp);
}

x64_stat x64_movq_rx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_X(s);
    return x64_b(p, m, 5, 0x66, set_rex2(d, s), 0x0F, 0x7E, modrm(MOD(11), d, s));
}

x64_stat x64_movq_rmx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_X(s);
    x64_b(p, m, 5, 0x66, set_rex2(d, s), 0x0F, 0xD6, modrm(MOD(00), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return X64_STAT(OK);
}

x64_stat x64_movq_rmbx(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    VALID_R(d);
    VALID_X(s);
    x64_b(p, m, 5, 0x66, set_rex2(d, s), 0x0F, 0xD6, modrm(MOD(01), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return x64_a(p, m, dsp);
}

x64_stat x64_movq_xr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_R(s);
    return x64_b(p, m, 5, 0x66, set_rex2(s, d), 0x0F, 0x6E, modrm(MOD(11), s, d));
}

x64_stat x64_movq_xrm(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_R(s);
    x64_b(p, m, 5, 0xF3, set_rex2(s, d), 0x0F, 0x7E, modrm(MOD(00), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return X64_STAT(OK);
}

x64_stat x64_movq_xrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    VALID_X(d);
    VALID_R(s);
    x64_b(p, m, 5, 0xF3, set_rex2(s, d), 0x0F, 0x7E, modrm(MOD(01), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return x64_a(p, m, dsp);
}

x64_stat x64_movq_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF3);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x7E, modrm(MOD(11), s, d));
}

x64_stat x64_movq_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0xF3);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x7E, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_movsd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x10, modrm(MOD(11), s, d));
}

x64_stat x64_movsd_xrm(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_R(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    x64_b(p, m, 3, 0x0F, 0x10, modrm(MOD(00), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return X64_STAT(OK);
}

x64_stat x64_movsd_xrmb(size_t *p, uint8_t *m, reg d, reg s, int8_t dsp) {
    VALID_X(d);
    VALID_R(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    x64_b(p, m, 3, 0x0F, 0x10, modrm(MOD(01), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return x64_a(p, m, dsp);
}

x64_stat x64_movsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0xF2);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x10, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_movsd_rmx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, d, s);
    x64_b(p, m, 3, 0x0F, 0x11, modrm(MOD(00), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return X64_STAT(OK);
}

x64_stat x64_movsd_rmbx(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    VALID_R(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, d, s);
    x64_b(p, m, 3, 0x0F, 0x11, modrm(MOD(01), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return x64_a(p, m, dsp);
}

x64_stat x64_lea_rrb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(s, d), 0x8D, modrm(MOD(01), s, d));
    if (s == R(SP)) x64_a(p, m, sib(MOD(00), s));
    return x64_a(p, m, dsp);
}

x64_stat x64_inc_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xFF, roe(MOD(11), 0, r));
}

x64_stat x64_add_rb(size_t *p, uint8_t *m, reg r, int8_t b) {
    VALID_R(r);
    return x64_b(p, m, 4, set_rex(r), 0x83, roe(MOD(11), 0, r), b);
}

x64_stat x64_add_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d)
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x01, modrm(MOD(11), d, s));
}

x64_stat x64_add_rmr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d)
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(d, s), 0x01, modrm(MOD(00), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return X64_STAT(OK);
}

x64_stat x64_add_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) {
    VALID_R(d)
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(d, s), 0x01, modrm(MOD(01), d, s));
    if (d == R(SP)) x64_a(p, m, sib(MOD(00), d));
    return x64_a(p, m, dsp);
}

x64_stat x64_addsd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x58, modrm(MOD(11), s, d));
}

x64_stat x64_addsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0xF2);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x58, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_dec_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xFF, roe(MOD(11), 1, r));
}

x64_stat x64_sub_rb(size_t *p, uint8_t *m, reg r, int8_t b) {
    VALID_R(r);
    return x64_b(p, m, 4, set_rex(r), 0x83, roe(MOD(11), 5, r), b);
}

x64_stat x64_sub_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x29, modrm(MOD(11), d, s));
}

x64_stat x64_subsd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x5C, modrm(MOD(11), s, d));
}

x64_stat x64_subsd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0xF2);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x5C, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_neg_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xF7, roe(MOD(11), 3, r));
}

x64_stat x64_mul_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xF7, roe(MOD(11), 4, r));
}

x64_stat x64_imul_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xF7, roe(MOD(11), 5, r));
}

x64_stat x64_div_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xF7, roe(MOD(11), 6, r));
}

x64_stat x64_idiv_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 3, set_rex(r), 0xF7, roe(MOD(11), 7, r));
}

x64_stat x64_mulsd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x59, modrm(MOD(11), s, d));
}

x64_stat x64_divsd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0xF2);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x5E, modrm(MOD(11), s, d));
}

x64_stat x64_cvtsi2sd_xr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_R(s);
    return x64_b(p, m, 5, 0xF2, set_rex2(s, d), 0x0F, 0x2A, modrm(MOD(11), s, d));
}

x64_stat x64_and_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x21, modrm(MOD(11), d, s));
}

x64_stat x64_xor_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x31, modrm(MOD(11), d, s));
}

x64_stat x64_cmp_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x39, modrm(MOD(11), d, s));
}

x64_stat x64_cmp_rrm(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    x64_b(p, m, 3, set_rex2(s, d), 0x3B, modrm(MOD(00), s, d));
    if (s == R(SP)) return x64_a(p, m, sib(MOD(00), s));
    return X64_STAT(OK);
}

x64_stat x64_cmp_ri(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_R(d);
    x64_b(p, m, 3, set_rex2(RIP, d), 0x3B, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_cmp_rb(size_t *p, uint8_t *m, reg r, uint8_t b) {
    VALID_R(r);
    return x64_b(p, m, 4, set_rex(r), 0x83, roe(MOD(11), 7, r), b);
}

x64_stat x64_cmp_rd(size_t *p, uint8_t *m, reg r, uint32_t d) {
    VALID_R(r);
    x64_b(p, m, 3, set_rex(r), 0x81, roe(MOD(11), 7, r));
    return x64_e(p, m, sizeof(uint32_t), U5(d));
}

x64_stat x64_ucomisd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0x66);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x2E, modrm(MOD(11), s, d));
}

x64_stat x64_ucomisd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0x66);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x2E, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_comisd_xx(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_X(d);
    VALID_X(s);
    x64_a(p, m, 0x66);
    rex_br(p, m, s, d);
    return x64_b(p, m, 3, 0x0F, 0x2F, modrm(MOD(11), s, d));
}

x64_stat x64_comisd_xi(size_t *p, uint8_t *m, reg d, uint32_t dsp) {
    VALID_X(d);
    x64_a(p, m, 0x66);
    rex_br(p, m, RIP, d);
    x64_b(p, m, 3, 0x0F, 0x2F, modrm(MOD(00), RIP, d));
    return x64_e(p, m, sizeof(uint32_t), U5(dsp));
}

x64_stat x64_test_rr(size_t *p, uint8_t *m, reg d, reg s) {
    VALID_R(d);
    VALID_R(s);
    return x64_b(p, m, 3, set_rex2(d, s), 0x85, modrm(MOD(11), d, s));
}

uint8_t x64_jmpu_lblb(size_t from, size_t to) {
    return to - from - sizeof(uint8_t);
}

uint32_t x64_jmpu_lbldw(size_t from, size_t to) {
    return to - from - sizeof(uint32_t);
}

uint8_t *x64_lb(size_t p, uint8_t *m) {
    return &m[p - 1];
}

void x64_jmpd_lblb(uint8_t *byte, size_t from, size_t to) {
    *byte = (uint8_t) to - from;
}

void x64_jmpd_lbldw(uint8_t *m, size_t from, size_t to) {
    size_t r = to - from;
    memcpy(m + from - sizeof(uint32_t), &r, sizeof(uint32_t));
}

x64_stat x64_jmp_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0xEB, b);
}

x64_stat x64_jmp_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_a(p, m, 0xE9);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jbjnaejc_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x72, b);
}

x64_stat x64_jbjnaejc_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x82);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jnbjaejnc_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x73, b);
}

x64_stat x64_jnbjaejnc_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x83);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jzje_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x74, b);
}

x64_stat x64_jzje_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x84);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jnzjne_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x75, b);
}

x64_stat x64_jnzjne_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x85);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jbejna_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x76, b);
}

x64_stat x64_jbejna_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x86);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jnbeja_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x77, b);
}

x64_stat x64_jnbeja_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x87);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jljnge_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x7C, b);
}

x64_stat x64_jljnge_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x8C);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jnljge_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x7D, b);
}

x64_stat x64_jnljge_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x8D);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jlejng_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x7E, b);
}

x64_stat x64_jlejng_dw(size_t *p, uint8_t *m, uint8_t dw) {
    x64_b(p, m, 2, 0x0F, 0x8E);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_jnlejg_b(size_t *p, uint8_t *m, uint8_t b) {
    return x64_b(p, m, 2, 0x7F, b);
}

x64_stat x64_jnlejg_dw(size_t *p, uint8_t *m, uint32_t dw) {
    x64_b(p, m, 2, 0x0F, 0x8F);
    return x64_e(p, m, sizeof(uint32_t), U5(dw));
}

x64_stat x64_setlsetnge_r(size_t *p, uint8_t *m, reg r) {
    VALID_R(r);
    return x64_b(p, m, 4, set_rex(r), 0x0F, 0x9C, roe(MOD(11), 0, r));
}
