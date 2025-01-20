
#include "../../src/lib/tkn.h"
#include <ctype.h>
#include <stdio.h>

#define TCUST(N) TCUST_##N

typedef enum {
    TCUST(NL) = TOKEN(USR),
    TCUST(WS),
    TCUST(LB),
    TCUST(RB),
    TCUST(LP),
    TCUST(RP),
    TCUST(LS),
    TCUST(RS),
    TCUST(NUM),
    TCUST(SEMI),
    TCUST(VAR),
    TCUST(SYM)
} tcust;

void tkn_p(tbl *tl, size_t idnt);

void tkn_m_p(const te *m, const mc *s);

size_t tkn_sh(un v);

tbl *tkn_mktbl(void);

void tkn_entry_f(void *v);

tkn_stat tkn_df(tkn *t, te *m);

tkn_stat tkn_num(tkn *t, te *m);

tkn_stat tkn_nl(tkn *t, te *m);

tkn_stat tkn_ft(tkn *t, te *m);

tkn_stat tkn_ws(tkn *t, te *m);

tkn_stat tkn_sym(tkn *t, te *m);

void tkn_standard(tkn *t);
