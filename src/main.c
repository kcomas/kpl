
#include "../src/mod.h"
#include "../src/jit.h"

int mt(void *volatile args) {
    mod *volatile m = (mod*) args;
    m->c->jf(NULL);
    return 0;
}

int run(mod *volatile m, bool repl) {
    ast_stat astat;
    ast_st as;
    ast_st_i(&as, m->r, m->src.str);
    if ((astat = ast_parse_stmts(&as, m->fns, m->fns->body, TFLS, TKN_FLG(NB))) != AST_STAT(OK)) {
        if (astat != AST_STAT(END)) {
            if (!repl) {
                fn_node_p(&as, m->fns, 0);
                putchar('\n');
            }
            er_p(m->r->e);
            return astat;
        }
    }
    type_stat tstat;
    type_st ts;
    type_st_i(&ts, m->s, m->r, m->src.path);
    if ((tstat = type_chk_fn(&ts, m->fns)) != TYPE_STAT(OK)) {
        if (!repl) {
            fn_node_p(&as, m->fns, 0);
            putchar('\n');
        }
        er_p(m->r->e);
        return tstat;
    }
    code_st cs;
    code_stat cstat;
    code_st_i(&cs, m->r);
    m->c = code_i(m->r->a, CODE_I_SIZE);
    if ((cstat = code_gen_fn(&cs, m->fns, &m->c)) != CODE_STAT(OK)) {
        if (!repl) code_p(m->c, 0);
        er_p(m->r->e);
        return cstat;
    }
    fn_stk *stk = fn_stk_i(m->r->a, FN_STK_SIZE);
    fn_stk_b(m->r->a, &stk, m->c);
    fn_stk_a(m->r->a, &stk, m->c);
    jit_i(m->r->a, stk->nops, &m->r->j, 0);
    jit_stat jstat;
    if ((jstat = jit_stk(m, stk, m->r->j, true)) != JIT_STAT(OK)) {
        if (!repl) code_p(m->c, 0);
        er_p(m->r->e);
        return jstat;
    }
    fn_stk_f(stk);
    m->id = clone(&mt, m->r->stkp, CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_IO | CLONE_SIGHAND | SIGCHLD, m);
    //waitpid(m->id, NULL, WEXITED);
    sem_wait(&m->done);
    return 0;
}

#ifndef REPL_BUF_SIZE
    #define REPL_BUF_SIZE 200
#endif

#define KPL_REPL_MSG "KPL 0.0.0\n"

int repl(void) {
    ssize_t rlen = 0;
    tds *volatile s = tds_i();
    tdr *volatile r = tds_g(s, true);
    mod *volatile m = mod_i(s, r);
    char rbuf[REPL_BUF_SIZE];
    m->src.str = rbuf;
    m->fns = fn_node_i(r->a, NULL);
    m->fns->sig = type_node_i(r->a, TYPE(REPL), NULL);
    m->c = code_i(r->a, CODE_I_SIZE);
    mod_ag(m, 255);
    jit_i(r->a, 100, &r->j, 0);
    write(STDOUT_FILENO, KPL_REPL_MSG, strlen(KPL_REPL_MSG));
    for (;;) {
        memset(rbuf, '\0', REPL_BUF_SIZE);
        write(STDOUT_FILENO, " ", 1);
        rlen = read(STDIN_FILENO, rbuf, REPL_BUF_SIZE);
        if (rlen == 0) break;
        run(m, true);
        FNNF(m->fns->body, lst_node_f);
        m->fns->body = lst_node_i(m->r->a, TYPE(STMT));
        m->c->len = 0;
        rlen = 0;
    }
    putchar('\n');
    m->src.str = NULL;
    code_f(m->c);
    fn_node_f(m->fns);
    FNNF(m->tn, type_node_f);
    tds_a(m->s, m->r);
    mod_f(m);
    tds_f(s);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) return repl();
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
    int stat;
    if ((stat = run(m, false)) != 0) return stat;
    code_f(m->c);
    fn_node_f(m->fns);
    FNNF(m->tn, type_node_f);
    tds_a(m->s, m->r);
    mod_f(m);
    tds_f(s);
    return 0;
}
