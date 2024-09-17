
#include "../src/mod.h"
#include "../src/jit.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *volatile a = al_i();
    er *volatile e = er_i(a);
    mod *volatile m = mod_i(a, e);
    mod_stat mstat;
    if ((mstat = mod_lfile(m, argv[1])) != MOD_STAT(OK)) {
        er_p(e);
        return mstat;
    }
    m->fns = fn_node_i(a, NULL);
    m->fns->sig = type_node_i(a, TYPE(MOD), NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, a, e, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            fn_node_p(&as, m->fns, 0);
            putchar('\n');
            er_p(e);
            return astat;
        }
    }
    type_stat tstat;
    type_st ts;
    type_st_i(&ts, a, e);
    if ((tstat = type_chk_fn(&ts, m->fns)) != TYPE_STAT(OK)) {
        fn_node_p(&as, m->fns, 0);
        putchar('\n');
        er_p(e);
        return tstat;
    }
    code_st cs;
    code_stat cstat;
    code_st_i(&cs, a, e, m->src.str);
    m->c = code_i(a, CODE_I_SIZE);
    if ((cstat = code_gen_fn(&cs, m->fns, &m->c)) != CODE_STAT(OK)) {
        code_p(&cs, m->c, 0);
        er_p(e);
        return cstat;
    }
    fn_stk *stk = fn_stk_i(a, FN_STK_SIZE);
    fn_stk_b(a, &stk, m->c);
    fn_stk_a(a, &stk, m->c);
    m->j = jit_i(a, stk->nops);
    jit_stat jstat;
    if ((jstat = jit_stk(m, stk, m->j)) != JIT_STAT(OK)) {
        code_p(&cs, m->c, 0);
        er_p(e);
        return jstat;
    }
    code_p(&cs, m->c, 0);
    m->c->jf();
    jit_f(m->j);
    fn_stk_f(stk);
    code_f(m->c);
    fn_node_f(m->fns);
    mod_f(m);
    er_f(e);
    al_f(a);
    return 0;
}
