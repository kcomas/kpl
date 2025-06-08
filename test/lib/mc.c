
#include "../../src/lib/mc.h"
#include <stdio.h>

static const alfr mm = { .a = &malloc, .f = &free };

int main(void) {
    mc *m = mc_i(1, &mm);
    for (int16_t i = 0; i < 10; i++) mc_wb(&m, sizeof(int16_t), (uint8_t*) &i);
    for (size_t i = 0; i < 10; i++) printf("%d ", ((int16_t*) m->d)[i]);
    putchar('\n');
    mc_f(m);
    return 0;
}
