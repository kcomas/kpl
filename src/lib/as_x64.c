
#include "as_x64.h"

const char *reg_str(size_t rid) {
    switch (rid) {
        case R(AX): return "RAX";
        case R(CX): return "RCX";
        case R(DX): return "RDX";
        case R(BX): return "RBX";
        case R(SP): return "RSP";
        case R(BP): return "RBP";
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
        case ARG_ID(RS): return "RS";
        case ARG_ID(X): return "X";
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
        "CALL",
        "MOV",
        "MOVQ",
        "MOVSD",
        "LEA",
        "ADD",
        "ADDSD",
        "SUB",
        "SUBSD",
        "NEG",
        "INC",
        "DEC",
        "MUL",
        "IMUL",
        "DIV",
        "IDIV",
        "MULSD",
        "DIVSD",
        "CVTSI2SD",
        "AND",
        "XOR",
        "PXOR",
        "CMP",
        "UCOMISD",
        "COMISD",
        "JMP",
        "JB",
        "JNAE",
        "JC",
        "JNB",
        "JAE",
        "JNC",
        "JZ",
        "JE",
        "JNZ",
        "JNE",
        "JBE",
        "JNA",
        "JNBE",
        "JA",
        "JL",
        "JNGE",
        "JNL",
        "JGE",
        "JLE",
        "JNG",
        "JNLE",
        "JG",
        "LOOP",
        "_END"
    };
    const char *s = "INV";
    if (id > AS_X64(_START) && id < AS_X64(_END)) s = insts[id];
    return s;
}

void as_op_p(const tbl *ot, bool args, size_t idnt) {
    te *h = ot->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *o = h->d[0].p;
        if (args) printf("A(%s)\n", arg_id_str(o->d[0].u3));
        else printf("O(%s)\n", as_inst_str(o->d[0].u6));
        if (o->d[3].p) as_op_p(o->d[3].p, true, idnt + 1);
        h = h->d[2].p;
    }
}

static uint8_t scale_g(scale x) {
    switch (x) {
        case S1: return 1;
        case S2: return 2;
        case S4: return 4;
        case S8: return 8;
    }
    return 0;
}

void as_code_i_p(const te *c, const uint8_t *m) {
    if (m) printf("%05lX:", c->d[8].u6);
    if (c->d[0].u6 == CODE_ID(L)) printf("L(0x%lX):\n", c->d[1].u6);
    else {
        printf("O(%s) ", as_inst_str(c->d[1].u6));
        for (size_t i = 2; i < 6; i++) {
            te *arg = c->d[i].p;
            if (!arg) break;
            printf("A(%s):", arg_id_str(arg->d[0].u6));
            switch (arg->d[0].u6) {
                case ARG_ID(R):
                    printf("%s", reg_str(arg->d[1].u3));
                    break;
                case ARG_ID(RM):
                    printf("[%s]", reg_str(arg->d[1].u3));
                    break;
                case ARG_ID(RS):
                    printf("[%s*%u]", reg_str(u3_g_o(arg->d[1], AS_X64_RS_R)), scale_g(u3_g_o(arg->d[1], AS_X64_RS_S)));
                    break;
                case ARG_ID(X):
                    printf("%s", reg_str(arg->d[1].u3));
                    break;
                default:
                    printf("0x%lX", arg->d[1].u6);
                    break;
            }
            putchar(' ');
        }
        if (m) {
            printf("\n ");
            for (size_t x = 0; x < c->d[9].u6; x++) printf("%02X ", m[c->d[8].u6 + x]);
        }
        putchar('\n');
    }
}

void as_data_p(const as *a, const uint8_t *m) {
    te* h = a->dq->h;
    while (h) {
        te *dqe = h->d[0].p;
        printf("%05lX:", dqe->d[4].u6);
        for (size_t i = 0; i < dqe->d[1].u6; i++) printf("%02X ", m[dqe->d[4].u6 + i]);
        putchar('\n');
        h = h->d[2].p;
    }
}

void as_code_p(const as *a, const uint8_t *m) {
    te *h = a->code->h;
    while (h) {
        te *c = h->d[0].p;
        as_code_i_p(c, m);
        h = h->d[2].p;
    }
    if (a->dq->l && m) as_data_p(a, m);
}

static void as_code_err_p(void *d) {
    as_code_i_p(d, NULL);
}

err_d_p *as_x64_err_g_p(as_stat stat) {
    switch (stat) {
        case AS_STAT(CODE):
            return as_code_err_p;
        case AS_STAT(LBL):
            HERE("TODO LBL ERR");
            break;
        case AS_STAT(DATA):
            HERE("TODO DATA ERR");
            break;
        default:
            break;
    }
    return NULL;
}

bool as_x64_dq(as *a, size_t *p, uint8_t *m, te *dqe) {
    (void) a;
    dqe->d[4] = U6(*p); // for print
    te *ci = dqe->d[0].p;
    x64_jmpd_lbld(m, ci->d[8].u6 + ci->d[9].u6, *p);
    return x64_e(p, m, dqe->d[1].u6, dqe->d[2]) == X64_STAT(OK);
}

un as_x64_rs(size_t rid, size_t sid) {
    un rs = U6(0);
    rs = u3_s_o(rs, AS_X64_RS_R, rid);
    scale x;
    switch (sid) {
        case 2:
            x = S2;
            break;
        case 4:
            x = S4;
            break;
        case 8:
            x = S8;
            break;
        default:
            x = S1;
            break;
    }
    rs = u3_s_o(rs, AS_X64_RS_S, x);
    return rs;
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

static bool as_call_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    te *lblc = as_lbl_g_c(a, arg1->d[1].u5);
    if (!lblc) return false;
    if (lblc->d[9].u6) return x64_call_d(p, m, lblc->d[8].u6 - *p - sizeof(uint32_t)) == X64_STAT(OK);
    else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false;
    for (size_t i = 0; i < 5; i++) x64_nop(p, m);
    return true;
}

static bool as_call_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) {
    (void) a;
    size_t p = fc->d[8].u6;
    return x64_call_d(&p, m, lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t) - sizeof(uint32_t)) == X64_STAT(OK);
}

// internal
void as_r_b(as *a);
void as_ro_b(as *a);
void as_rx_b(as *a);
void as_x_b(as *a);
void as_jmp_b(as *a);

as *as_b(as *a) {
    as_op_a(a, AS_X64(NOP), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_nop, NULL);
    as_op_a(a, AS_X64(RET), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_ret, NULL);
    as_op_a(a, AS_X64(LEAVE), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_leave, NULL);
    as_op_a(a, AS_X64(CALL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_call_l, as_call_e);
    as_jmp_b(a);
    as_r_b(a);
    as_ro_b(a);
    as_x_b(a);
    as_rx_b(a);
    return a;
}
