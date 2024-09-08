
#include "../src/mod.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    er *e = er_i(a);
    mod *m = mod_i(a, e);
    if (mod_lfile(m, argv[1]) != MOD_STAT(OK)) {
        er_p(e);
        return 1;
    }
    mod_psrc(m);
    mod_f(m);
    er_f(e);
    al_f(a);
    return 0;
}
