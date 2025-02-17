
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
