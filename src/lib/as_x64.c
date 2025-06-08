
#include "as_x64.h"

static const char *reg_str(size_t rid) {
    switch (rid) {
        case R(AX): return "RAX";
        case R(CX): return "RCX";
        case R(DX): return "RDX";
        case R(BX): return "RBX";
        case R(SP): return "RSP";
        case R(BP): return "RSP";
        case R(SI): return "RSI";
        case R(DI): return "RDI";
        case R(8): return "R8";
        case R(9): return "R9";
        case R(10): return "R10";
        case R(11): return "R11";
        case R(12): return "R12";
        case R(13): return "R13";
        case R(14): return "R14";
        case R(15): return "R15";
        case XMM(0): return "XMM0";
        case XMM(1): return "XMM1";
        case XMM(2): return "XMM2";
        case XMM(3): return "XMM3";
        case XMM(4): return "XMM4";
        case XMM(5): return "XMM5";
        case XMM(6): return "XMM6";
        case XMM(7): return "XMM7";
        case XMM(8): return "XMM8";
        case XMM(9): return "XMM9";
        case XMM(10): return "XMM10";
        case XMM(11): return "XMM11";
        case XMM(12): return "XMM12";
        case XMM(13): return "XMM13";
        case XMM(14): return "XMM14";
        case XMM(15): return "XMM15";
        default:
            break;
    }
    return "INV";
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
    static const char *insts[] = {
        "_START",
        "NOP",
        "RET",
        "LEAVE",
        "PUSH",
        "POP",
        "MOV",
        "CALL",
        "XOR",
        "CMP",
        // jmps
        "JMP",
        "JNL",
        "JGE",
        "_END"
    };
    const char *s = "INV";
    if (id > AS_INST(_START) && id < AS_INST(_END)) s = insts[id];
    return s;
}

void as_op_p(tbl *ot, bool args, size_t idnt) {
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

void as_code_p(const as *a, const uint8_t *m) {
    te *h = a->code->h;
    while (h) {
        te *c = (te*) h->d[0].p;
        printf("%05lu:", c->d[8].u6);
        if (c->d[0].u6 == CODE_ID(L)) printf("L(%lu):\n", c->d[1].u6);
        else {
            printf("O(%s) ", as_inst_str(c->d[1].u6));
            for (size_t i = 2; i < 6; i++) {
                te *a = c->d[i].p;
                if (!a) break;
                printf("A(%s):", arg_id_str(a->d[0].u6));
                switch (a->d[0].u6) {
                    case ARG_ID(R):
                        printf("%s", reg_str(a->d[1].u6));
                        break;
                    case ARG_ID(RM):
                        printf("(%s)", reg_str(a->d[1].u6));
                        break;
                    default:
                        printf("%lu", a->d[1].u6);
                }
                putchar(' ');
            }
            if (m) {
                printf("\n ");
                for (size_t x = 0; x < c->d[9].u6; x++) printf("%02X ", m[c->d[8].u6 + x]);
            }
            putchar('\n');
        }
        h = h->d[2].p;
    }
}

#define INST(N) static bool as_##N(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg1; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N(p, m) == X64_STAT(OK); \
}

INST(nop);
INST(ret);
INST(leave);

#define INST_R(N) static bool as_##N##_r(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_r(p, m, arg1->d[1].u6) == X64_STAT(OK); \
}

INST_R(push);
INST_R(pop);
INST_R(call);

#define INST_RR(N) static bool as_##N##_rr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rr(p, m, arg1->d[1].u6, arg2->d[1].u6) == X64_STAT(OK); \
}

INST_RR(mov);
INST_RR(xor);
INST_RR(cmp);

bool as_mov_rv(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) ci;
    (void) arg3;
    (void) arg4;
    return x64_mov_rq(p, m, arg1->d[1].u6, arg2->d[1]) == X64_STAT(OK);
}

#define INST_J_B(N, M) static bool as_##N##_b(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##M##_b(p, m, arg1->d[1].u3) == X64_STAT(OK); \
}

//INST_J_B(jmp, jmp);

#define INST_J_L(N) static bool as_##N##_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    te *lblc = as_lbl_g_c(a, arg1->d[1].u6); \
    if (!lblc) return false; \
    if (lblc->d[9].u6) { \
        ssize_t diff = lblc->d[8].u6 - *p - sizeof(uint8_t); \
        if (diff > INT8_MIN && diff < INT8_MAX) return x64_##N##_b(p, m, diff) == X64_STAT(OK); \
        return x64_##N##_dw(p, m, lblc->d[8].u6 - *p - sizeof(uint32_t)) == X64_STAT(OK); \
    } else if (as_lbl_s_c(a, 1, ci) != AS_STAT(OK)) return false; \
    for (size_t i = 0; i < 6; i++) x64_nop(p, m); \
    return true; \
}

// label code, fill code
#define INST_J_F(N) static bool as_##N##_f(as *a, uint8_t *m, te *restrict lc, te *restrict fc) { \
    (void) a; \
    size_t p = fc->d[8].u6; \
    ssize_t diff = lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t); \
    if (diff > INT8_MIN && diff < INT8_MAX) return x64_##N##_b(&p, m, diff) == X64_STAT(OK); \
    return x64_##N##_dw(&p, m, lc->d[8].u6 - fc->d[8].u6 - sizeof(uint32_t)) == X64_STAT(OK); \
}

#define INST_J_LF(N) INST_J_L(N) \
    INST_J_F(N)

INST_J_LF(jmp);
INST_J_LF(jnljge);

as *as_b(as *a) {
    as_op_a(a, AS_INST(NOP), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_nop, NULL);
    as_op_a(a, AS_INST(RET), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_ret, NULL);
    as_op_a(a, AS_INST(LEAVE), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_leave, NULL);
    as_op_a(a, AS_INST(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_push_r, NULL);
    as_op_a(a, AS_INST(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_pop_r, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_mov_rr, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), &as_mov_rv, NULL);
    as_op_a(a, AS_INST(MOV), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), &as_mov_rv, NULL);
    as_op_a(a, AS_INST(CALL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_call_r, NULL);
    as_op_a(a, AS_INST(XOR), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_xor_rr, NULL);
    as_op_a(a, AS_INST(CMP), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_cmp_rr, NULL);
    // jmps
    as_op_a(a, AS_INST(JMP), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jmp_l, &as_jmp_f);
    as_op_a(a, AS_INST(JNL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnljge_l, &as_jnljge_f);
    as_op_a(a, AS_INST(JGE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnljge_l, &as_jnljge_f);
    return a;
}

