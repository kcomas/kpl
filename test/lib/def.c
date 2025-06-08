
#include "../../src/lib/def.h"
#include <stdio.h>

int main(void) {
    const char *greek = "ΑΔΓΦΠΣΩ";
    /*
    putchar(greek[0]);
    putchar('\n');
    */
    size_t s = 0, e = 0;;
    for (;;) {
        un u = c4_g(greek, s, &e);
        if (u.c.a == '\0') break;
        putchar(u.c.a);
        putchar(u.c.b);
        putchar(u.c.c);
        putchar(u.c.d);
        putchar('\n');
        s = e + 1;
    }
    return 0;
}
