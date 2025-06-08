
#include "z.h"

static const alfr al_main = { .a = malloc, .f = free };

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("\e[1;33mUsage: %s file.kpl\n\e[0m", argv[0]);
        return 1;
    }
    mc *fn = mc_i_cstr(argv[1], &al_main);
    tbl *et = NULL;
    err *e = z(fn, &et);
    mc_f(fn);
    tbl_f(et);
    if (e) {
        err_p(e);
        err_f(e);
        return 1;
    }
    return 0;
}
