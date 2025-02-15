
#include "./lib/tkn.h"
#include <stdio.h>
#include <ctype.h>

#define TCUST(N) TCUST_##N

typedef enum {
    TCUST(WS) = TOKEN(USR),
    TCUST(SEMI),
    TCUST(NL),
    TCUST(LB),
    TCUST(RB),
    TCUST(LP),
    TCUST(RP),
    TCUST(LS),
    TCUST(RS),
    TCUST(SYM),
    TCUST(NUM),
    TCUST(VAR),
    // types
    // keys
    // ops
    TCUST(ADD),
    TCUST(SUB),
    TCUST(_END)
} tcust;

void tkn_p(tbl *tl, size_t idnt);

void tkn_m_p(const te *m, const mc *s);

size_t tkn_sh(un v);

void tkn_entry_f(void *v);

tkn_stat tkn_df(tkn *t, te *m);

tkn_stat tkn_num(tkn *t, te *m);

tkn_stat tkn_nl(tkn *t, te *m);

tkn_stat tkn_ft(tkn *t, te *m);

tkn_stat tkn_ws(tkn *t, te *m);

tkn_stat tkn_sym(tkn *t, te *m);

void tkn_b(tkn *t);

int64_t tkn_g_i6(const te *t, const mc *s);
