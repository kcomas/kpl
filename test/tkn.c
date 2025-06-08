
#include "../src/mod.h"
#include "../src/tkn.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    er *e = er_i(a);
    mod *m = mod_i(a, e);
    mod_lfile(m, argv[1]);
    tkn_st ts;
    tkn_st_i(&ts, a, e);
    tkn t;
    while (tkn_next(&ts, &t, m->src.str) == TKN_STAT(OK)) {
        tkn_p(&t, m->src.str);
        putchar('\n');
    }
    tkn_p(&t, m->src.str);
    putchar('\n');
    mod_f(m);
    er_f(e);
    al_f(a);
    return 0;
}
