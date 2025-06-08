
#include "as_t.h"

te *as_arg_r(size_t rid) {
    te *a = te_i(2, &malloc, &free);
    a->d[0] = U6(ARG_ID(R));
    a->d[1] = I6(rid);
    return a;
}

static size_t no_hsh(un d) {
    return d.u6;
}

static bool id_eq(un a, un b) {
    return a.u6 == b.u6;
}

lst *as_mklst(void) {
    return lst_i(&malloc, &malloc, &free, (void*) &te_f, &free);
}

tbl *as_mktbl(void) {
    lst *tl = as_mklst();
    te *b = te_i(10, &malloc, &free);
    tbl *t = tbl_i(&malloc, &free, &no_hsh, &id_eq, tl, b);
    return t;
}

static const char *arg_id_str(size_t id) {
    switch (id) {
        case ARG_ID(N): return "N";
        case ARG_ID(R): return "R";
        case ARG_ID(RM): return "RM";
        case ARG_ID(L): return "L";
        case ARG_ID(M): return "M";
        case ARG_ID(B): return "B";
        case ARG_ID(W): return "W";
        case ARG_ID(DW): return "DW";
        case ARG_ID(QW): return "QW";
        default:
            break;
    }
    return "INV";
}

static const char *as_inst_str(size_t id) {
    switch (id) {
        case AS_INST(NOP): return "NOP";
        case AS_INST(RET): return "RET";
        case AS_INST(LEAVE): return "LEAVE";
        case AS_INST(PUSH): return "PUSH";
        case AS_INST(POP): return "POP";
        case AS_INST(MOV): return "MOV";
        default:
            break;
    }
    return "INV";
}

void as_op_p(tbl *const ot, bool args, size_t idnt) {
    te *h = ot->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *o = h->d[0].p;
        if (args) printf("A(%s)\n", arg_id_str(o->d[0].u6));
        else printf("O(%s)\n", as_inst_str(o->d[0].u6));
        if (o->d[3].p) as_op_p(o->d[3].p, true, idnt + 1);
        h = h->d[2].p;
    }
}

void as_code_p(const as *const a) {
    te *h = a->code->h;
    while (h) {
        te *c = (te*) h->d[0].p;
        if (c->d[0].u6 == CODE_ID(L)) printf("L(%lu):\n", c->d[1].u6);
        else {
            printf("O(%s) ", as_inst_str(c->d[1].u6));
            for (size_t i = 2; i < 6; i++) {
                te *a = c->d[i].p;
                if (a) printf("A(%s):%lu ", arg_id_str(a->d[0].u6), a->d[1].u6);
                else break;
            }
            putchar('\n');
        }
        h = h->d[2].p;
    }
}

bool as_ret(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_ret(p, m) == JIT_STAT(OK);
}

bool as_push_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_push_r(p, m, arg1->d[0].u6) == JIT_STAT(OK);
}

bool as_pop_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_pop_r(p, m, arg1->d[0].u6) == JIT_STAT(OK);
}

bool as_mov_rr(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg3;
    (void) arg4;
    return jit_mov_rr(p, m, arg1->d[0].u6, arg2->d[1].u6) == JIT_STAT(OK);
}

void label_entry_f(void *p) {
    te *l = (te*) p;
    te_f(l->d[1].p);
    lst_f(l->d[2].p);
    free(l);
}

void op_entry_f(void *p) {
    te *oe = (te*) p;
    tbl_f(oe->d[3].p);
    free(oe);
}

void code_entry_f(void *p) {
    te *o = (te*) p;
    te_f(o->d[2].p);
    te_f(o->d[3].p);
    te_f(o->d[4].p);
    te_f(o->d[5].p);
    free(o);
}
