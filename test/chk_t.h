
#include "../src/chk.h"
#include "fld_t.h"

extern chk *bchk;

tbl *cti(void);

void rchk(_tests *_t, chk *c, te *an);

#define IC(PGM) ast *a = ast_i_ast(bast); \
    te *an = NULL; \
    astb(_t, a, PGM, &an); \
    fast(_t, a, &an, bfld, true); \
    chk *c = chk_i_chk(bchk, a)

#define RC() rchk(_t, c, an); \
    E()
