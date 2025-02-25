
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
    TCUST(I6),
    TCUST(U6),
    TCUST(F6),
    TCUST(FN),
    TCUST(UN),
    // keys
    // ops
    TCUST(AGN),
    TCUST(CST),
    TCUST(ADD),
    TCUST(SUB),
    // cmds
    TCUST(P1),
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

tkn_stat tkn_g_i6(const te *t, const mc *s, int64_t *i);

// byte offset + for off start - for off end
tkn_stat tkn_g_mc(const te *t, const mc *s, ssize_t off, const alfr *af, mc **v);
