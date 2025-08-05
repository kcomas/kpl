
#include "atg_t.h"

atg *batg = NULL;

void atg_t_i(void) {
    batg = atg_b(atg_i(&ast_am, &al_te, &al_err, atg_err, cti, lst_i(&al_lst, &al_te, NULL), ali(), tbl_i(&al_tbl, type_un_hsh, type_un_eq, lst_i(&al_lst, &al_te, (void*) te_f), te_i(5, &al_te, NULL)), gen_i_gen(bg), as_i_as(ba)));
    p = 0; // do not reset per test, need reset here
}

void atg_t_f(void) {
    atg_f(batg);
    batg = NULL;
}

void atg_des_verify(_tests *_t, atg *t, te *restrict tn, const void *fg, gen_st *st) {
    E();
    te *h;
    if (tbl_g_i(t->dt, P(tn), &h) != TBL_STAT(OK)) {
        type_p(tn);
        putchar('\n');
        A(0, "des type nf");
    }
    type_p(h->d[0].p);
    putchar('\n');
    err *e = NULL;
    gen *g = NULL;
    gen_st *sc = NULL;
    void *fn = NULL;
    atg_stat stat = atg_d_n(t, h, &g, &fn, gen_type_des, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(stat == ATG_STAT(OK), "atg_d_n");
    if (fn) {
        A(fn == fg, "fn ptr neq");
        h->d[1] = P(fn);
        return;
    }
    if (g) {
        sc = gen_st_i_gen_st(st);
        A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
        gen_x64_opt(g, sc);
    }
    const gen *gc = fg;
    gen_p(g, NULL);
    bool eq = gen_code_eq(g, gc);
    if (!eq) {
        printf("--- DIFF ---\n");
        if (gc) gen_p(gc, NULL);
    }
    A(eq, "des_eq");
    if (g) {
        size_t pc = p;
        gen_stat gstat = gen_n(g, sc, t->a, &e);
        if (e) {
            err_p(e, true);
            err_f(e);
        }
        A(gstat == GEN_STAT(OK), "gen_n");
        UNLOCK();
        as_stat astat = as_n(t->a, &p, m, &e);
        LOCK();
        if (e) {
            err_p(e, true);
            err_f(e);
        }
        A(astat == AS_STAT(OK), "as_n");
        h->d[1] = P(&m[pc]);
        as_f(t->a);
        t->a = as_i_as(ba);
    }
    gen_st_f(sc);
    gen_f(g);
}

void atg_verify(_tests *_t, atg *t, ast *a, te *restrict an, te *restrict tn, const gen *gc, gen_st *st) {
    E();
    err *e = NULL;
    gen *g = NULL;
    atg_stat astat = atg_n(t, &g, a, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(astat == ATG_STAT(OK), "atg_n");
    ast_p(an, 0);
    bool eq = ast_eq(an, tn);
    if (!eq) {
        printf("\n--- DIFF ---\n");
        ast_p(tn, 0);
    }
    putchar('\n');
    A(eq, "ast_eq");
    gen_st *sc = gen_st_i_gen_st(st);
    A(gen_st_p1(g, sc) == GEN_STAT(OK), "gen_st_p1");
    gen_x64_opt(g, sc);
    gen_p(g, NULL);
    eq = gen_code_eq(g, gc);
    if (!eq) {
        printf("--- DIFF ---\n");
        if (gc) gen_p(gc, NULL);
    }
    A(eq, "gen_code_eq");
    gen_stat gstat = gen_n(g, sc, t->a, &e);
    if (e) {
        err_p(e, true);
        err_f(e);
    }
    A(gstat == GEN_STAT(OK), "gen_n");
    gen_st_f(sc);
    gen_f(g);
}

void atg_run(_tests *_t, atg *t, te *an, uint32_t elcmp, err **e) {
    E();
    UNLOCK();
    as_stat stat = as_n(t->a, &p, m, e);
    LOCK();
    if (*e) err_p(*e, true);
    A(stat == AS_STAT(OK), "as_n");
    as_code_p(t->a, m);
    uint32_t el = ((te*) an->d[4].p)->d[4].u5;
    A(el == elcmp, "el");
    ssize_t ep = as_lbl_g_c_i(t->a, el);
    A(ep > -1, "ep");
    *e = atg_y(t, an->d[3].p, m, ep);
}
