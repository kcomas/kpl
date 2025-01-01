
#include "psr_t.h"

static void psr_p(tbl *tl, size_t idnt) {
    te *h = tl->i->h;
    while (h) {
        te *kv = h->d[0].p;
        printf("%lu\n", kv->d[1].u6);
        h = h->d[2].p;
    }
}

void btest(void) {
    const char *pgm = "0 Σ [12;44;67]";
    tkn *t = tkn_i(&malloc, &free, &tkn_entry_free, &tkn_mktbl, &tkn_df, mc_i_cstr(pgm, &malloc, &free));
    tkn_standard(t);
    //tkn_a(t, TOKEN(SEMI), ";", &tkn_ft);
    //tkn_a(t, TOKEN(UN), "Σ", &tkn_ft);
    //tkn_a(t, TOKEN(UN), "[", &tkn_ft);
    //tkn_a(t, TOKEN(UN), "]", &tkn_ft);
    vr *v = vr_i(5, &malloc, &tkn_entry_free, &free);
    psr *p = psr_i(&malloc, &free, &psr_ef, &psr_mktbl, &psr_df, t, v);
    psr_a(p, PARSER(UN), &psr_ign, 1, TCUST(WS));
    psr_a(p, PCUST(INT), &psr_val, 1, TCUST(NUM));
    psr_p(p->pt, 0);
    psr_f(p);
}

int main(void) {
    btest();
    return 0;
}
