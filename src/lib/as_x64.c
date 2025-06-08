
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
        "ADD",
        "SUB",
        "INC",
        "DEC",
        "AND",
        "XOR",
        "CMP",
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
        "_END"
    };
    const char *s = "INV";
    if (id > AS_X64(_START) && id < AS_X64(_END)) s = insts[id];
    return s;
}

te *as_arg(alfn *aa, frfn *af, arg_id id, un v) {
    te *a = te_i(2, aa, af);
    a->d[0] = U6(id);
    a->d[1] = v;
    return a;
}

void as_op_p(const tbl *ot, bool args, size_t idnt) {
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

void as_code_i_p(const te *c, const uint8_t *m) {
    if (m) printf("%05lu:", c->d[8].u6);
    if (c->d[0].u6 == CODE_ID(L)) printf("L(%lu):\n", c->d[1].u6);
    else {
        printf("O(%s) ", as_inst_str(c->d[1].u6));
        for (size_t i = 2; i < 6; i++) {
            te *arg = c->d[i].p;
            if (!arg) break;
            printf("A(%s):", arg_id_str(arg->d[0].u6));
            switch (arg->d[0].u6) {
                case ARG_ID(R):
                    printf("%s", reg_str(arg->d[1].u6));
                    break;
                case ARG_ID(RM):
                    printf("[%s]", reg_str(arg->d[1].u6));
                    break;
                default:
                    printf("%lu", arg->d[1].u6);
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

void as_code_p(const as *a, const uint8_t *m) {
    te *h = a->code->h;
    while (h) {
        te *c = (te*) h->d[0].p;
        as_code_i_p(c, m);
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
INST_R(inc);
INST_R(dec);

#define INST_RR(N) static bool as_##N##_rr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rr(p, m, arg1->d[1].u6, arg2->d[1].u6) == X64_STAT(OK); \
}

INST_RR(mov);
INST_RR(add);
INST_RR(sub);
INST_RR(and);
INST_RR(xor);
INST_RR(cmp);

#define INST_RB(N) static bool as_##N##_rb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rb(p, m, arg1->d[1].u6, arg2->d[1].u3) == X64_STAT(OK); \
}

INST_RB(add);
INST_RB(sub);

#define INST_RMR(N) static bool as_##N##_rmr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rmr(p, m, arg1->d[1].u6, arg2->d[1].u6) == X64_STAT(OK); \
}

INST_RMR(mov);

#define INST_RMBR(N) static bool as_##N##_rmbr(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rmbr(p, m, arg1->d[1].u6, arg2->d[1].u3, arg3->d[1].u6) == X64_STAT(OK); \
}

INST_RMBR(mov);

#define INST_RRM(N) static bool as_##N##_rrm(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_rrm(p, m, arg1->d[1].u6, arg2->d[1].u6) == X64_STAT(OK); \
}

INST_RRM(mov);
INST_RRM(cmp);

#define INST_RRMB(N) static bool as_##N##_rrmb(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg4; \
    return x64_##N##_rrmb(p, m, arg1->d[1].u6, arg2->d[1].u6, arg3->d[1].u3) == X64_STAT(OK); \
}

INST_RRMB(mov);

bool as_mov_rv(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) ci;
    (void) arg3;
    (void) arg4;
    return x64_mov_rq(p, m, arg1->d[1].u6, arg2->d[1]) == X64_STAT(OK);
}

static bool as_call_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    te *lblc = as_lbl_g_c(a, arg1->d[1].u6);
    if (!lblc) return false;
    if (lblc->d[9].u6) return x64_call_dw(p, m, lblc->d[8].u6 - *p - sizeof(uint32_t)) == X64_STAT(OK);
    else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false;
    for (size_t i = 0; i < 5; i++) x64_nop(p, m);
    return true;
}

static bool as_call_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) {
    (void) a;
    size_t p = fc->d[8].u6;
    return x64_call_dw(&p, m, lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t) - sizeof(uint32_t)) == X64_STAT(OK);
}

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
    } else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false; \
    for (size_t i = 0; i < 6; i++) x64_nop(p, m); \
    return true; \
}

// label code, end fill code
#define INST_J_E(N) static bool as_##N##_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) { \
    (void) a; \
    size_t p = fc->d[8].u6; \
    ssize_t diff = lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t) * 2; \
    if (diff > INT8_MIN && diff < INT8_MAX) return x64_##N##_b(&p, m, diff) == X64_STAT(OK); \
    return x64_##N##_dw(&p, m, lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t) * 2 - sizeof(uint32_t)) == X64_STAT(OK); \
}

#define INST_J_LE(N) INST_J_L(N) \
    INST_J_E(N)

INST_J_LE(jmp);
INST_J_LE(jbjnaejc);
INST_J_LE(jnbjaejnc);
INST_J_LE(jzje);
INST_J_LE(jnzjne);
INST_J_LE(jbejna);
INST_J_LE(jnbeja);
INST_J_LE(jljnge);
INST_J_LE(jnljge);
INST_J_LE(jlejng);
INST_J_LE(jnlejg);

as *as_b(as *a) {
    as_op_a(a, AS_X64(NOP), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_nop, NULL);
    as_op_a(a, AS_X64(RET), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_ret, NULL);
    as_op_a(a, AS_X64(LEAVE), ARG_ID(N), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_leave, NULL);
    as_op_a(a, AS_X64(PUSH), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_push_r, NULL);
    as_op_a(a, AS_X64(POP), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_pop_r, NULL);
    as_op_a(a, AS_X64(CALL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_call_r, NULL);
    as_op_a(a, AS_X64(CALL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_call_l, &as_call_e);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_mov_rr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(QW), ARG_ID(N), ARG_ID(N), &as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), &as_mov_rv, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_mov_rmr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(RM), ARG_ID(B), ARG_ID(R), ARG_ID(N), &as_mov_rmbr, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), &as_mov_rrm, NULL);
    as_op_a(a, AS_X64(MOV), ARG_ID(R), ARG_ID(RM), ARG_ID(B), ARG_ID(N), &as_mov_rrmb, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_add_rr, NULL);
    as_op_a(a, AS_X64(ADD), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), &as_add_rb, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_sub_rr, NULL);
    as_op_a(a, AS_X64(SUB), ARG_ID(R), ARG_ID(B), ARG_ID(N), ARG_ID(N), &as_sub_rb, NULL);
    as_op_a(a, AS_X64(INC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_inc_r, NULL);
    as_op_a(a, AS_X64(DEC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_dec_r, NULL);
    as_op_a(a, AS_X64(AND), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_and_rr, NULL);
    as_op_a(a, AS_X64(XOR), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_xor_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(R), ARG_ID(N), ARG_ID(N), &as_cmp_rr, NULL);
    as_op_a(a, AS_X64(CMP), ARG_ID(R), ARG_ID(RM), ARG_ID(N), ARG_ID(N), &as_cmp_rrm, NULL);
    as_op_a(a, AS_X64(JMP), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jmp_l, &as_jmp_e);
    as_op_a(a, AS_X64(JB), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jbjnaejc_l, &as_jbjnaejc_e);
    as_op_a(a, AS_X64(JNAE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jbjnaejc_l, &as_jbjnaejc_e);
    as_op_a(a, AS_X64(JC), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jbjnaejc_l, &as_jbjnaejc_e);
    as_op_a(a, AS_X64(JNB), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnbjaejnc_l, &as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JAE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnbjaejnc_l, &as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JNC), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnbjaejnc_l, &as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JZ), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jzje_l, &as_jzje_e);
    as_op_a(a, AS_X64(JE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jzje_l, &as_jzje_e);
    as_op_a(a, AS_X64(JNZ), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnzjne_l, &as_jnzjne_e);
    as_op_a(a, AS_X64(JNE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnzjne_l, &as_jnzjne_e);
    as_op_a(a, AS_X64(JBE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jbejna_l, &as_jbejna_e);
    as_op_a(a, AS_X64(JNBE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnbeja_l, &as_jnbeja_e);
    as_op_a(a, AS_X64(JA), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnbeja_l, &as_jnbeja_e);
    as_op_a(a, AS_X64(JL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jljnge_l, &as_jljnge_e);
    as_op_a(a, AS_X64(JNGE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jljnge_l, &as_jljnge_e);
    as_op_a(a, AS_X64(JNL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnljge_l, &as_jnljge_e);
    as_op_a(a, AS_X64(JGE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnljge_l, &as_jnljge_e);
    as_op_a(a, AS_X64(JLE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jlejng_l, &as_jlejng_e);
    as_op_a(a, AS_X64(JNG), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jlejng_l, &as_jlejng_e);
    as_op_a(a, AS_X64(JNLE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnlejg_l, &as_jnlejg_e);
    as_op_a(a, AS_X64(JG), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), &as_jnlejg_l, &as_jnlejg_e);
    return a;
}

