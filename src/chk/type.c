
#include "../chk.h"

static chk_stat chk_type_fn(chk *c, te *an, err **e) {
    (void) c;
    (void) e;
    type_rrf((te**) &an->d[3].p);
    return CHK_STAT(OK);
}

void chk_type(chk *c) {
    CHK_AA(c, chk_nop, AST_CLS(T), TYPE(U6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(T), TYPE(I6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_nop, AST_CLS(T), TYPE(F6), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(TE), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(ST), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(VR), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(FN), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(NF), AST_CLS(_), TYPE(_A));
    CHK_AA(c, chk_type_fn, AST_CLS(T), TYPE(CJ), AST_CLS(_), TYPE(_A));
}
