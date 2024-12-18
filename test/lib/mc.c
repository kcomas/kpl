
#include "../../src/lib/mc.h"
#include <stdio.h>

int main(void) {
    mc *m = mc_i(1, &malloc, &free);
    for (int16_t i = 0; i < 10; i++) mc_wb(&m, (uint8_t*) &i, sizeof(int16_t));
    for (size_t i = 0; i < 10; i++) printf("%d ", ((int16_t*) m->d)[i]);
    putchar('\n');
    mc_f(m);
    return 0;
}
