
#include "../chk.h"

#define REGX_ARG_S 2

static chk_stat regx(chk *c, te *an, err **e) {
    tbl *fa;
    ast_regx_s_i(an, c->fnlc--);
    switch (ast_regx_g_mode(an)) {
        case REGX_MODE(T):
            fa = tbl_i(c->tbla, tbl_mc_sdbm, tbl_mc_eq, lst_i(c->la, c->ta, (void*) te_f), te_i(REGX_ARG_S, c->ta, NULL));
            type_tbl_a(fa, c->ta, mc_i_cstr("s", c->a->ma), 0, type_s_i(c->ta, NULL, TYPE(SG)));
            an->d[3] = P(type_f_i(c->ta, NULL, TYPE(FN), type_s_i(c->ta, NULL, TYPE(BL)), fa, NULL));
            break;
        default:
            return chk_err(c, an, e, "chk inv reg mode");
    }
    size_t ss = 0, se = 0, i = 0;
    un sc;
    mc *mtch = an->d[5].p, *rplc = an->d[6].p;
    if (mtch) {
        for (;;) {
            sc = c4_g((char*) mtch->d, ss, &se);
            switch (sc.c.a) {
                case '\0':
                    goto mtche;
                case '^':
                    if (i != 0) return chk_err(c, an, e, "chk regx inv ^");
                    break;
                case '$':
                    ss = se + 1;
                    i++;
                    sc = c4_g((char*) mtch->d, ss, &se);
                    if (sc.c.a != '\0') return chk_err(c, an, e, "chk regex inv $");
                    goto mtche;
                default:
                    break;
            }
            ss = se + 1;
            i++;
        }
    }
    mtche:
    if (rplc) {
        STOP("REGEX REPLACE");
    }
    return CHK_STAT(OK);
}

void chk_regx(chk *c) {
    CHK_AA(c, regx, AST_CLS(X), TYPE(_N), AST_CLS(_), TYPE(_A));
}
