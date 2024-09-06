
#include "../src/mod.h"
#include "../src/code.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    mod *m = mod_i(a);
    mod_lfile(m, argv[1]);
    m->fns = fn_node_i(a, NULL);
    m->fns->sig = type_node_i(a, TYPE(MOD), NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, a, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            return astat;
        }
    }
    type_stat tstat;
    type_st ts;
    type_st_i(&ts, a);
    if ((tstat = type_chk_fn(&ts, m->fns)) != TYPE_STAT(OK)) return tstat;
    code_st cs;
    code_stat cstat;
    code_st_i(&cs, a, m->src.str);
    m->c = code_i(a, CODE_I_SIZE);
    if ((cstat = code_gen_fn(&cs, m->fns, &m->c)) != CODE_STAT(OK)) {
        code_p(&cs, m->c, 0);
        printf("EC: %d\n", cstat);
        return cstat;
    }
    code_p(&cs, m->c, 0);
    code_f(m->c);
    fn_node_f(m->fns);
    mod_f(m);
    al_f(a);
    return 0;
}
