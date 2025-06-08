
#include "type.h"

const char *type_str(type t) {
    static const char *ts[] = {
        "_START",
        "_S",
        "_N",
        "_A",
        "_L",
        "_G",
        "VD",
        "BL",
        "I3",
        "I4",
        "I5",
        "I6",
        "U3",
        "U4",
        "U5",
        "U6",
        "F5",
        "F6",
        "SL",
        "C4",
        "SG",
        "_V",
        "VR",
        "LT",
        "MC",
        "_H",
        "UN",
        "HH",
        "ST",
        "_F",
        "FN",
        "NF",
        "_C",
        "TE",
        "BA",
        "TD",
        "CJ",
        "_END"
    };
    const char *s = "INV TYPE";
    if (t > TYPE(_START) && t < TYPE(_END)) s = ts[t];
    return s;
}

type_cls type_g_c(type t) {
    if (t > TYPE(_S) && t < TYPE(_V)) return TYPE_CLS(S);
    if (t > TYPE(_V) && t < TYPE(_H)) return TYPE_CLS(V);
    if (t > TYPE(_H) && t < TYPE(_F)) return TYPE_CLS(H);
    if (t > TYPE(_F) && t < TYPE(_C)) return TYPE_CLS(F);
    if (t > TYPE(_C) && t < TYPE(_END)) return TYPE_CLS(C);
    return TYPE_CLS(_);
}

static void type_tbl_e_f(void *p) {
    te *t = p;
    mc_f(t->d[0].p);
    te_f(t->d[2].p);
    t->af->f(t);
}

void type_tbl_a(tbl *t, const alfr *af, mc *s, uint32_t id, te *type) {
    te *kv = te_i(3, af, type_tbl_e_f);
    kv->d[0] = P(s);
    kv->d[1] = U5(id);
    kv->d[2] = P(type);
    tbl_a(t, kv);
}

te *type_s_i(const alfr *af, te *p, type t) {
    te *s = te_i(2, af, NULL);
    s->d[0] = P(p);
    s->d[1] = U4(t);
    return s;
}

static void type_v_f(void *p) {
    te *t = p;
    te_f(t->d[2].p);
    t->af->f(t);
}

te *type_v_i(const alfr *af, te *restrict p, type v, te *restrict t) {
    te *vv = te_i(3, af, type_v_f);
    vv->d[0] = P(p);
    vv->d[1] = U4(v);
    vv->d[2] = P(t);
    return vv;
}

static void type_h_f(void *p) {
    te *t = p;
    tbl_f(t->d[2].p);
    t->af->f(t);
}

te *type_h_i(const alfr *af, te *restrict p, type h, tbl *restrict t) {
    te *hh = te_i(3, af, type_h_f);
    hh->d[0] = P(p);
    hh->d[1] = U4(h);
    hh->d[2] = P(t);
    return hh;
}

static void type_f_f(void *p) {
    te *t = p;
    te_f(t->d[2].p);
    tbl_f(t->d[3].p);
    tbl_f(t->d[4].p);
    t->af->f(t);
}

te *type_f_i(const alfr *af, te *restrict p, type f, te *restrict r, tbl *restrict a, tbl *restrict s) {
    te *ff = te_i(5, af, type_f_f);
    ff->d[0] = P(p);
    ff->d[1] = U4(f);
    ff->d[2] = P(r);
    ff->d[3] = P(a);
    ff->d[4] = P(s);
    return ff;
}

te *type_i(const alfr *af, te *restrict p, type t) {
    switch (type_g_c(t)) {
        case TYPE_CLS(S):
            return type_s_i(af, p, t);
        case TYPE_CLS(V):
            return type_v_i(af, p, t, NULL);
        case TYPE_CLS(H):
            return type_h_i(af, p, t, NULL);
        case TYPE_CLS(F):
            return type_f_i(af, p, t, NULL, NULL, NULL);
        case TYPE_CLS(C):
            STOP("COLLECTION TYPES MUST BE INVOKED DIRECTLY");
            break;
        default:
            break;
    }
    return NULL;
}

static void type_te_f(void *p) {
    te *t = p;
    for (size_t i = 2; i < t->l; i++) te_f(t->d[i].p);
    t->af->f(t);
}

te *type_te_i(const alfr *af, te *restrict p, size_t l) {
    te *t = te_i(l + 2, af, type_te_f);
    t->d[0] = P(p);
    t->d[1] = U4(TYPE(TE));
    return t;
}

te *type_te_i_v(const alfr *af, te *restrict p, size_t l, ...) {
    va_list args;
    te *t = type_te_i(af, p, l), *a;
    va_start(args, l);
    for (size_t i = 0; i < l; i++) {
        a = te_c(va_arg(args, void*));
        a->d[0] = P(t);
        t->d[2 + i] = P(a);
    }
    va_end(args);
    return t;
}

te *type_te_g_t(const te *t, size_t n) {
    n += 2;
    if (n < t->l) return t->d[n].p;
    return NULL;
}

static void type_tbl_p(const tbl *t) {
    if (!t) {
        printf("``");
        return;
    }
    te *h = t->i->h;
    while (h) {
        te *li = h->d[0].p;
        type_p(li->d[2].p);
        printf("`%s[%lu]", (char*) ((mc*) li->d[0].p)->d, li->d[1].u6);
        h = h->d[2].p;
        if (h) putchar(' ');
    }
}

void type_p(const te *t) {
    if (!t) {
        printf("??");
        return;
    }
    type_cls cls = type_g_c(t->d[1].u4);
    switch (cls) {
        case TYPE_CLS(S):
            printf("%s", type_str(t->d[1].u4));
            break;
        case TYPE_CLS(V):
            printf("%s(", type_str(t->d[1].u4));
            type_p(t->d[2].p);
            putchar(')');
            break;
        case TYPE_CLS(H):
            // TODO
            break;
        case TYPE_CLS(F):
            printf("%s(", type_str(t->d[1].u4));
            type_tbl_p(t->d[3].p);
            if (t->d[4].p) {
                printf(" {");
                type_tbl_p(t->d[4].p);
                printf("} ");
            } else putchar(' ');
            type_p(t->d[2].p);
            putchar(')');
            break;
        case TYPE_CLS(C):
            switch (t->d[1].u4) {
                case TYPE(TE):
                    printf("%s(", type_str(t->d[1].u4));
                    for (size_t i = 2; i < t->l; i++) {
                        type_p(t->d[i].p);
                        if (i < t->l - 1) putchar(';');
                    }
                    putchar(')');
                    break;
                default:
                    printf("INV _C");
                    return;
            }
            break;
        default:
            printf("TINV");
            break;
    }
}

static bool type_tbl_eq(const tbl *restrict a, const tbl *restrict b) {
    if (!a && !b) return true;
    if (!a || !b || a->i->l != b->i->l) return false;
    te *ah = a->i->h, *bh = b->i->h;
    while (ah && bh) {
        te *ae = ah->d[0].p, *be = bh->d[0].p;
        if (!mc_eq(ae->d[0].p, be->d[0].p) || ae->d[1].u6 != be->d[1].u6 || !type_eq(ae->d[2].p, be->d[2].p)) return false;
        ah = ah->d[2].p;
        bh = bh->d[2].p;
    }
    return true;
}

bool type_eq(const te *restrict a, const te *restrict b) {
    if (!a && !b) return true;
    if (!a || !b || a->d[1].u4 != b->d[1].u4 || a->l != b->l) return false;
    type_cls cls = type_g_c(a->d[1].u4);
    switch (cls) {
        case TYPE_CLS(S):
            return true;
        case TYPE_CLS(V):
            return type_eq(a->d[2].p, b->d[2].p);
        case TYPE_CLS(H):
            // TODO
            break;
        case TYPE_CLS(F):
            return type_eq(a->d[2].p, b->d[2].p) && type_tbl_eq(a->d[3].p, b->d[3].p) && type_tbl_eq(a->d[4].p, b->d[4].p);
        case TYPE_CLS(C):
            switch (a->d[1].u4) {
                case TYPE(TE):
                    for (size_t i = 2; i < a->l; i++) if (!type_eq(a->d[i].p, b->d[i].p)) return false;
                    return true;
                default:
                    return false;
            }
            break;
        default:
            break;
    }
    return false;
}

bool type_ic(const te *restrict a, const te *restrict b) {
    te *pn = a->d[0].p;
    while (pn) {
        if (b == pn) return true;
        pn = pn->d[0].p;
    }
    return false;
}
