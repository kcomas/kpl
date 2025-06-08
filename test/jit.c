
#include "../src/mod.h"
#include "../src/jit.h"

int main(int argc, char *argv[]) {
    if (argc != 2) return 1;
    al *a = al_i();
    mod *m = mod_i(a);
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
    code_st cs;
    code_stat cstat;
    code_st_i(&cs, m->src.str);
    m->c = code_i(CODE_I_SIZE);
    if ((cstat = code_gen_fn(&cs, m->fns, &m->c)) != CODE_STAT(OK)) return cstat;
    fn_stk *stk = fn_stk_i(FN_STK_SIZE);
    fn_stk_b(&stk, m->c);
    fn_stk_a(&stk, m->c);
    m->j = jit_i(stk->nops);
    jit_stat jstat;
    if ((jstat = jit_stk(m, stk, m->j)) != JIT_STAT(OK)) {
        code_p(&cs, m->c, 0);
        printf("EC: %d\n", jstat);
        return jstat;
    }
    code_p(&cs, m->c, 0);
    m->c->jf();
    jit_f(m->j);
    fn_stk_f(stk);
    code_f(m->c);
    fn_node_f(m->fns);
    mod_f(m);
    al_f(a);
    return 0;
}
