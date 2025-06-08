
#include "../src/mod.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    tds *s = tds_i();
    tdr *r = tds_g(s, false);
    mod *m = mod_i(s, r);
    if (mod_lfile(m, argv[1]) != MOD_STAT(OK)) {
        er_p(r->e);
        return 1;
    }
    mod_psrc(m);
    mod_f(m);
    tds_a(s, r);
    tds_f(s);
    return 0;
}
