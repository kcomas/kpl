
#include "../src/mod.h"
#include "../src/code.h"

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
    if ((tstat = type_chk_fn(m->fns)) != TYPE_STAT(OK)) return tstat;
    tkn t = {TKN_TYPE(NB), 0, 0, 0, 0};
    m->afns = ast_i(AST_TYPE(FN), (node) { .fn = m->fns }, &t);
    ast_f(m->afns);
    mod_f(m);
    return 0;
}
