
#include "../src/lib/x64.h"
#include "t.h"

static uint8_t *m = NULL;

__attribute__((constructor)) void x64_con(void) {
    m = x64_mmap(1);
}

__attribute__((destructor)) void x64_des(void) {
    x64_munmap(1, m);
}

static void printj(size_t len, uint8_t *m) {
    printf("- %lu - ", len);
    for (size_t i = 0; i < len; i++) {
        if (i % 30 == 0 || m[i] == 0x90) putchar('\n');
        printf("0x%X ", m[i]);
    }
    putchar('\n');
}

typedef int64_t add3(int64_t a);

T(radd3) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(9), R(DI));
    x64_push_r(&p, m, R(9));
    x64_pop_r(&p, m, R(AX));
    x64_add_rb(&p, m, R(AX), 3);
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    uint64_t a = 3;
    uint64_t r = ((add3*) m)(a);
    printf("add3: %ld, %ld\n", a, r);
    A(r == a + 3, "add3");
}

typedef int64_t add(int64_t a, int64_t b);

T(radd) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(AX), R(DI));
    x64_add_rr(&p, m, R(AX), R(SI));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    uint64_t a = 1, b = 3;
    uint64_t r = ((add*) m)(a, b);
    printf("add: %ld + %ld = %ld\n", a, b, r);
    A(r == a + b, "add");
}
typedef int64_t sub(int64_t a, int64_t b);

T(rsub) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(12), R(DI));
    x64_sub_rr(&p, m, R(12), R(SI));
    x64_mov_rr(&p, m, R(9), R(12));
    x64_mov_rr(&p, m, R(AX), R(9));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    uint64_t a = 20, b = 9;
    uint64_t r = ((sub*) m)(a, b);
    printf("sub: %ld - %ld = %ld\n", a, b, r);
    A(r = a - b, "sub");
}

typedef int64_t loop(int64_t a);

static void x64_printf(size_t *p, uint8_t *m, const char *fmt) {
    x64_xor_rr(p, m, R(AX), R(AX));
    x64_mov_rq(p, m, R(15), P(&printf));
    x64_mov_rq(p, m, R(DI), P(fmt));
    x64_mov_rrm(p, m, R(SI), R(SP));
    x64_call_r(p, m, R(15));
}

T(rloop) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(8), R(DI));
    x64_push_r(&p, m, R(8));
    x64_xor_rr(&p, m, R(10), R(10));
    x64_push_r(&p, m, R(10));
    size_t lbl = p;
    x64_printf(&p, m, "V: %ld\n");
    x64_mov_rrm(&p, m, R(10), R(SP));
    x64_inc_r(&p, m, R(10));
    x64_mov_rmr(&p, m, R(SP), R(10));
    x64_mov_rrmb(&p, m, R(8), R(SP), 8);
    x64_cmp_rr(&p, m, R(10), R(8));
    x64_jnzjne_b(&p, m, x64_jmpu_lblb(p, lbl));
    x64_mov_rr(&p, m, R(AX), R(10));
    x64_leave(&p, m);
    x64_ret(&p, m);
    printj(p, m);
    uint64_t a = 12;
    uint64_t b = ((loop*) m)(a);
    printf("%ld\n", b);
    A(b = a + 1, "loop");
}

typedef uint64_t fib(uint64_t n);

static void bfib(size_t *p, uint8_t *m) {
    x64_push_r(p, m, R(BP));
    x64_mov_rr(p, m, R(BP), R(SP));
    x64_push_r(p, m, R(DI)); // push n
    // if n == 0
    x64_mov_rq(p, m, R(9), U6(0));
    x64_mov_rrm(p, m, R(DI), R(SP));
    x64_cmp_rr(p, m, R(9), R(DI));
    x64_jnzjne_b(p, m, 0);
    uint8_t *byte = x64_lb(*p, m);
    size_t lbl = *p;
    x64_mov_rq(p, m, R(AX), U6(0));
    x64_leave(p, m);
    x64_ret(p, m);
    x64_jmpd_lblb(byte, lbl, *p);
    // if n <= 2
    x64_mov_rq(p, m, R(9), U6(2));
    x64_mov_rrm(p, m, R(DI), R(SP));
    x64_cmp_rr(p, m, R(9), R(DI));
    x64_jbjnaejc_b(p, m, 0);
    byte = x64_lb(*p, m);
    lbl = *p;
    x64_mov_rq(p, m, R(AX), U6(1));
    x64_leave(p, m);
    x64_ret(p, m);
    x64_jmpd_lblb(byte, lbl, *p);
    // fib(n - 1)
    x64_mov_rrm(p, m, R(DI), R(SP));
    x64_sub_rb(p, m, R(DI), 1);
    x64_mov_rq(p, m, R(AX), P(m));
    x64_call_r(p, m, R(AX));
    x64_push_r(p, m, R(AX));
    // fib(n - 2)
    x64_mov_rrmb(p, m, R(DI), R(BP), -8);
    x64_sub_rb(p, m, R(DI), 2);
    x64_mov_rq(p, m, R(AX), P(m));
    x64_call_r(p, m, R(AX));
    x64_pop_r(p, m, R(DI));
    x64_add_rr(p, m, R(AX), R(DI));
    x64_leave(p, m);
    x64_ret(p, m);
}

T(rfib) {
    size_t p = 0;
    bfib(&p, m);
    uint64_t n = 10;
    printj(p, m);
    uint64_t r = ((fib*) m)(n);
    printf("fib(%lu): %lu\n", n, r);
    A(r == 55, "fib");
}

T(daddsub) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_addsd_xx(&p, m, XMM(0), XMM(1));
    x64_subsd_xx(&p, m, XMM(0), XMM(2));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    double a = 1.2, b = 3.4, c = 0.15;
    double d = ((double(*)(double, double, double)) m)(a, b, c);
    printf("dasf(%f + %f - %f): %f\n", a, b, c, d);
    A(d = a + b - c, "double");
}

T(cmp) {
    size_t p = 0;
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_cmp_rr(&p, m, R(DI), R(SI));
    x64_setlsetnge_r(&p, m, R(AX)); // only effect lower bits
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    int64_t a = 1, b = 2;
    uint8_t r = ((uint8_t(*)(int64_t, int64_t)) m)(a, b);
    printf("cmp(%ld < %ld): %d\n", a, b, r);
    A(r == 1, "less");
    r = ((uint8_t(*)(int64_t, int64_t)) m)(b, a);
    printf("cmp(%ld < %ld): %d\n", b, a, r);
    A(r == 0, "greater");
}

static void printp(int64_t **a) {
    printf("%p, %ld\n", a, **a);
}

T(p2p) {
    size_t p = 0;
    int64_t *a = malloc(sizeof(int64_t));
    *a = 1;
    printp(&a);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(DX), R(DI));
    x64_mov_rq(&p, m, R(DI), I6(0));
    x64_mov_rrm(&p, m, R(CX), R(DX));
    x64_mov_rq(&p, m, R(SI), I6(23));
    x64_mov_rmr(&p, m, R(CX), R(SI));
    x64_push_r(&p, m, R(CX));
    x64_lea_rrb(&p, m, R(DI), R(BP), -8);
    x64_mov_rq(&p, m, R(AX), P(&printp));
    x64_call_r(&p, m, R(AX));
    x64_pop_r(&p, m, R(CX));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    printj(p, m);
    ((void(*)(int64_t**)) m)(&a);
    A(p == 58, "bytes");
    printp(&a);
    free(a);
}

T(rskiploop) {
    size_t p = 0;
    x64_mov_rq(&p, m, R(AX), U6(1));
    x64_push_r(&p, m, R(AX));
    x64_mov_rr(&p, m, R(SI), R(DI));
    uint32_t loop = p;
    x64_mov_rrm(&p, m, R(AX), R(SP));
    x64_cmp_rr(&p, m, R(SI), R(AX));
    x64_jbjnaejc_dw(&p, m, 0);
    uint32_t test = p;
    x64_push_r(&p, m, R(SI));
    x64_printf(&p, m, "Dec: %d\n");
    x64_pop_r(&p, m, R(SI));
    x64_dec_r(&p, m, R(SI));
    x64_jmp_dw(&p, m, x64_jmpu_lbldw(p, loop));
    uint32_t end = p;
    x64_jmpd_lbldw(m, test, end);
    x64_pop_r(&p, m, R(AX));
    x64_ret(&p, m);
    printj(p, m);
    A(((int32_t(*)(int32_t)) m)(5) == 1, "loop");
}

T(cvtsi2sd) {
    size_t p = 0;
    x64_movq_xr(&p, m, XMM(2), R(DI));
    x64_movq_rx(&p, m, R(8), XMM(2));
    x64_cvtsi2sd_xr(&p, m, XMM(11), R(8));
    x64_movq_xx(&p, m, XMM(0), XMM(11));
    x64_ret(&p, m);
    printj(p, m);
    int64_t r = 123; double x = 123.0;
    A(((double(*)(int64_t)) m)(r) == x, "cvtsi2sd");
}

static const char *comisdg = "Greater Equal";

static const char *comisdl = "Less";

T(comisd) {
    size_t p = 0;
    x64_movq_xx(&p, m, XMM(15), XMM(0));
    x64_comisd_xx(&p, m, XMM(15), XMM(1));
    x64_jbjnaejc_b(&p, m, 0);
    uint8_t *byte = x64_lb(p, m);
    size_t lbl = p;
    x64_mov_rq(&p, m, R(AX), P(comisdg));
    x64_ret(&p, m);
    x64_jmpd_lblb(byte, lbl, p);
    x64_mov_rq(&p, m, R(AX), P(comisdl));
    x64_ret(&p, m);
    printj(p, m);
    double x = 1.2, y = 3.4;
    const char *r = ((char*(*)(double, double)) m)(x, y);
    printf("comisd(%lf, %lf): %s\n", x, y, r);
    A(r == comisdl, "Less");
}

T(cmprip) {
    int64_t c = INT64_MAX;
    size_t p = 0;
    x64_cmp_ri(&p, m, R(DI), 0); // ripe - rips
    uint32_t rips = p;
    x64_jzje_b(&p, m, 0);
    uint8_t *byte = x64_lb(p, m);
    size_t lbl = p;
    x64_mov_rq(&p, m, R(AX), P(0));
    x64_ret(&p, m);
    x64_jmpd_lblb(byte, lbl, p);
    x64_mov_rr(&p, m, R(AX), R(DI));
    x64_ret(&p, m);
    uint32_t ripe = p;
    x64_jmpd_lbldw(m, rips, ripe);
    x64_e(&p, m, sizeof(int64_t), I6(c));
    printj(p, m);
    int64_t r = ((int64_t(*)(int64_t)) m)(INT64_MAX);
    printf("rip disp32: %d, %ld == %ld\n", ripe - rips, c, r);
    A(c == r, "ne");
}
