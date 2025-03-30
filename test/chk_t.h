
#include "../src/chk.h"
#include "fld_t.h"

tbl *cti(void);

void rchk(_tests *_t, chk *c, te *an);

#define IC(PGM) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, a, &an, fld_b, true); \
    chk *c = chk_i(&ast_am, &ast_am, cti, a); \
    chk_b(c);

#define RC() rchk(_t, c, an); \
    E()
