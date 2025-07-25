
#include "../src/lib/x64.h"
#include "t.h"

/*
static void printj(size_t len, uint8_t *m) {
    printf("- %lu - ", len);
    for (size_t i = 0; i < len; i++) {
        if (i % 30 == 0 || m[i] == 0x90) putchar('\n');
        printf("0x%X ", m[i]);
    }
    putchar('\n');
}
*/

typedef int64_t add3(int64_t a);

T(radd3) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(9), R(DI));
    x64_push_r(&p, m, R(9));
    x64_pop_r(&p, m, R(AX));
    x64_add_rb(&p, m, R(AX), 3);
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    uint64_t a = 3;
    uint64_t r = ((add3*) m)(a);
    printf("add3: %ld, %ld\n", a, r);
    A(r == a + 3, "add3");
}

typedef int64_t add(int64_t a, int64_t b);

T(radd) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(AX), R(DI));
    x64_add_rr(&p, m, R(AX), R(SI));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    uint64_t a = 1, b = 3;
    uint64_t r = ((add*) m)(a, b);
    printf("add: %ld + %ld = %ld\n", a, b, r);
    A(r == a + b, "add");
}
typedef int64_t sub(int64_t a, int64_t b);

T(rsub) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_mov_rr(&p, m, R(12), R(DI));
    x64_sub_rr(&p, m, R(12), R(SI));
    x64_mov_rr(&p, m, R(9), R(12));
    x64_mov_rr(&p, m, R(AX), R(9));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    int64_t a = 20, b = 9;
    X64_RS();
    int64_t r = ((sub*) m)(a, b);
    printf("sub: %ld - %ld = %ld\n", a, b, r);
    A(r == a - b, "sub");
    X64_RR();
}

typedef int64_t loop(int64_t a);

static void x64_printf(size_t *p, uint8_t *m, const char *fmt) {
    x64_xor_rr(p, m, R(AX), R(AX));
    x64_mov_rq(p, m, R(13), P(&printf));
    x64_mov_rq(p, m, R(DI), P(fmt));
    x64_mov_rrm(p, m, R(SI), R(SP));
    x64_call_r(p, m, R(13));
}

T(rloop) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
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
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    uint64_t a = 12;
    X64_RS();
    uint64_t b = ((loop*) m)(a);
    X64_RR();
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
    p = 0;
    x64_mp_w(JIT_P, p, m);
    bfib(&p, m);
    uint64_t n = 10;
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    uint64_t r = ((fib*) m)(n);
    printf("fib(%lu): %lu\n", n, r);
    A(r == 55, "fib");
}

T(daddsub) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_addsd_xx(&p, m, XMM(0), XMM(1));
    x64_subsd_xx(&p, m, XMM(0), XMM(2));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    double a = 1.2, b = 3.4, c = 0.15;
    double d = ((double(*)(double, double, double)) m)(a, b, c);
    printf("dasf(%f + %f - %f): %f\n", a, b, c, d);
    A(d = a + b - c, "double");
}

T(cmp) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_push_r(&p, m, R(BP));
    x64_mov_rr(&p, m, R(BP), R(SP));
    x64_cmp_rr(&p, m, R(DI), R(SI));
    x64_setlsetnge_r(&p, m, R(AX)); // only effect lower bits
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
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
    p = 0;
    x64_mp_w(JIT_P, p, m);
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
    x64_lea_rrmb(&p, m, R(DI), R(BP), -8);
    x64_mov_rq(&p, m, R(AX), P(&printp));
    x64_call_r(&p, m, R(AX));
    x64_pop_r(&p, m, R(CX));
    x64_pop_r(&p, m, R(BP));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    ((void(*)(int64_t**)) m)(&a);
    printp(&a);
    A(*a == 23, "inv ref value");
    free(a);
}

T(rskiploop) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_mov_rq(&p, m, R(AX), U6(1));
    x64_push_r(&p, m, R(AX));
    x64_mov_rr(&p, m, R(SI), R(DI));
    uint32_t loop = p;
    x64_mov_rrm(&p, m, R(AX), R(SP));
    x64_cmp_rr(&p, m, R(SI), R(AX));
    x64_jbjnaejc_d(&p, m, 0);
    uint32_t test = p;
    x64_push_r(&p, m, R(SI));
    x64_printf(&p, m, "Dec: %lu\n");
    x64_pop_r(&p, m, R(SI));
    x64_dec_r(&p, m, R(SI));
    x64_jmp_d(&p, m, x64_jmpu_lbld(p, loop));
    x64_jmpd_lbld(m, test, p);
    x64_pop_r(&p, m, R(AX));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    A(((int32_t(*)(int32_t)) m)(5) == 1, "loop");
}

T(cvtsi2sd) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_movq_xr(&p, m, XMM(2), R(DI));
    x64_movq_rx(&p, m, R(8), XMM(2));
    x64_cvtsi2sd_xr(&p, m, XMM(11), R(8));
    x64_movq_xx(&p, m, XMM(0), XMM(11));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    int64_t r = 123; double x = 123.0;
    A(((double(*)(int64_t)) m)(r) == x, "cvtsi2sd");
}

static const char *comisdg = "Greater Equal";

static const char *comisdl = "Less";

T(comisd) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
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
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    double x = 1.2, y = 3.4;
    const char *r = ((char*(*)(double, double)) m)(x, y);
    printf("comisd(%lf, %lf): %s\n", x, y, r);
    A(r == comisdl, "Less");
}

T(cmprip) {
    int64_t c = INT64_MAX;
    p = 0;
    x64_mp_w(JIT_P, p, m);
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
    x64_jmpd_lbld(m, rips, ripe);
    x64_e(&p, m, sizeof(int64_t), I6(c));
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    int64_t r = ((int64_t(*)(int64_t)) m)(INT64_MAX);
    printf("rip disp32: %d, %ld == %ld\n", ripe - rips, c, r);
    A(c == r, "ne");
}

T(imulidiv) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_mov_rr(&p, m, R(AX), R(DI));
    x64_mov_rr(&p, m, R(BX), R(DX));
    x64_imul_r(&p, m, R(SI));
    x64_idiv_r(&p, m, R(BX));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    int64_t a = 6, b = 7, c = 2;
    X64_RS();
    int64_t r =  ((int64_t(*)(int64_t, int64_t, int64_t)) m)(a, b, c);
    printf("%ld * %ld / %ld = %ld\n", a, b, c, r);
    A(r == 21, "imulidiv");
    X64_RR();
}

T(pxor) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_pxor_xx(&p, m, XMM(0), XMM(0));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    X64_RS();
    A(!((double(*)(double)) m)(1.1), "xor");
    X64_RR();
}

T(sib) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    int64_t a[] = {1, 2, 3};
    int64_t v = 5;
    x64_mov_rrmo(&p, m, R(AX), R(DI), R(SI), S8);
    x64_ret(&p, m);
    //printj(p, m);
    x64_mp_rx(JIT_P, &p, m);
    int64_t r = ((int64_t(*)(int64_t*, size_t)) m)(a, 2);
    A(r == a[2], "inv sib");
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_mov_rmor(&p, m, R(DI), R(SI), S8, R(DX));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    ((void(*)(int64_t*, size_t, int64_t)) m)(a, 1, v);
    A(a[1] == v, "inv sib");
    for (size_t i = 0; i < 3; i++) printf("%ld\n", a[i]);
}

T(sibupper) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    int64_t a[] = {5, 6, 7};
    x64_mov_rr(&p, m, R(10), R(DI));
    x64_mov_rr(&p, m, R(11), R(SI));
    x64_mov_rrmo(&p, m, R(AX), R(10), R(11), S8);
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    //printj(p, m);
    int64_t r = ((int64_t(*)(int64_t volatile*, size_t)) m)(a, 1);
    for (size_t i = 0; i < 3; i++) printf("%ld ", a[i]);
    putchar('\n');
    printf("%ld = %ld\n", r, a[1]);
    A(r == a[1], "inv sib");
}

T(addp) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    int64_t a = 5;
    x64_add_rmr(&p, m, R(DI), R(SI));
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    ((void(*)(int64_t*, int64_t)) m)(&a, 4);
    A(a == 9, "inv add");
}

T(movsd_xrmo) {
    p = 0;
    x64_mp_w(JIT_P, p, m);
    x64_movsd_xrmo(&p, m, XMM(0), R(DI), R(SI), S8);
    x64_ret(&p, m);
    x64_mp_rx(JIT_P, &p, m);
    // double v[] = {1.1, 2.2, 3.3};
    double *v = calloc(3, sizeof(double));
    v[0] = 1.1;
    v[1] = 2.2;
    v[2] = 3.3;
    double r = ((double(*)(double[], uint64_t)) m)(v, 2);
    A(r == v[2], "sib");
    free(v);
}
