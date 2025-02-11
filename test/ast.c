
#include "../src/ast.h"
#include "t.h"

I;

const alfr am = { .a = malloc, .f = free };

static ast *ai(void) {
    lst *tl = lst_i(&am, &am, (void*) te_f);
    te *b = te_i(10, &am, NULL);
    tbl *t = tbl_i(&am, tbl_no_hsh, tbl_un_eq, tl, b);
    return ast_i(&am, &am, t);
}

T(init, {
    ast *a = ai();
    ast_f(a);
});
