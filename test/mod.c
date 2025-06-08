
#include "../src/mod.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    mod *m = mod_i(a);
    mod_lfile(m, argv[1]);
    mod_psrc(m);
    mod_f(m);
    al_f(a);
    return 0;
}
