
#include "../src/lib/mc.h"
#include "t.h"

T(mc_wb) {
    mc *m = mc_i(1, &al_mc);
    for (int16_t i = 0; i < 10; i++) mc_wb(&m, sizeof(int16_t), (uint8_t*) &i);
    for (size_t i = 0; i < 10; i++) printf("%d ", ((int16_t*) m->d)[i]);
    putchar('\n');
    A(m->l == sizeof(int16_t) * 10, "len");
    mc_f(m);
}

T(mc_eq) {
    mc *a = mc_i_cstr("abc", &al_mc);
    mc *b = mc_i_cstr("abc", &al_mc);
    A(mc_eq(a, b), "mc_eq");
    mc *c = mc_i_cstr("abd", &al_mc);
    A(!mc_eq(b, c), "!mc_eq");
    mc_f(a);
    mc_f(b);
    mc_f(c);
}

T(str_cnct) {
    mc *a = mc_i_cstr("_", &al_mc);
    mc *b = mc_i_cstr("a", &al_mc);
    mc *c = mc_str_cnct(a, b);
    mc *d = mc_i_cstr("_a", &al_mc);
    printf("%s\n", (char*) c->d);
    A(mc_eq(c, d), "inv cnct");
    mc_f(a);
    mc_f(b);
    mc_f(c);
    mc_f(d);
}
