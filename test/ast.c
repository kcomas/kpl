
#include "../src/mod.h"
#include "../src/ast.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    mod *m = mod_i(a);
    mod_lfile(m, argv[1]);
    m->fns = fn_node_i(a, NULL);
    m->fns->sig = type_node_i(a, TYPE(MOD), NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, m->a, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            return astat;
        }
    }
    fn_node_p(&as, m->fns, 0);
    putchar('\n');
    fn_node_f(m->fns);
    mod_f(m);
    al_f(a);
    return 0;
}
