
#include "./lib/tkn.h"
#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#define TCUST(N) TCUST_##N

typedef enum {
    TCUST(WS) = TOKEN(USR),
    TCUST(TAB),
    TCUST(SEMI),
    TCUST(NL),
    TCUST(CMT),
    TCUST(LB),
    TCUST(RB),
    TCUST(LP),
    TCUST(RP),
    TCUST(LS),
    TCUST(RS),
    TCUST(SYM),
    TCUST(STR),
    TCUST(NUM),
    TCUST(VAR),
    TCUST(DOT),
    // types
    TCUST(VD),
    TCUST(I6),
    TCUST(U6),
    TCUST(F6),
    TCUST(FN),
    TCUST(NF),
    TCUST(UN),
    TCUST(VR),
    // keys
    // ops
    TCUST(DFN),
    TCUST(AGN),
    TCUST(CST),
    TCUST(IF),
    TCUST(LOOP),
    TCUST(MTCH),
    TCUST(ADD),
    TCUST(ADDA),
    TCUST(SUB),
    TCUST(SUBA),
    TCUST(MUL),
    TCUST(MULA),
    TCUST(DIV),
    TCUST(EQ),
    TCUST(GT),
    TCUST(GTE),
    TCUST(LT),
    TCUST(LTE),
    TCUST(AND),
    TCUST(OR),
    TCUST(CNCT),
    TCUST(CNCTA),
    // cmds
    TCUST(E),
    TCUST(L),
    TCUST(P1),
    TCUST(_END)
} tcust;

void tkn_p(tbl *tl, size_t idnt);

void tkn_m_p(const te *m, const mc *s);

size_t tkn_sh(un v);

tkn_stat tkn_df(tkn *t, te *m, err **e);

tkn_stat tkn_num(tkn *t, te *m, err **e);

tkn_stat tkn_nl(tkn *t, te *m, err **e);

tkn_stat tkn_ft(tkn *t, te *m, err **e);

tkn_stat tkn_ws(tkn *t, te *m, err **e);

tkn_stat tkn_cmt(tkn *t, te *m, err **e);

tkn_stat tkn_sym(tkn *t, te *m, err **e);

tkn *tkn_b(tkn *t);

tkn_stat tkn_g_i6(const te *t, const mc *s, int64_t *i);

tkn_stat tkn_g_f6(const te *restrict tu, const te *restrict tm, const te *restrict tl, const mc *s, double *d);

// byte offset + for off start - for off end
tkn_stat tkn_g_mc(const te *t, const mc *s, ssize_t off, const alfr *af, mc **v);

tkn_stat tkn_g_str(const te *t, const mc *s, const alfr *af, mc **v);
