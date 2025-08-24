
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
    TCUST(SG),
    TCUST(FN),
    TCUST(NF),
    TCUST(CJ),
    TCUST(ST),
    TCUST(UN),
    TCUST(VR),
    // keys
    TCUST(A),
    // regex
    TCUST(REGT),
    TCUST(REGM),
    TCUST(REGS),
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
    TCUST(DIVA),
    TCUST(MOD),
    TCUST(NOT),
    TCUST(EQ),
    TCUST(NE),
    TCUST(GT),
    TCUST(GTE),
    TCUST(LT),
    TCUST(LTE),
    TCUST(AND),
    TCUST(OR),
    TCUST(CNCT),
    TCUST(CNCTA),
    TCUST(UNER),
    // cmds
    TCUST(E),
    TCUST(L),
    TCUST(T),
    TCUST(RT),
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

#define REG_MODE(N) REG_MODE_##N

typedef enum {
    REG_MODE(N), // none
    REG_MODE(T), // test
    REG_MODE(_)
} reg_mode;

const char *reg_mode_str(reg_mode m);

tkn_stat tkn_g_reg_mode(const te *t, const mc *s, uint16_t *mode);

#define REG_FLG(N) REG_FLG_##N

typedef enum {
    REG_FLG(I) = 1 << 0, // case insensitive
    REG_FLG(G) = 1 << 1 // global
} reg_flg;

#define REG_FLGS 2

extern const char *reg_flg_str[];

tkn_stat tkn_g_reg_flg(const te *t, const mc *s, uint16_t *flgs);

tkn_stat tkn_g_reg_mtch(const te *t, const mc *s, const alfr *af, mc **m);

// TODO get reg replace
