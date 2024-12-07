
#include "../src/var.h"

int main(void) {
    var *nl = VAR_I(CR, cr, C1('\n'));
    var *space = VAR_I(CR, cr, C1(' '));
    var *a = VAR_I(I6, i6, 3);
    var *b = VAR_I(I6, i6, 4);
    var_p(a, space, nl, NULL);
    var_p(b, space, nl, NULL);
    var *c = VAR_I(CR, cr, C2(0xCE, 0xA9));
    var_p(c, space, nl, NULL);
    var *l = VAR_I(LST, lst, var_lst_i());
    var_lst_a(l->d.lst, a);
    var_lst_a(l->d.lst, b);
    var_lst_a(l->d.lst, c);
    var *s = VAR_I(CR, cr, C2(0xC2, 0xBF));
    var_p(l, space, nl, s);
    var_f(a);
    var_f(b);
    var_f(c);
    var_f(l);
    var_f(s);
    var_f(nl);
    var_f(space);
    return 0;
}
