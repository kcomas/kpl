
#include <stdio.h>
#include "../src/jit.h"

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
    jit_movrr(&p, m, R(BP), R(SP));
    jit_movrr(&p, m, R(9), R(DI));
    jit_push(&p, m, R(9));
    jit_pop(&p, m, R(AX));
    jit_addrb(&p, m, R(AX), 3);
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
    jit_movrr(&p, m, R(BP), R(SP));
    jit_movrr(&p, m, R(AX), R(DI));
    jit_addrr(&p, m, R(AX), R(SI));
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
    jit_movrr(&p, m, R(BP), R(SP));
    jit_movrr(&p, m, R(12), R(DI));
    jit_subrr(&p, m, R(12), R(SI));
    jit_movrr(&p, m, R(9), R(12));
    jit_movrr(&p, m, R(AX), R(9));
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
    jit_movrr(&p, m, R(BP), R(SP));
    jit_movrr(&p, m, R(8), R(DI));
    jit_push(&p, m, R(8));
    jit_xorrr(&p, m, R(10), R(10));
    jit_push(&p, m, R(10));
    size_t lbl = p;
    jit_movrq(&p, m, R(AX), &printf);
    jit_movrq(&p, m, R(DI), "V: %ld\n");
    jit_movrra(&p, m, R(SI), R(SP));
    jit_callr(&p, m, R(AX));
    jit_movrra(&p, m, R(10), R(SP));
    jit_incr(&p, m, R(10));
    jit_movrar(&p, m, R(SP), R(10));
    jit_movrrab(&p, m, R(8), R(SP), 8);
    jit_cmprr(&p, m, R(10), R(8));
    jit_jneb(&p, m, jit_jmpu_lblb(p, lbl));
    jit_movrr(&p, m, R(AX), R(10));
    jit_leave(&p, m);
    jit_ret(&p, m);
    printj(p, m);
    uint64_t a = 12;
    printf("%ld\n", ((loop*) m)(a));
}

typedef uint64_t fib(uint64_t n);

static void bfib(size_t *p, uint8_t *m) {
    jit_push(p, m, R(BP));
    jit_movrr(p, m, R(BP), R(SP));
    jit_push(p, m, R(DI)); // push n
    // if n == 0
    jit_movrq(p, m, R(9), (void*) 0);
    jit_movrra(p, m, R(DI), R(SP));
    jit_cmprr(p, m, R(9), R(DI));
    jit_jneb(p, m, 0);
    uint8_t *byte = jit_lb(*p, m);
    size_t lbl = *p;
    jit_movrq(p, m, R(AX), (void*) 0);
    jit_leave(p, m);
    jit_ret(p, m);
    jit_jmpd_lblb(byte, lbl, *p);
    // if n <= 2
    jit_movrq(p, m, R(9), (void*) 2);
    jit_movrra(p, m, R(DI), R(SP));
    jit_cmprr(p, m, R(9), R(DI));
    jit_jcb(p, m, 0);
    byte = jit_lb(*p, m);
    lbl = *p;
    jit_movrq(p, m, R(AX), (void*) 1);
    jit_leave(p, m);
    jit_ret(p, m);
    jit_jmpd_lblb(byte, lbl, *p);
    // fib(n - 1)
    jit_movrra(p, m, R(DI), R(SP));
    jit_subrb(p, m, R(DI), 1);
    jit_movrq(p, m, R(AX), m);
    jit_callr(p, m, R(AX));
    jit_push(p, m, R(AX));
    // fib(n - 2)
    jit_movrrab(p, m, R(DI), R(BP), -8);
    jit_subrb(p, m, R(DI), 2);
    jit_movrq(p, m, R(AX), m);
    jit_callr(p, m, R(AX));
    jit_pop(p, m, R(DI));
    jit_addrr(p, m, R(AX), R(DI));
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

int main(void) {
    uint8_t *m = jit_mmap(1);
    radd3(m);
    radd(m);
    rsub(m);
    rloop(m);
    rfib(m);
    jit_munmap(1, m);
    return 0;
}
