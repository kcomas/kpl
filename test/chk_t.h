
#include "../src/chk.h"
#include "fld_t.h"

tbl *cti(void);

#define IC(PGM) ast *a = ast_b(ast_i(&ast_am, &ast_am, &ast_am, pig, ali, mktbl(NODE_TYPE(_END)), mktbl(TCUST(_END)))); \
    te *an = NULL; \
    bast(_t, a, PGM, &an); \
    fast(_t, a, &an, fld_b, true); \
    chk *c = chk_i(&ast_am, &ast_am, cti, a); \
    chk_b(c);

#define RC(N) te *e = NULL; \
    chk_stat stat = chk_n(c, an, &e); \
    chk_f(c); \
    if (e) { \
        ast_p(e, 0); \
        putchar('\n'); \
    } \
    A(stat == CHK_STAT(OK), "chk_n");
