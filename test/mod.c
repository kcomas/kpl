
#include "../src/mod.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    mod *m = mod_i();
    mod_lfile(m, argv[1]);
    mod_psrc(m);
    mod_f(m);
    return 0;
}
