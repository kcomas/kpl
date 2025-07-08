
#include "x64.h"

static size_t pg_algn(size_t pages) {
    return pages * getpagesize();
}

uint8_t *x64_mmap(size_t pages) {
    return mmap(NULL, pg_algn(pages), PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
}

void x64_mp_w(size_t pages, size_t p, uint8_t *m) {
    if (p % getpagesize()) STOP("jit mem w unaligned");
    mprotect(m + p, pg_algn(pages) - p, PROT_WRITE);
}

static size_t mp_align(size_t p) {
    size_t mod = p % getpagesize();
    if (mod) p = p - mod + getpagesize();
    return p;
}

void x64_mp_rx(size_t pages, size_t *p, uint8_t *m) {
    *p = mp_align(*p);
    mprotect(m, *p, PROT_READ | PROT_EXEC);
    mprotect(m + *p, pg_algn(pages) - *p, PROT_NONE);
}

void x64_munmap(size_t pages, uint8_t *m) {
    munmap(m, pg_algn(pages));
}

size_t p = 0;

uint8_t *m = NULL;

static __attribute__((constructor)) void x64_con(void) {
    m = x64_mmap(getpagesize() * JIT_P);
}

static __attribute__((destructor)) void x64_des(void) {
    x64_munmap(getpagesize() * JIT_P, m);
}

#define RIP 5

#define SIB 4

static uint8_t rid(reg r) {
    return r & 7;
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

x64_stat x64_c(size_t *p, uint8_t *m, size_t len, uint8_t *b) {
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

#define Z(N, C) x64_stat x64_##N(size_t *p, uint8_t *m) { \
    return x64_a(p, m, C); \
}

#define ZB(N, C) x64_stat x64_##N##_b(size_t *p, uint8_t *m, uint8_t b) { \
    return x64_b(p, m, 2, C, b); \
}

#define ZD(N, C) x64_stat x64_##N##_d(size_t *p, uint8_t *m, uint32_t dw) { \
    x64_a(p, m, C); \
    return x64_e(p, m, sizeof(uint32_t), U5(dw)); \
}

#define ZZD(N, C1, C2) x64_stat x64_##N##_d(size_t *p, uint8_t *m, uint32_t dw) { \
    x64_b(p, m, 2, C1, C2); \
    return x64_e(p, m, sizeof(uint32_t), U5(dw)); \
}

#define ZR(N, C) x64_stat x64_##N##_r(size_t *p, uint8_t *m, reg r) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 2, rex, MOD(00) | C | rid(r)); \
}

#define ZRE(N, C, E) x64_stat x64_##N##_r(size_t *p, uint8_t *m, reg r) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 3, rex, C, MOD(11) | E << 3 | rid(r)); \
}

#define ZREB(N, C, E) x64_stat x64_##N##_rb(size_t *p, uint8_t *m, reg r, uint8_t b) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 4, rex, C, MOD(11) | E << 3 | rid(r), b); \
}

#define ZRED(N, C, E) x64_stat x64_##N##_rd(size_t *p, uint8_t *m, reg r, uint32_t d) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(11) | E << 3 | rid(r)); \
    return x64_e(p, m, sizeof(uint32_t), U5(d)); \
}

#define ZRME(N, C, E) x64_stat x64_##N##_rm(size_t *p, uint8_t *m, reg r) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(00) | E << 3 | rid(r)); \
    if (r == R(SP) || r == R(12)) x64_a(p, m, S1 | rid(r) << 3 | rid(r)); \
    return X64_STAT(OK); \
}

#define ZRMEB(N, C, E) x64_stat x64_##N##_rmb(size_t *p, uint8_t *m, reg r, uint8_t b) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(00) | E << 3 | rid(r)); \
    if (r == R(SP) || r == R(12)) x64_a(p, m, S1 | rid(r) << 3 | rid(r)); \
    return x64_a(p, m, b); \
}

#define ZRMBE(N, C, E) x64_stat x64_##N##_rmb(size_t *p, uint8_t *m, reg r, uint8_t dsp) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(01) | E << 3 | rid(r)); \
    if (r == R(SP) || r == R(12)) x64_a(p, m, S1 | rid(r) << 3 | rid(r)); \
    return x64_a(p, m, dsp); \
}

#define ZRMBEB(N, C, E) x64_stat x64_##N##_rmbb(size_t *p, uint8_t *m, reg r, uint8_t dsp, uint8_t b) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(01) | E << 3 | rid(r)); \
    if (r == R(SP) || r == R(12)) x64_a(p, m, S1 | rid(r) << 3 | rid(r)); \
    return x64_b(p, m, 2, dsp, b); \
}

#define ZRMBED(N, C, E) x64_stat x64_##N##_rmbd(size_t *p, uint8_t *m, reg r, uint8_t dsp, uint32_t d) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(01) | E << 3 | rid(r)); \
    if (r == R(SP) || r == R(12)) x64_a(p, m, S1 | rid(r) << 3 | rid(r)); \
    x64_b(p, m, 1, dsp); \
    return x64_e(p, m, sizeof(uint32_t), U5(d)); \
}

#define ZZRE(N, C1, C2, E) x64_stat x64_##N##_r(size_t *p, uint8_t *m, reg r) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 4, rex, C1, C2, MOD(11) | E << 3 | rid(r)); \
}

#define ZZR(N, C1, C2) x64_stat x64_##N##_r(size_t *p, uint8_t *m, reg r) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (r >= R(8)) rex |= REX(B); \
    if (rex) x64_a(p, m, rex); \
    return x64_b(p, m, 2, C1, MOD(00) | C2 | rid(r)); \
}

#define ZRQ(N, C) x64_stat x64_##N##_rq(size_t *p, uint8_t *m, reg r, un u) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(B); \
    x64_b(p, m, 2, rex, MOD(00) | C | rid(r)); \
    return x64_d(p, m, u); \
}

#define ZRI(N, C) x64_stat x64_##N##_ri(size_t *p, uint8_t *m, reg r, uint32_t dsp) { \
    if (r > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (r >= R(8)) rex |= REX(R); \
    x64_b(p, m, 3, rex, C, MOD(00) | rid(r) << 3 | RIP); \
    return x64_e(p, m, sizeof(uint32_t), U5(dsp)); \
}

#define ZRR(N, C, RR, RM) x64_stat x64_##N##_rr(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 3, rex, C, MOD(11) | rid(RR) << 3 | rid(RM)); \
}

#define ZRMR(N, C, RR, RM) x64_stat x64_##N##_rmr(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(00) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return X64_STAT(OK); \
}

#define ZRMBR(N, C, RR, RM) x64_stat x64_##N##_rmbr(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(01) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | RM << 3 | RM); \
    return x64_a(p, m, dsp); \
}

#define ZRMOR(N, C, RR, RS, RM) x64_stat x64_##N##_rmor(size_t *p, uint8_t *m, reg d, reg o, scale x, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 4, rex, C, MOD(00) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
}

#define ZRMOBR(N, C, RR, RS, RM) x64_stat x64_##N##_rmobr(size_t *p, uint8_t *m, reg d, reg o, scale x, uint8_t dsp, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 4, rex, C, MOD(01) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

#define ZRRM(N, C, RR, RM) x64_stat x64_##N##_rrm(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(00) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return X64_STAT(OK); \
}

#define ZRRMB(N, C, RR, RM) x64_stat x64_##N##_rrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(01) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

#define ZRRMD(N, C, RR, RM) x64_stat x64_##N##_rrmd(size_t *p, uint8_t *m, reg d, reg s, uint32_t dsp) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 3, rex, C, MOD(10) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return x64_e(p, m, sizeof(uint32_t), U5(dsp)); \
}

#define ZRRMO(N, C, RR, RS, RM) x64_stat x64_##N##_rrmo(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 4, rex, C, MOD(00) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
}

#define ZRRMOB(N, C, RR, RS, RM) x64_stat x64_##N##_rrmob(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x, uint8_t dsp) { \
    if (RR > R(15)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= R(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_b(p, m, 4, rex, C, MOD(01) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

#define ZZZXI(N, C1, C2, C3) x64_stat x64_##N##_xi(size_t *p, uint8_t *m, reg r, uint32_t dsp) { \
    if (r < XMM(0)) return X64_STAT(INV_REG); \
    x64_a(p, m, C1); \
    uint8_t rex = 0; \
    if (r >= XMM(8)) rex |= REX(R); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 3, C2, C3, MOD(00) | rid(r) << 3 | RIP); \
    return x64_e(p, m, sizeof(uint32_t), U5(dsp)); \
}

#define ZZZXX(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_xx(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM < XMM(0)) return X64_STAT(INV_REG); \
    x64_a(p, m, C1); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= XMM(8)) rex |= REX(B); \
    if (rex) x64_a(p, m, rex); \
    return x64_b(p, m, 3, C2, C3, MOD(11) | rid(RR) << 3 | rid(RM)); \
}

#define ZZZXR(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_xr(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 5, C1, rex, C2, C3, MOD(11) | rid(RR) << 3 | rid(RM)); \
}

#define ZZZRX(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_rx(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = REX(W); \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    return x64_b(p, m, 5, C1, rex, C2, C3, MOD(11) | rid(RR) << 3 | rid(RM)); \
}

#define ZZZRMX(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_rmx(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 3, C2, C3, MOD(00) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return X64_STAT(OK); \
}

#define ZZZRMBX(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_rmbx(size_t *p, uint8_t *m, reg d, uint8_t dsp, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 3, C2, C3, MOD(01) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

#define ZZZXRM(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_xrm(size_t *p, uint8_t *m, reg d, reg s) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 3, C2, C3, MOD(00) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return X64_STAT(OK); \
}

#define ZZZXRMB(N, C1, C2, C3, RR, RM) x64_stat x64_##N##_xrmb(size_t *p, uint8_t *m, reg d, reg s, uint8_t dsp) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 3, C2, C3, MOD(01) | rid(RR) << 3 | rid(RM)); \
    if (RM == R(SP) || RM == R(12)) x64_a(p, m, S1 | rid(RM) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

#define ZZZXRMO(N, C1, C2, C3, RR, RS, RM) x64_stat x64_##N##_xrmo(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    return x64_b(p, m, 4, C2, C3, MOD(00) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
}

#define ZZZXRMOB(N, C1, C2, C3, RR, RS, RM) x64_stat x64_##N##_xrmob(size_t *p, uint8_t *m, reg d, reg s, reg o, scale x, uint8_t dsp) { \
    if (RR < XMM(0)) return X64_STAT(INV_REG); \
    if (RS > R(15)) return X64_STAT(INV_REG); \
    if (RM > R(15)) return X64_STAT(INV_REG); \
    uint8_t rex = 0; \
    if (RR >= XMM(8)) rex |= REX(R); \
    if (RS >= R(8)) rex |= REX(X); \
    if (RM >= R(8)) rex |= REX(B); \
    x64_a(p, m, C1); \
    if (rex) x64_a(p, m, rex); \
    x64_b(p, m, 4, C2, C3, MOD(01) | rid(RR) << 3 | SIB, x | rid(RS) << 3 | rid(RM)); \
    return x64_a(p, m, dsp); \
}

Z(nop, 0x90);

Z(ret, 0xC3);

Z(leave, 0xC9);

ZR(push, 0x50);

ZR(pop, 0x58);

ZZR(call, 0xFF, 0xD0);

ZD(call, 0xE8);

ZRQ(mov, 0xB8);

ZRR(mov, 0x89, s, d);

ZRMR(mov, 0x89, s, d);

ZRMBR(mov, 0x89, s, d);

ZRMOR(mov, 0x89, s, o, d);

ZRMOBR(mov, 0x89, s, o, d);

ZRRM(mov, 0x8B, d, s);

ZRRMB(mov, 0x8B, d, s);

ZRRMD(mov, 0x8B, d, s);

ZRRMO(mov, 0x8B, d, o, s);

ZRRMOB(mov, 0x8B, d, o, s);

ZRI(mov, 0x8B);

ZRMBED(mov, 0xC7, 0);

ZZZRX(movq, 0x66, 0x0F, 0x7E, s, d);

ZZZRMX(movq, 0x66, 0x0F, 0xD6, s, d);

ZZZRMBX(movq, 0x66, 0x0F, 0xD6, s, d);

ZZZXR(movq, 0x66, 0x0F, 0x6E, d, s);

ZZZXRM(movq, 0xF3, 0x0F, 0x7E, d, s);

ZZZXRMB(movq, 0xF3, 0x0F, 0x7E, d, s);

ZZZXX(movq, 0xF3, 0x0F, 0x7E, d, s);

ZZZXI(movq, 0xF3, 0x0F, 0x7E);

ZZZXX(movsd, 0xF2, 0x0F, 0x10, d, s);

ZZZXRM(movsd, 0xF2, 0x0F, 0x10, d, s);

ZZZXRMB(movsd, 0xF2, 0x0F, 0x10, d, s);

ZZZXI(movsd, 0xF2, 0x0F, 0x10);

ZZZRMX(movsd, 0xF2, 0x0F, 0x11, s, d);

ZZZRMBX(movsd, 0xF2, 0x0F, 0x11, s, d);

ZZZXRMO(movsd, 0xF2, 0x0F, 0x10, d, o, s);

ZZZXRMOB(movsd, 0xF2, 0x0F, 0x10, d, o, s);

ZRRMB(lea, 0x8D, d, s);

ZRI(lea, 0x8D);

ZRE(inc, 0xFF, 0);

ZRME(inc, 0xFF, 0);

ZRMBE(inc, 0xFF, 0);

ZREB(add, 0x83, 0);

ZRED(add, 0x81, 0);

ZRMEB(add, 0x83, 0);

ZRMBEB(add, 0x83, 0);

ZRR(add, 0x01, s, d);

ZRMR(add, 0x01, s, d);

ZRMBR(add, 0x01, s, d);

ZRRMB(add, 0x03, d, s);

ZZZXX(addsd, 0xF2, 0x0F, 0x58, d, s);

ZZZXRM(addsd, 0xF2, 0x0F, 0x58, d, s);

ZZZXRMB(addsd, 0xF2, 0x0F, 0x58, d, s);

ZZZXI(addsd, 0xF2, 0x0F, 0x58);

ZRE(dec, 0xFF, 1);

ZRME(dec, 0xFF, 1);

ZRMBE(dec, 0xFF, 1);

ZREB(sub, 0x83, 5);

ZRR(sub, 0x29, s, d);

ZZZXX(subsd, 0xF2, 0x0F, 0x5C, d, s);

ZZZXI(subsd, 0xF2, 0x0F, 0x5C);

ZRE(neg, 0xF7, 3);

ZRE(mul, 0xF7, 4);

ZRE(imul, 0xF7, 5);

ZRME(imul, 0xF7, 5);

ZRMBE(imul, 0xF7, 5);

ZRE(div, 0xF7, 6);

ZRE(idiv, 0xF7, 7);

ZZZXX(mulsd, 0xF2, 0x0F, 0x59, d, s);

ZZZXI(mulsd, 0xF2, 0x0F, 0x59);

ZZZXX(divsd, 0xF2, 0x0F, 0x5E, d, s);

ZZZXI(divsd, 0xF2, 0x0F, 0x5E);

ZZZXR(cvtsi2sd, 0xF2, 0x0F, 0x2A, d, s);

ZRR(and, 0x21, s, d);

ZREB(and, 0x83, 4);

ZRR(xor, 0x31, s, d);

ZZZXX(pxor, 0x66, 0x0F, 0xEF, d, s);

ZRR(cmp, 0x39, s, d);

ZRRM(cmp, 0x3B, d, s);

ZRRMB(cmp, 0x3B, d, s);

ZRRMD(cmp, 0x3B, d, s);

ZRI(cmp, 0x3B);

ZREB(cmp, 0x83, 7);

ZRMEB(cmp, 0x83, 7);

ZRMBEB(cmp, 0x83, 7);

ZRED(cmp, 0x81, 7);

ZRMBED(cmp, 0x81, 7);

ZZZXX(ucomisd, 0x66, 0x0F, 0x2E, d, s);

ZZZXI(ucomisd, 0x66, 0x0F, 0x2E);

ZZZXX(comisd, 0x66, 0x0F, 0x2F, d, s);

ZZZXI(comisd, 0x66, 0x0F, 0x2F);

ZRR(test, 0x85, s, d);

uint8_t x64_jmpu_lblb(size_t from, size_t to) {
    return to - from - sizeof(uint8_t);
}

uint32_t x64_jmpu_lbld(size_t from, size_t to) {
    return to - from - sizeof(uint32_t);
}

uint8_t *x64_lb(size_t p, uint8_t *m) {
    return &m[p - 1];
}

void x64_jmpd_lblb(uint8_t *byte, size_t from, size_t to) {
    *byte = (uint8_t) to - from;
}

void x64_jmpd_lbld(uint8_t *m, size_t from, size_t to) {
    size_t r = to - from;
    memcpy(m + from - sizeof(uint32_t), &r, sizeof(uint32_t));
}

ZB(jmp, 0xEB);

ZD(jmp, 0xE9);

ZB(jbjnaejc, 0x72);

ZZD(jbjnaejc, 0x0F, 0x82);

ZB(jnbjaejnc, 0x73);

ZZD(jnbjaejnc, 0x0F, 0x83);

ZB(jzje, 0x74);

ZZD(jzje, 0x0F, 0x84);

ZB(jnzjne, 0x75);

ZZD(jnzjne, 0x0F, 0x85);

ZB(jbejna, 0x76);

ZZD(jbejna, 0x0F, 0x86);

ZB(jnbeja, 0x77);

ZZD(jnbeja, 0x0F, 0x87);

ZB(jljnge, 0x7C);

ZZD(jljnge, 0x0F, 0x8C);

ZB(jnljge, 0x7D);

ZZD(jnljge, 0x0F, 0x8D);

ZB(jlejng, 0x7E);

ZZD(jlejng, 0x0F, 0x8E);

ZB(jnlejg, 0x7F);

ZZD(jnlejg, 0x0F, 0x8F);

ZB(loop, 0xE2);

ZZRE(setlsetnge, 0x0F, 0x9C, 0);
