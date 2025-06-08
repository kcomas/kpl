
#include "../src/lib/def.h"
#include "t.h"

T(utf8) {
    const char *greek = "AΑΔΓΦΠΣZΩ";
    /*
    putchar(greek[0]);
    putchar('\n');
    */
    size_t s = 0, e = 0;;
    un l;
    for (;;) {
        un u = c4_g(greek, s, &e);
        if (u.c.a == '\0') break;
        printf("Conts %lu, ", e - s);
        putchar(u.c.a);
        putchar(u.c.b);
        putchar(u.c.c);
        putchar(u.c.d);
        putchar('\n');
        s = e + 1;
        l = u;
    }
    un o = c4_g("Ω", 0, &e);
    A(c4_eq(l, o) , "Ω");
}

T(offset8) {
    un v;
    for (size_t i = 0; i < 8; i++) v = u3_s_o(v, i, i + 1);
    for (size_t i = 0; i < 8; i++) {
        uint8_t a = u3_g_o(v, i);
        printf("%ld: %d\n", i, a);
        A(a == i + 1, "u3_g_o");
    }
    uint8_t d = 123;
    v = u3_s_o(v, 5, d);
    printf("%d: %d\n", 5, u3_g_o(v, 5));
    A(d == u3_g_o(v, 5), "u3_g_o");
}

T(offset1632) {
    int32_t ns = 7000001;
    un v; //32;16;16
    v = u4_s_o(v, 0, 12);
    v = i4_s_o(v, 1, 32);
    v = i5_s_o(v, 1, ns);
    A(u4_g_o(v, 0) == 12, "u4_g_o");
    A(i4_g_o(v, 1) == 32, "i4_g_o");
    printf("%d\n", i5_g_o(v, 1));
    A(i5_g_o(v, 1) == ns, "i5_g_o");
}
