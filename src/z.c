
#include "z.h"

static const alfr z_al = { .a = malloc, .f = free };

static psr *bp = NULL;

static ast *ba = NULL;

static fld *bf = NULL;

static chk *bc = NULL;

static fld *bo = NULL;

static as *bs = NULL;

static gen_st *bst = NULL;

static gen *bg = NULL;

static atg *bt = NULL;

static size_t p = 0;

static uint8_t *m = NULL;

static tbl *tkn_mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(5, &al_te, NULL);
    return tbl_i(&al_tbl, tkn_sh, c4_eq, tl, b);
}

static tbl *mktbl(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(30, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
}

static ast_stat pig(te *pn, size_t *pid) {
    if (!pn || pn->l < 2) return AST_STAT(INV);
    *pid = pn->d[1].u6;
    return AST_STAT(OK);
}

static tbl *ati(void) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(10, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_mc_sdbm, tbl_mc_eq, tl, b);
}

static lst *ali(void) {
    return lst_i(&al_lst, &al_te, (void*) te_f);
}

static tbl *mktbls(size_t size) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(size, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_no_hsh, tbl_un_eq, tl, b);
}

static tbl *as_arg_tbl(void) {
    return mktbls(5);
}

static tbl *gen_cls_info_tbl(void) {
    return mktbls(GEN_CLS(L) + 1 + X64_TYPE(F6) + 1);
}

#ifndef JIT_M
    #define JIT_M 1e6
#endif

static __attribute__((constructor)) void z_con(void) {
    tkn *t = tkn_b(tkn_i(&z_al, &al_te, &z_al, tkn_mktbl, tkn_df, mc_i(0, &z_al)));
    vr *v = vr_i(10, &z_al, (void*) te_f);
    bp = psr_b(psr_i(&z_al, &al_te, &al_lst, &z_al, psr_entry_f, mktbl, t, v));
    ba = ast_b(ast_i(&z_al, &al_te, &z_al, &z_al, node_err_p, pig, ali, mktbls(NODE_TYPE(_END)), mktbls(TCUST(_END))));
    bf = fld_b(fld_i(&z_al, &al_te, &z_al, ati, ali, NULL, mktbls(AST_CLS(_))));
    bc = chk_b(chk_i(&z_al, &al_te, &z_al, chk_err, mktbl, NULL));
    bo = opt_b(fld_i(&z_al, &al_te, &z_al, ati, ali, NULL, mktbls(AST_CLS(_))));
    bs = as_b(as_i(&z_al, &al_te, &al_lst, &z_al, as_x64_err_g_p, as_arg_tbl, mktbls(AS_X64(_END)), lst_i(&al_lst, &al_te, (void*) te_f)));
    bst = gen_st_i(&z_al, &al_te, mktbls(10), mktbls(10), vr_i(16, &z_al, NULL), vr_i(16, &z_al, NULL));
    bg = gen_b(gen_i(&z_al, &al_te, &z_al, &z_al, gen_cls_info_tbl, mktbls(GEN_OP(_END)), lst_i(&al_lst, &al_te, (void*) te_f)));
    bt = atg_b(atg_i(&z_al, &al_te, &z_al, atg_err, mktbl, lst_i(&al_lst, &al_te, NULL), lst_i(&al_lst, &al_te, (void*) te_f), tbl_i(&al_tbl, type_un_hsh, type_un_eq, lst_i(&al_lst, &al_te, (void*) te_f), te_i(10, &al_te, NULL)),gen_i_gen(bg), as_i_as(bs)));
    m = x64_mmap(JIT_M);
}

static __attribute__((destructor)) void z_des(void) {
    psr_f(bp);
    ast_f(ba);
    fld_f(bf);
    chk_f(bc);
    fld_f(bo);
    atg_f(bt);
    as_f(bs);
    gen_st_f(bst);
    gen_f(bg);
    x64_munmap(JIT_M, m);
}

static void z_e_p(void *d) {
    mc *fn = d;
    printf("\e[1m%s\e[0m", (char*) fn->d);
}

#define APLYLSTS 4 // {}()

err *z(mc *fn, tbl **et, uint32_t dflgs) {
    err *e = NULL;
    mc *pgm = NULL;
    int fd;
    if ((fd = open((char*) fn->d, O_RDONLY)) == -1) return err_i(&z_al, z_e_p, (void*) mc_f, mc_c(fn), __FUNCTION__);
    struct statx sx;
    if (statx(fd, "", AT_EMPTY_PATH, STATX_SIZE, &sx) == -1) {
        close(fd);
        return err_i(&z_al, z_e_p, (void*) mc_f, mc_c(fn), __FUNCTION__);
    }
    pgm = mc_i(sx.stx_size + APLYLSTS + sizeof(char), &z_al);
    pgm->d[0] = '{';
    if (read(fd, pgm->d + 1, sx.stx_size) == -1) {
        close(fd);
        mc_f(pgm);
        return err_i(&z_al, z_e_p, (void*) mc_f, mc_c(fn), __FUNCTION__);
    }
    close(fd);
    pgm->d[sx.stx_size + 1] = '}';
    pgm->d[sx.stx_size + 2] = '(';
    pgm->d[sx.stx_size + 3] = ')';
    psr *zp = psr_i_psr(bp, mc_c(pgm));
    te *nh = te_i(3, zp->ta, psr_n_err_f);
    if (psr_n(zp, nh, &e) != PSR_STAT(END)) {
        te_f(nh);
        psr_f(zp);
        mc_f(pgm);
        return e;
    }
    nh = psr_g_rn(zp, nh);
    if (dflgs & Z_D_FLG(P)) {
        node_p(nh->d[2].p, 0);
        putchar('\n');
    }
    te *an = NULL;
    ast *za = ast_i_ast(ba);
    if (ast_n(za, NULL, nh, (void**) &an, &e) != AST_STAT(OK)) {
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    if (dflgs & Z_D_FLG(A)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld *zf = fld_i_fld(bf, za);
    if (fld_n(zf, &an, &e, true) != FLD_STAT(OK)) {
        fld_f(zf);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    if (dflgs & Z_D_FLG(F)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld_f(zf);
    chk *zc = chk_i_chk(bc, za);
    if (chk_n(zc, an, &e) != CHK_STAT(OK)) {
        chk_f(zc);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    if (dflgs & Z_D_FLG(C)) {
        ast_p(an, 0);
        putchar('\n');
    }
    chk_f(zc);
    fld *zo = fld_i_fld(bo, za);
    if (fld_n(zo, &an, &e, false) != FLD_STAT(OK)) {
        fld_f(zo);
        ast_f(za);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    if (dflgs & Z_D_FLG(O)) {
        ast_p(an, 0);
        putchar('\n');
    }
    fld_f(zo);
    atg *zt = atg_i_atg(bt);
    if (atg_q(zt, &an, atg_x64_enq) != ATG_STAT(OK)) {
        ast_f(za);
        atg_f(zt);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    while (zt->q->l > 0) {
        gen *zg = NULL;
        if (atg_n(zt, &zg, za, &e) != ATG_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            te_f(an);
            mc_f(pgm);
            return e;
        }
        gen_st *zst = gen_st_i_gen_st(bst);
        if (gen_st_p1(zg, zst) != GEN_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            gen_st_f(zst);
            te_f(an);
            mc_f(pgm);
            return e;
        }
        gen_x64_opt(zg, zst);
        if (dflgs & Z_D_FLG(G)) {
            gen_p(zg, NULL);
            putchar('\n');
        }
        if (gen_n(zg, zst, zt->a, &e) != GEN_STAT(OK)) {
            ast_f(za);
            atg_f(zt);
            gen_f(zg);
            gen_st_f(zst);
            te_f(an);
            mc_f(pgm);
            return e;
        }
        gen_st_f(zst);
        gen_f(zg);
    }
    ast_f(za);
    if (as_n(zt->a, &p, m, &e) != AS_STAT(OK)) {
        atg_f(zt);
        te_f(an);
        mc_f(pgm);
        return e;
    }
    if (dflgs & Z_D_FLG(S)) {
        as_code_p(zt->a, m);
        putchar('\n');
    }
    ssize_t ep = as_lbl_g_c_i(zt->a, ((te*) an->d[4].p)->d[4].u5);
    if (ep < 0) return err_i(&z_al, NULL, NULL, NULL, __FUNCTION__);
    if (an->d[3].p) *et = tbl_c(an->d[3].p);
    te_f(an);
    e = atg_z(zt, *et, m, ep);
    atg_f(zt);
    mc_f(pgm);
    return e;
}
