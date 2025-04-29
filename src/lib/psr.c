
#include "psr.h"

psr *psr_i(const alfr *af, const alfr *ta, const alfr *la, const alfr *ea, frfn pef, psr_tbl_i pti, tkn *tt, vr *ts) {
    psr *p = af->a(sizeof(psr));
    p->r = 1;
    p->idc = PARSER(_);
    p->af = af;
    p->ta = ta;
    p->la = la;
    p->ea = ea;
    p->pef = pef;
    p->pti = pti;
    p->tt = tt;
    p->ts = ts;
    p->pt = pti();
    return p;
}

psr *psr_i_psr(const psr *p, mc *s) {
    psr *pp = p->af->a(sizeof(psr));
    pp->r = 1;
    pp->idc = PARSER(_);
    pp->af = p->af;
    pp->ta = p->ta;
    pp->la = p->la;
    pp->ea = p->ea;
    pp->pef = p->pef;
    pp->pti = p->pti;
    pp->tt = tkn_i_tkn(p->tt, s);
    pp->ts = vr_i_vr(p->ts);
    pp->pt = tbl_c(p->pt);
    return pp;
}

psr *psr_c(psr *p) {
    p->r++;
    return p;
}

size_t psr_a(psr *p, size_t pid, size_t mode, te *st, psr_each_fn *ef, psr_megre_fn *mf, psr_node_fn *nf, size_t nt, ...) {
    tbl *pt = p->pt;
    te *kv;
    size_t nid = 0;
    va_list tkns;
    va_start(tkns, nt);
    while (nt > 0) {
        uint16_t tid = va_arg(tkns, int);
        if (tbl_g_i(pt, U4(tid), &kv) == TBL_STAT(NF)) {
            while (nt > 0) {
                kv = te_i(8, p->ta, p->pef);
                kv->d[0] = U4(tid);
                kv->d[7] = P(p->pti());
                tbl_a(pt, kv);
                pt = kv->d[7].p;
                tid = va_arg(tkns, int);
                nt--;
            }
            kv->d[1] = pid ? U6(pid) : U6(p->idc++);
            kv->d[2] = U6(mode);
            kv->d[3] = P(st);
            kv->d[4] = P(ef);
            kv->d[5] = P(mf);
            kv->d[6] = P(nf);
            va_end(tkns);
            return pid;
        }
        nid = kv->d[1].u6;
        pt = kv->d[7].p;
        nt--;
    }
    va_end(tkns);
    return nid;
}

static void psr_err_p(void *d) {
    psr *p = d;
    printf("lno:%u,cno:%u\n", p->tt->lno, p->tt->cno);
}

psr_stat psr_err(psr *p, err **e, const char *m) {
    if (!*e) *e = err_i(p->ea, psr_err_p, (void*) psr_f, psr_c(p), m);
    return PSR_STAT(INV);
}

psr_stat psr_n(psr *p, te *nh, err **e) {
    psr_stat pstat;
    te *m = te_i(2, p->ta, NULL);
    for (;;) {
        if ((p->tstat = tkn_n(p->tt, m, e)) != TKN_STAT(OK)) break;
        psr_mode pm = PSR_MODE(NONE);
        te *st = NULL;
        psr_each_fn *ef = NULL;
        psr_megre_fn *mf = NULL;
        psr_node_fn *nf = NULL;
        tbl *pt = p->pt;
        te *kv = NULL, *pn = NULL;
        while (tbl_g_i(pt, U4(tkn_m_g_i(m)), &kv) == TBL_STAT(OK)) {
            vr_ab(&p->ts, P(m));
            pm = kv->d[2].u6;
            st = kv->d[3].p;
            ef = kv->d[4].p;
            mf = kv->d[5].p;
            nf = kv->d[6].p;
            pt = kv->d[7].p;
            m = te_i(2, p->ta, NULL);
            if ((p->tstat = tkn_n(p->tt, m, e)) != TKN_STAT(OK)) break;
        }
        tkn_s(p->tt, m);
        if (pm == PSR_MODE(NONE)) return psr_err(p, e, "psr mode node");
        else if (pm == PSR_MODE(STOP)) {
            te_f(m);
            return PSR_STAT(END);
        } else if (pm == PSR_MODE(LOOP)) {
            if (!st || !ef || !nf || !mf) return psr_err(p, e, "psr fns NULL");
            if ((pstat = nf(p, &pn, e)) != PSR_STAT(OK)) {
                te_f(m);
                te_f(pn);
                return pstat;
            }
            vr_d(p->ts);
            te *lnh = te_i(3, p->ta, NULL);
            for (;;) {
                lnh->d[0] = P(NULL);
                lnh->d[1] = P(NULL);
                lnh->d[2] = P(NULL);
                if ((pstat = psr_n(p, lnh, e)) != PSR_STAT(END)) {
                    te_f(m);
                    te_f(pn);
                    if (!lnh->d[0].p && !lnh->d[2].p) te_f(lnh->d[1].p);
                    else {
                        if (lnh->d[0].p == lnh->d[2].p) lnh->d[2] = P(NULL);
                        te_f(lnh->d[0].p);
                        te_f(lnh->d[2].p);
                    }
                    te_f(lnh);
                    return pstat;
                }
                te *ln = lnh->d[0].p ? lnh->d[0].p : lnh->d[2].p;
                if (ln && (pstat = ef(p, pn, ln, e)) != PSR_STAT(OK)) {
                    te_f(m);
                    te_f(pn);
                    te_f(ln);
                    te_f(lnh);
                    return pstat;
                }
                for (size_t i = 0; i < st->l; i++) if (p->ts->l > 0 && tkn_m_g_i(p->ts->d[p->ts->l - 1].p) == st->d[i].u4) goto el;
                if (p->tstat != TKN_STAT(OK)) {
                    te_f(m);
                    te_f(pn);
                    te_f(lnh);
                    return psr_err(p, e, "psr tkn invalid");
                }
                vr_d(p->ts);
            }
            el:
            te_f(lnh);
            vr_d(p->ts);
            te_f(m);
            if ((pstat = mf(p, nh, pn, e)) != PSR_STAT(OK)) {
                te_f(pn);
                return pstat;
            }
            m = te_i(2, p->ta, NULL);
        } else if (nf && mf) {
            if ((pstat = nf(p, &pn, e)) != PSR_STAT(OK)) {
                te_f(m);
                te_f(pn);
                return pstat;
            }
            if (pn && (pstat = mf(p, nh, pn, e)) != PSR_STAT(OK)) {
                te_f(m);
                te_f(pn);
                return pstat;
            }
        }
        vr_d(p->ts);
    }
    te_f(m);
    return p->tstat == TKN_STAT(END) ? PSR_STAT(END) : PSR_STAT(INV);
}

void psr_f(psr *p) {
    if (!p || --p->r > 0) return;
    tkn_f(p->tt);
    vr_f(p->ts);
    tbl_f(p->pt);
    p->af->f(p);
}
