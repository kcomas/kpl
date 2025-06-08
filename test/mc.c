
#include "../src/lib/mc.h"
#include "t.h"

I;

static const alfr mm = { .a = &malloc, .f = &free };

T(mc_wb, {
    mc *m = mc_i(1, &mm);
    for (int16_t i = 0; i < 10; i++) mc_wb(&m, sizeof(int16_t), (uint8_t*) &i);
    for (size_t i = 0; i < 10; i++) printf("%d ", ((int16_t*) m->d)[i]);
    putchar('\n');
    A(m->l == sizeof(int16_t) * 10, "len");
    mc_f(m);
});
