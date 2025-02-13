
#include "../src/type.h"
#include "t.h"

I;

const alfr tm = { .a = malloc, .f = free };

T(scalar, {
    te *a = type_s_i(&tm, TYPE(I6));
    te *b = type_s_i(&tm, TYPE(I6));
    A(type_eq(a, b), "equal");
    te *c = type_s_i(&tm, TYPE(F6));
    A(!type_eq(b, c), "not equal");
    te_f(a);
    te_f(b);
    te_f(c);
});
