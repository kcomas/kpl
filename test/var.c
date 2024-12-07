
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
    return 0;
}
