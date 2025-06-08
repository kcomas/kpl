
#include "../src/lib/type.h"
#include "t.h"

T(scalar) {
    te *a = type_s_i(&al_te, NULL, TYPE(I6));
    te *b = type_s_i(&al_te, NULL, TYPE(I6));
    type_p(b);
    putchar('\n');
    A(type_eq(a, b), "equal");
    te *c = type_s_i(&al_te, NULL, TYPE(F6));
    A(!type_eq(b, c), "not equal");
    te_f(a);
    te_f(b);
    te_f(c);
}

T(vector) {
    te *va = type_v_i(&al_te, NULL, TYPE(VR), type_s_i(&al_te, NULL, TYPE(U6)));
    type_p(va);
    putchar('\n');
    te *vb = type_v_i(&al_te, NULL, TYPE(LT), type_s_i(&al_te, NULL, TYPE(U6)));
    A(!type_eq(va, vb), "vr lt");
    te *vc = type_v_i(&al_te, NULL, TYPE(LT), type_s_i(&al_te, NULL, TYPE(U6)));
    A(type_eq(vb, vc), "lt lt");
    te_f(va);
    te_f(vb);
    te_f(vc);
}

T(circular) {
    te *va = type_v_i(&al_te, NULL, TYPE(VR), type_s_i(&al_te, NULL, TYPE(U6)));
    te *vb = type_v_i(&al_te, va, TYPE(VR), type_s_i(&al_te, NULL, TYPE(U6)));
    A(type_ic(vb, va), "type_ic");
    te_f(va);
    te_f(vb);
}

T(tetype) {
    te *arr[] = {type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6))};
    te *a = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    type_p(a);
    putchar('\n');
    te *b = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    A(type_eq(a, b), "type_eq");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    te_f(b);
}
