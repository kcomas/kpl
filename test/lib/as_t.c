
#include "as_t.h"

te *as_arg_r(size_t rid) {
    te *a = te_i(2, &malloc, &free);
    a->d[0] = U6(ARG_ID(R));
    a->d[1] = U6(rid);
    return a;
}

static te *as_arg_v(arg_id id, un v) {
    te *a = te_i(2, &malloc, &free);
    a->d[0] = U6(id);
    a->d[1] = v;
    return a;
}

te *as_arg_qw(un v) {
    return as_arg_v(ARG_ID(QW), v);
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
        case AS_INST(CALL): return "CALL";
        case AS_INST(XOR): return "XOR";
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

void as_code_p(const as *const a, const uint8_t *const m) {
    te *h = a->code->h;
    while (h) {
        te *c = (te*) h->d[0].p;
        if (c->d[0].u6 == CODE_ID(L)) printf("L(%lu):\n", c->d[1].u6);
        else {
            printf("O(%s) ", as_inst_str(c->d[1].u6));
            for (size_t i = 2; i < 6; i++) {
                te *a = c->d[i].p;
                if (!a) break;
                printf("A(%s):%lu ", arg_id_str(a->d[0].u6), a->d[1].u6);

            }
            if (m) {
                putchar('\n');
                for (size_t x = 0; x < c->d[9].u6; x++) {
                    printf("%X ", m[c->d[8].u6 + x]);
                }
            }
            putchar('\n');
        }
        h = h->d[2].p;
    }
}

bool as_nop(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg1;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_nop(p, m) == JIT_STAT(OK);
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
    return jit_push_r(p, m, arg1->d[1].u6) == JIT_STAT(OK);
}

bool as_pop_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_pop_r(p, m, arg1->d[1].u6) == JIT_STAT(OK);
}

bool as_mov_rr(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg3;
    (void) arg4;
    return jit_mov_rr(p, m, arg1->d[1].u6, arg2->d[1].u6) == JIT_STAT(OK);
}

bool as_mov_rv(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg3;
    (void) arg4;
    return jit_mov_rq(p, m, arg1->d[1].u6, arg2->d[1]) == JIT_STAT(OK);
}

bool as_call_r(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    return jit_call_r(p, m, arg1->d[1].u6) == JIT_STAT(OK);
}

bool as_xor_rr(as *const a, size_t *p, uint8_t *m, te *arg1, te *arg2, te *arg3, te *arg4) {
    (void) a;
    (void) arg3;
    (void) arg4;
    return jit_xor_rr(p, m, arg1->d[1].u6, arg2->d[1].u6) == JIT_STAT(OK);
}


as *as_b(void) {
    as *a = as_i(&malloc, &free, &label_entry_f, &op_entry_f, &code_entry_f, &as_mktbl, as_mktbl(), as_mklst());
    as_op_a(a, AS_INST(NOP), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_nop, NULL);
    as_op_a(a, AS_INST(RET), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_ret, NULL);
    as_op_a(a, AS_INST(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_push_r, NULL);
    as_op_a(a, AS_INST(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_pop_r, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_mov_rr, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), &as_mov_rv, NULL);
    as_op_a(a, AS_INST(CALL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_call_r, NULL);
    as_op_a(a, AS_INST(XOR), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_xor_rr, NULL);
    return a;
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
