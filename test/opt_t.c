
#include "opt_t.h"

fld *bopt = NULL;

static __attribute__((constructor(106))) void opt_con(void) {
    bopt = opt_b(fld_i(&ast_am, &al_te, &ast_am, ati, ali, NULL, mktbl(AST_CLS(_))));
}

static __attribute__((destructor)) void opt_des(void) {
    fld_f(bopt);
}
