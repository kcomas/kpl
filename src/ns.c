
#include "ns.h"

static tbl* ns = NULL;

#define NS_TBL_S 10

static tbl *ntbl(size_t n) {
    lst *tl = lst_i(&al_lst, &al_te, (void*) te_f);
    te *b = te_i(n, &al_te, NULL);
    return tbl_i(&al_tbl, tbl_mc_sdbm, tbl_mc_eq, tl, b);
}

void ns_f_b();
void ns_s_b();

static __attribute__((constructor)) void ns_con(void) {
    ns = ntbl(NS_TBL_S);
    ns_f_b();
    ns_s_b();
}

static __attribute__((destructor)) void ns_des(void) {
    tbl_f(ns);
}

te *ns_type_s(type t) {
   if (type_g_c(t) != TYPE_CLS(S)) STOP("inv ns type cls s");
   return type_s_i(&al_te, NULL, t);
}

te *ns_type_v(type t, te *vt) {
   if (type_g_c(t) != TYPE_CLS(V)) STOP("inv ns type cls v");
   return type_v_i(&al_te, NULL, t, vt);
}

te *ns_type_h(type t, size_t n, ...) {
   if (type_g_c(t) != TYPE_CLS(H)) STOP("inv ns type cls h");
   va_list args;
   tbl *tt = ntbl(n + 1);
   va_start(args, n);
   while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &al_mc);
        te *at = va_arg(args, te*);
        type_tbl_a(tt, &al_te, s, 0, at);
        n--;
   }
   lst_s(tt->i, type_h_cmp);
   va_end(args);
   return type_h_i(&al_te, NULL, t, tt);
}

te *ns_type_fp(te *rt, size_t n, ...) {
    uint32_t ic = 0, fc = 0;
    va_list args;
    tbl *tt = n ? ntbl(n + 1) : NULL;
    va_start(args, n);
    while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &al_mc);
        te *at = va_arg(args, te*);
        switch (at->d[1].u4) {
            case TYPE(F5):
            case TYPE(F6):
                type_tbl_a(tt, &al_te, s, fc++, at);
                break;
            default:
                type_tbl_a(tt, &al_te, s, ic++, at);
                break;
        }
        n--;
    }
    va_end(args);
    return type_f_i(&al_te, NULL, TYPE(FP), rt, tt, NULL);
}

te *ns_un(size_t k, un v, frfn des) {
    te *u = te_i(2, &al_te, des);
    u->d[0] = U6(k);
    u->d[1] = v;
    return u;
}

static void ns_entry_f(void *p) {
    te *t = p;
    mc_f(t->d[0].p);
    te_f(t->d[1].p);
    tbl_f(t->d[3].p);
    t->af->f(t);
}

void ns_a(te *type, void *any, size_t n, ...) {
    va_list args;
    te *kv = NULL;
    tbl *t = ns;
    va_start(args, n);
    while (n > 0) {
        mc *s = mc_i_cstr(va_arg(args, char*), &al_mc);
        if (tbl_g_i(t, P(s), &kv) == TBL_STAT(NF)) {
            kv = te_i(4, &al_te, ns_entry_f);
            kv->d[0] = P(s);
            kv->d[3] = P(ntbl(NS_TBL_S));
            tbl_a(t, kv);
        } else mc_f(s);
        t = kv->d[3].p;
        n--;
    }
    va_end(args);
    if (!kv) STOP("inv ns_a");
    kv->d[1] = P(type);
    kv->d[2] = P(any);
}

te *ns_n(size_t n, mc *m[NS_MAX_L]) {
    if (n >= NS_MAX_L) return NULL;
    te *kv = NULL;
    tbl *t = ns;
    for (size_t i = 0; i < n; i++) {
        if (tbl_g_i(t, P(m[i]), &kv) != TBL_STAT(OK)) return NULL;
        t = kv->d[3].p;
    }
    return kv;
}

static void _ns_p(tbl *t, size_t idnt) {
    te *h = t->i->h, *nte;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        nte = h->d[0].p;
        printf("%s ", (char*) ((mc*) nte->d[0].p)->d);
        if (nte->d[1].p) type_p(nte->d[1].p);
        putchar('\n');
        if (nte->d[3].p) _ns_p(nte->d[3].p, idnt + 1);
        h = h->d[2].p;
    }
}

void ns_un_er_sg_f(void *p) {
    te *u = p;
    if (!u->d[0].u6) err_f(u->d[1].p);
    else mc_f(u->d[1].p);
    u->af->f(u);
}

void ns_un_er_vr_f(void *p) {
    te *u = p;
    if (!u->d[0].u6) err_f(u->d[1].p);
    else vr_f(u->d[1].p);
    u->af->f(u);
}

static void ns_err_p(void *d, uint32_t idnt) {
    mc *s = d;
    printf("%*s\e[1m%s\e[0m", idnt, "", (char*) s->d);
}

te *ns_err(mc *s, const char *msg, frfn des) {
    return ns_un(0, P(err_i(&al_err, ns_err_p, (void*) mc_f, mc_c(s), msg)), des);
}

void ns_p(void) {
    _ns_p(ns, 0);
}
