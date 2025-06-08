
#include <stdio.h>
#include "../../src/lib/jit.h"

static void printj(size_t len, uint8_t *m) {
    printf("- %lu - ", len);
    for (size_t i = 0; i < len; i++) {
        if (i % 30 == 0 || m[i] == 0x90) putchar('\n');
        printf("0x%X ", m[i]);
    }
    putchar('\n');
}

typedef int64_t add3(int64_t a);

static void radd3(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_mov_rr(&p, m, R(9), R(DI));
    jit_push(&p, m, R(9));
    jit_pop(&p, m, R(AX));
    jit_add_rb(&p, m, R(AX), 3);
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    uint64_t a = 3;
    printf("add3: %ld, %ld\n", a, ((add3*) m)(a));
}

typedef int64_t add(int64_t a, int64_t b);

static void radd(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_mov_rr(&p, m, R(AX), R(DI));
    jit_add_rr(&p, m, R(AX), R(SI));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    uint64_t a = 1, b = 3;
    printf("add: %ld + %ld = %ld\n", a, b, ((add*) m)(a, b));
}

typedef int64_t sub(int64_t a, int64_t b);

static void rsub(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_mov_rr(&p, m, R(12), R(DI));
    jit_sub_rr(&p, m, R(12), R(SI));
    jit_mov_rr(&p, m, R(9), R(12));
    jit_mov_rr(&p, m, R(AX), R(9));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    uint64_t a = 20, b = 9;
    printf("sub: %ld - %ld = %ld\n", a, b, ((sub*) m)(a, b));
}

typedef int64_t loop(int64_t a);

static void rloop(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_mov_rr(&p, m, R(8), R(DI));
    jit_push(&p, m, R(8));
    jit_xor_rr(&p, m, R(10), R(10));
    jit_push(&p, m, R(10));
    size_t lbl = p;
    jit_mov_rq(&p, m, R(AX), P(&printf));
    jit_mov_rq(&p, m, R(DI), P("V: %ld\n"));
    jit_mov_rra(&p, m, R(SI), R(SP));
    jit_call_r(&p, m, R(AX));
    jit_mov_rra(&p, m, R(10), R(SP));
    jit_inc_r(&p, m, R(10));
    jit_mov_rar(&p, m, R(SP), R(10));
    jit_mov_rrab(&p, m, R(8), R(SP), 8);
    jit_cmp_rr(&p, m, R(10), R(8));
    jit_jne_b(&p, m, jit_jmpu_lblb(p, lbl));
    jit_mov_rr(&p, m, R(AX), R(10));
    jit_leave(&p, m);
    jit_ret(&p, m);
    printj(p, m);
    uint64_t a = 12;
    printf("%ld\n", ((loop*) m)(a));
}

typedef uint64_t fib(uint64_t n);

static void bfib(size_t *p, uint8_t *m) {
    jit_push(p, m, R(BP));
    jit_mov_rr(p, m, R(BP), R(SP));
    jit_push(p, m, R(DI)); // push n
    // if n == 0
    jit_mov_rq(p, m, R(9), U6(0));
    jit_mov_rra(p, m, R(DI), R(SP));
    jit_cmp_rr(p, m, R(9), R(DI));
    jit_jne_b(p, m, 0);
    uint8_t *byte = jit_lb(*p, m);
    size_t lbl = *p;
    jit_mov_rq(p, m, R(AX), U6(0));
    jit_leave(p, m);
    jit_ret(p, m);
    jit_jmpd_lblb(byte, lbl, *p);
    // if n <= 2
    jit_mov_rq(p, m, R(9), U6(2));
    jit_mov_rra(p, m, R(DI), R(SP));
    jit_cmp_rr(p, m, R(9), R(DI));
    jit_jc_b(p, m, 0);
    byte = jit_lb(*p, m);
    lbl = *p;
    jit_mov_rq(p, m, R(AX), U6(1));
    jit_leave(p, m);
    jit_ret(p, m);
    jit_jmpd_lblb(byte, lbl, *p);
    // fib(n - 1)
    jit_mov_rra(p, m, R(DI), R(SP));
    jit_sub_rb(p, m, R(DI), 1);
    jit_mov_rq(p, m, R(AX), P(m));
    jit_call_r(p, m, R(AX));
    jit_push(p, m, R(AX));
    // fib(n - 2)
    jit_mov_rrab(p, m, R(DI), R(BP), -8);
    jit_sub_rb(p, m, R(DI), 2);
    jit_mov_rq(p, m, R(AX), P(m));
    jit_call_r(p, m, R(AX));
    jit_pop(p, m, R(DI));
    jit_add_rr(p, m, R(AX), R(DI));
    jit_leave(p, m);
    jit_ret(p, m);
}

static void rfib(uint8_t *m) {
    size_t p = 0;
    bfib(&p, m);
    uint64_t n = 35;
    printj(p, m);
    printf("fib(%lu): %lu\n", n, ((fib*) m)(n));
}

static void daddsub(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_addsd_rr(&p, m, XMM(0), XMM(1));
    jit_subsd_rr(&p, m, XMM(0), XMM(2));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    double a = 1.2, b = 3.4, c = 0.15;
    double d = ((double(*)(double, double, double)) m)(a, b, c);
    printf("dasf(%f + %f - %f): %f\n", a, b, c, d);
}

static void cmp(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_cmp_rr(&p, m, R(DI), R(SI));
    jit_setl_r(&p, m, R(AX));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    int64_t a = 1, b = 2;
    printf("cmp(%ld < %ld): %d\n", a, b, ((bool(*)(int64_t, int64_t)) m)(a, b));
    printf("cmp(%ld < %ld): %d\n", b, a, ((bool(*)(int64_t, int64_t)) m)(b, a));
}

static void p2p(uint8_t *m) {
    size_t p = 0;
    int64_t *a = malloc(sizeof(int64_t));
    *a = 1;
    printf("%p, %ld\n", a, *a);
    jit_push(&p, m, R(BP));
    jit_mov_rr(&p, m, R(BP), R(SP));
    jit_mov_rra(&p, m, R(DI), R(DI));
    jit_mov_rq(&p, m, R(SI), I6(23));
    jit_mov_rar(&p, m, R(DI), R(SI));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    ((void(*)(int64_t**)) m)(&a);
    printj(p, m);
    printf("%p, %ld\n", a, *a);
    free(a);
}

int main(void) {
    uint8_t *m = jit_mmap(1);
    radd3(m);
    radd(m);
    rsub(m);
    rloop(m);
    rfib(m);
    daddsub(m);
    cmp(m);
    p2p(m);
    jit_munmap(1, m);
    return 0;
}
