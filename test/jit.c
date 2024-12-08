
#include "../src/jit.h"

static void printj(size_t len, uint8_t *m) {
    printf("---- START ----\n");
    printf("len: %lu\n", len);
    for (size_t i = 0; i < len; i++) {
        printf("0x%X\n", m[i]);
    }
    printf("---- END ----\n");
}

typedef int64_t add3(int64_t a);

static void radd3(uint8_t *m) {
    size_t p = 0;
    jit_push(&p, m, R(BP));
    jit_movrr(&p, m, R(BP), R(SP));
    jit_addrb(&p, m, R(DI), 3);
    jit_movrr(&p, m, R(AX), R(DI));
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

int main(void) {
    uint8_t *m = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    radd3(m);
    radd(m);
    rsub(m);
    munmap(m, getpagesize());
    return 0;
}
