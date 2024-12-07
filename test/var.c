
#include "../src/var.h"

int main(void) {
    var *a = VAR_I(I6, i6, 3);
    var *b = VAR_I(I6, i6, 4);
    var_p(a, 0);
    putchar('\n');
    var_p(b, 0);
    putchar('\n');
    var_f(a);
    var_f(b);
    var *c = VAR_I(CR, cr, U8(0xCE, 0xA9, '\0', '\0'));
    var_p(c, 0);
    putchar('\n');
    var_f(c);
    return 0;
}
