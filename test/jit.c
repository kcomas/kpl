
#include "../src/jit.h"

typedef int64_t add3(int64_t a);

void printj(size_t len, uint8_t *m) {
    printf("---- START ----\n");
    printf("len: %lu\n", len);
    for (size_t i = 0; i < len; i++) {
        printf("0x%X\n", m[i]);
    }
    printf("---- END ----\n");
}

int main(void) {
    size_t p = 0;
    uint8_t *m = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    jit_push(&p, m, R(BP));
    jit_movrr(&p, m, R(BP), R(SP));
    jit_addrb(&p, m, R(DI), 3);
    jit_movrr(&p, m, R(AX), R(DI));
    jit_pop(&p, m, R(BP));
    jit_ret(&p, m);
    printj(p, m);
    printf("%ld\n", ((add3*) m)(5));
    munmap(m, getpagesize());
    return 0;
}
