
#include "../src/mod.h"
#include "../src/type.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    mod *m = mod_i();
    mod_lfile(m, argv[1]);
    m->fns = fn_node_i(NULL);
    m->fns->sig = type_node_i(TYPE(MOD), NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            return astat;
        }
    }
    type_stat tstat;
    if ((tstat = type_chk_fn(m->fns)) != TYPE_STAT(OK)) {
        fn_node_p(&as, m->fns, 0);
        printf("\nEC: %d\n", tstat);
        return tstat;
    }
    fn_node_p(&as, m->fns, 0);
    putchar('\n');
    fn_node_f(m->fns);
    mod_f(m);
    return 0;
}
