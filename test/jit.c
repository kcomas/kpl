
#include "../src/mod.h"
#include "../src/jit.h"

int mt(void *volatile args) {
    mod *volatile m = (mod*) args;
    m->c->jf(NULL);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    tds *volatile s = tds_i();
    tdr *volatile r = tds_g(s, true);
    mod *volatile m = mod_i(s, r);
    mod_stat mstat;
    if ((mstat = mod_lfile(m, argv[1])) != MOD_STAT(OK)) {
        er_p(r->e);
        return mstat;
    }
    m->fns = fn_node_i(r->a, NULL);
    m->fns->sig = type_node_i(r->a, TYPE(MOD), NULL);
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, r, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            fn_node_p(&as, m->fns, 0);
            putchar('\n');
            er_p(r->e);
            return astat;
        }
    }
    type_stat tstat;
    type_st ts;
    type_st_i(&ts, s, r, m->src.path);
    if ((tstat = type_chk_fn(&ts, m->fns)) != TYPE_STAT(OK)) {
        fn_node_p(&as, m->fns, 0);
        putchar('\n');
        er_p(r->e);
        return tstat;
    }
    code_st cs;
    code_stat cstat;
    code_st_i(&cs, r);
    m->c = code_i(r->a, CODE_I_SIZE);
    if ((cstat = code_gen_fn(&cs, m->fns, &m->c)) != CODE_STAT(OK)) {
        code_p(m->c, 0);
        er_p(r->e);
        return cstat;
    }
    fn_stk *stk = fn_stk_i(r->a, FN_STK_SIZE);
    fn_stk_b(r->a, &stk, m->c);
    fn_stk_a(r->a, &stk, m->c);
    jit_i(r->a, stk->nops, &r->j, 0);
    jit_stat jstat;
    if ((jstat = jit_stk(m, stk, r->j)) != JIT_STAT(OK)) {
        code_p(m->c, 0);
        er_p(r->e);
        return jstat;
    }
    fn_stk_f(stk);
    code_p(m->c, 0);
    m->c->jf(NULL);
    code_f(m->c);
    fn_node_f(m->fns);
    FNNF(m->tn, type_node_f);
    tds_a(m->s, m->r);
    mod_f(m);
    tds_f(s);
    return 0;
}
