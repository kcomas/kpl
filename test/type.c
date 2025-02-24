
#include "../src/type.h"
#include "t.h"

static const alfr tm = { .a = malloc, .f = free };

T(scalar) {
    te *a = type_s_i(&tm, NULL, TYPE(I6));
    te *b = type_s_i(&tm, NULL, TYPE(I6));
    type_p(b);
    putchar('\n');
    A(type_eq(a, b), "equal");
    te *c = type_s_i(&tm, NULL, TYPE(F6));
    A(!type_eq(b, c), "not equal");
    te_f(a);
    te_f(b);
    te_f(c);
}

T(vector) {
    te *va = type_v_i(&tm, NULL, TYPE(VR), type_s_i(&tm, NULL, TYPE(U6)));
    type_p(va);
    putchar('\n');
    te *vb = type_v_i(&tm, NULL, TYPE(LT), type_s_i(&tm, NULL, TYPE(U6)));
    A(!type_eq(va, vb), "vr lt");
    te *vc = type_v_i(&tm, NULL, TYPE(LT), type_s_i(&tm, NULL, TYPE(U6)));
    A(type_eq(vb, vc), "lt lt");
    te_f(va);
    te_f(vb);
    te_f(vc);
}
