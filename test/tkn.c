
#include "../src/mod.h"
#include "../src/tkn.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    tds *s = tds_i();
    tdr *r = tds_g(s, false);
    mod *m = mod_i(s, r);
    mod_stat mstat;
    if ((mstat = mod_lfile(m, argv[1])) != MOD_STAT(OK)) {
        er_p(r->e);
        return mstat;
    }
    tkn_st ts;
    tkn_st_i(&ts, r);
    tkn t;
    tkn_stat tstat;
    while ((tstat = tkn_next(&ts, &t, m->src.str)) == TKN_STAT(OK)) {
        tkn_p(&t);
        putchar('\n');
    }
    if (tstat != TKN_STAT(END)) {
        er_p(r->e);
        return 1;
    }
    tkn_p(&t);
    putchar('\n');
    mod_f(m);
    tds_a(s, r);
    tds_f(s);
    return 0;
}
