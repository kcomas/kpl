
#include "../src/mod.h"
#include "../src/tkn.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    mod *m = mod_init();
    mod_lfile(m, argv[1]);
    tkn_st ts;
    tkn_st_init(&ts);
    tkn t;
    while (tkn_next(&ts, &t, m->src.str) == TKN_STAT(OK)) {
        tkn_p(&t, m->src.str);
        putchar('\n');
    }
    mod_free(m);
    return 0;
}
