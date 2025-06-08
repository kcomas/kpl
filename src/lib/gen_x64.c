
#include "gen_x64.h"

const char *gen_op_str(gen_op go) {
    static const char *gos[] = {
        "_START",
        "ADD",
        "RET",
        "_END"
    };
    const char *s = "INV";
    if (go > GEN_OP(_START) && go < GEN_OP(_END)) s = gos[go];
    return s;
}

const char *x64_type_str(x64_type xt) {
    switch (xt) {
        case X64_TYPE(N): return "N";
        case X64_TYPE(M): return "M";
        case X64_TYPE(U3): return "U3";
        case X64_TYPE(U4): return "U4";
        case X64_TYPE(U5): return "U5";
        case X64_TYPE(U6): return "U6";
        case X64_TYPE(I3): return "I3";
        case X64_TYPE(I4): return "I4";
        case X64_TYPE(I5): return "I5";
        case X64_TYPE(I6): return "I6";
        case X64_TYPE(F5): return "F5";
        case X64_TYPE(F6): return "F6";
        default:
            break;
    }
    return "INV";
}

void gen_op_p(tbl *ot, bool ci, size_t idnt) {
    te *h = ot->i->h;
    while (h) {
        te *o = h->d[0].p;
        if (!ci) {
            printf("(%s", gen_op_str(o->d[0].u6));
            gen_op_p(o->d[2].p, true, idnt + 1);
            printf(")\n");
        } else {
            putchar('\n');
            for (size_t i = 0; i < idnt; i++) putchar(' ');
            printf("(%s %s)", gen_cls_str(o->d[1].u3), x64_type_str(o->d[2].u3));
            gen_op_p(o->d[4].p, true, idnt + 1);
            if (h->d[2].p) putchar('\n');
        }
        h = h->d[2].p;
    }
}

void gen_p(gen *g, uint8_t *m) {
    te *h = g->code->h;
    while (h) {
        te *o = h->d[0].p;
        printf("(%s", gen_op_str(o->d[0].u6));
        for (size_t i = 1; i < 4; i++) {
            te *ovt = o->d[i].p;
            if (!ovt) {
                printf(" (N)");
                continue;
            }
            printf(" (%s ", gen_cls_str(ovt->d[0].u3));
            switch (ovt->d[0].u3) {
                case GEN_CLS(A):
                case GEN_CLS(V):
                case GEN_CLS(T):
                    printf("%s ", x64_type_str(ovt->d[1].u3));
                default:
                    break;
            }
            printf("%lu)", ovt->d[2].u6);
        }
        printf(")\n");
        h = h->d[2].p;
    }
}

gen *gen_b(gen *g) {
    GEN_OP_A3(g, GEN_OP(ADD), GEN_CLS(A), U6(X64_TYPE(U6)), GEN_CLS(A), U6(X64_TYPE(U6)), GEN_CLS(A), U6(X64_TYPE(U6)), NULL);
    GEN_OP_A1(g, GEN_OP(RET), GEN_CLS(A), U6(X64_TYPE(U6)), NULL);
    return g;
}
