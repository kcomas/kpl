
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
    te *c = type_te_i_v(&al_te, NULL, 2, arr[0], arr[1]);
    A(!type_eq(b, c), "!type_eq");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    te_f(b);
    te_f(c);
}

T(typeteeq) {
    te *arr[] = {type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6))};
    te *a = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    type_p(a);
    putchar('\n');
    A(type_te_eq(a), "type_te_eq");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    arr[0] = type_s_i(&al_te, NULL, TYPE(U6));
    arr[1] = type_s_i(&al_te, NULL, TYPE(I6));
    arr[2] = type_s_i(&al_te, NULL, TYPE(F6));
    a = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    type_p(a);
    putchar('\n');
    A(!type_te_eq(a), "!type_te_eq");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    te *b = type_s_i(&al_te, NULL, TYPE(F6));
    a = type_te_i_v(&al_te, NULL, 1, b);
    type_p(a);
    putchar('\n');
    A(type_te_eq(a), "type_te_eq");
    te_f(b);
    te_f(a);
}

T(is_ref) {
    A(type_is_ref(TYPE(SG)), "sg");
    A(!type_is_ref(TYPE(I6)), "!i6");
    A(type_is_ref(TYPE(TE)), "te");
}

T(type_hsh) {
    te *arr[] = {type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6))};
    te *a = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    type_p(a);
    putchar('\n');
    te *b = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    A(type_hsh(a) == type_hsh(b), "type_hsh");
    te *c = type_te_i_v(&al_te, NULL, 2, arr[0], arr[1]);
    A(type_hsh(b) != type_hsh(c), "!type_hsh");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    te_f(b);
    te_f(c);
}

T(has_refs) {
    te *arr[] = {type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6)), type_s_i(&al_te, NULL, TYPE(U6))};
    te *a = type_te_i_v(&al_te, NULL, 3, arr[0], arr[1], arr[2]);
    type_p(a);
    putchar('\n');
    A(!type_has_refs(a), "!has_refs");
    for (size_t i = 0; i < 3; i++) te_f(arr[i]);
    te_f(a);
    te *s = type_s_i(&al_te, NULL, TYPE(SG));
    te *i = type_s_i(&al_te, NULL, TYPE(U6));
    te *b = type_te_i_v(&al_te, NULL, 2, s, i);
    type_p(b);
    putchar('\n');
    A(type_has_refs(b), "has_refs");
    te_f(s);
    te_f(i);
    te_f(b);
}
