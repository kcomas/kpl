
#include "../as_x64.h"

// jmp, set

#define JBS (sizeof(uint8_t) * 2)
#define JMPS (sizeof(uint8_t) + sizeof(uint32_t))

static bool as_jmp_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    te *lblc = as_lbl_g_c(a, arg1->d[1].u6);
    if (!lblc) return false;
    if (lblc->d[9].u6) {
        ssize_t diff = lblc->d[8].u6 - *p - JBS;
        if (diff >= INT8_MIN && diff <= INT8_MAX) return x64_jmp_b(p, m, diff) == X64_STAT(OK);
        return x64_jmp_d(p, m, lblc->d[8].u6 - *p - JMPS) == X64_STAT(OK);
    } else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false;
    for (size_t i = 0; i < 5; i++) x64_nop(p, m);
    return true;
}

static bool as_jmp_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) {
    (void) a;
    size_t p = fc->d[8].u6;
    ssize_t diff = lc->d[8].u6 - fc->d[8].u6 - JBS;
    if (diff >= INT8_MIN && diff <= INT8_MAX) return x64_jmp_b(&p, m, diff) == X64_STAT(OK);
    return x64_jmp_d(&p, m, lc->d[8].u6 - fc->d[8].u6 - JMPS) == X64_STAT(OK);
}

#define JDS (JBS + sizeof(uint32_t))

#define INST_J_L(N) static bool as_##N##_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    te *lblc = as_lbl_g_c(a, arg1->d[1].u6); \
    if (!lblc) return false; \
    if (lblc->d[9].u6) { \
        ssize_t diff = lblc->d[8].u6 - *p - JBS; \
        if (diff >= INT8_MIN && diff <= INT8_MAX) return x64_##N##_b(p, m, diff) == X64_STAT(OK); \
        return x64_##N##_d(p, m, lblc->d[8].u6 - *p - JDS) == X64_STAT(OK); \
    } else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false; \
    for (size_t i = 0; i < 6; i++) x64_nop(p, m); \
    return true; \
}

// label code, end fill code
#define INST_J_E(N) static bool as_##N##_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) { \
    (void) a; \
    size_t p = fc->d[8].u6; \
    ssize_t diff = lc->d[8].u6 - fc->d[8].u6 - JBS; \
    if (diff >= INT8_MIN && diff <= INT8_MAX) return x64_##N##_b(&p, m, diff) == X64_STAT(OK); \
    return x64_##N##_d(&p, m, lc->d[8].u6 - fc->d[8].u6 - JDS) == X64_STAT(OK); \
}

#define INST_J_LE(N) INST_J_L(N) \
    INST_J_E(N)

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

#define SET(N) static bool as_##N##_r(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) { \
    (void) a; \
    (void) ci; \
    (void) arg2; \
    (void) arg3; \
    (void) arg4; \
    return x64_##N##_r(p, m, arg1->d[1].u3) == X64_STAT(OK); \
}

SET(setbsetnaesetc);
SET(setnbsetaesetnc);
SET(setzsete);
SET(setnzsetne);
SET(setbesetna);
SET(setnbeseta);
SET(setlsetnge);
SET(setnlsetge);
SET(setlesetng);
SET(setnlesetg);

static bool as_loop_l(as *a, te *restrict ci, size_t *p, uint8_t *m, te *restrict arg1, te *restrict arg2, te *restrict arg3, te *restrict arg4) {
    (void) a;
    (void) arg2;
    (void) arg3;
    (void) arg4;
    te *lblc = as_lbl_g_c(a, arg1->d[1].u6);
    if (!lblc) return false;
    if (lblc->d[9].u6) {
        ssize_t diff = lblc->d[8].u6 - *p - sizeof(uint8_t);
        if (diff < INT8_MIN && diff > INT8_MAX) return false;
        return x64_loop_b(p, m, diff) == X64_STAT(OK);
    } else if (as_lbl_s_c(a, arg1->d[1].u6, ci) != AS_STAT(OK)) return false;
    x64_nop(p, m);
    x64_nop(p, m);
    return true;
}

static bool as_loop_e(as *a, uint8_t *m, te *restrict lc, te *restrict fc) {
    (void) a;
    size_t p = fc->d[8].u6;
    ssize_t diff = lc->d[8].u6 - fc->d[8].u6 - sizeof(uint8_t) * 2;
    if (diff < INT8_MIN && diff > INT8_MAX) return false;
    return x64_loop_b(&p, m, diff) == X64_STAT(OK);
}

as *as_jmp_b(as *a) {
    as_op_a(a, AS_X64(JMP), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jmp_l, as_jmp_e);
    as_op_a(a, AS_X64(JB), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jbjnaejc_l, as_jbjnaejc_e);
    as_op_a(a, AS_X64(JNAE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jbjnaejc_l, as_jbjnaejc_e);
    as_op_a(a, AS_X64(JC), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jbjnaejc_l, as_jbjnaejc_e);
    as_op_a(a, AS_X64(JNB), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnbjaejnc_l, as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JAE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnbjaejnc_l, as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JNC), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnbjaejnc_l, as_jnbjaejnc_e);
    as_op_a(a, AS_X64(JZ), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jzje_l, as_jzje_e);
    as_op_a(a, AS_X64(JE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jzje_l, as_jzje_e);
    as_op_a(a, AS_X64(JNZ), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnzjne_l, as_jnzjne_e);
    as_op_a(a, AS_X64(JNE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnzjne_l, as_jnzjne_e);
    as_op_a(a, AS_X64(JBE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jbejna_l, as_jbejna_e);
    as_op_a(a, AS_X64(JNBE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnbeja_l, as_jnbeja_e);
    as_op_a(a, AS_X64(JA), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnbeja_l, as_jnbeja_e);
    as_op_a(a, AS_X64(JL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jljnge_l, as_jljnge_e);
    as_op_a(a, AS_X64(JNGE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jljnge_l, as_jljnge_e);
    as_op_a(a, AS_X64(JNL), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnljge_l, as_jnljge_e);
    as_op_a(a, AS_X64(JGE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnljge_l, as_jnljge_e);
    as_op_a(a, AS_X64(JLE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jlejng_l, as_jlejng_e);
    as_op_a(a, AS_X64(JNG), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jlejng_l, as_jlejng_e);
    as_op_a(a, AS_X64(JNLE), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnlejg_l, as_jnlejg_e);
    as_op_a(a, AS_X64(JG), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_jnlejg_l, as_jnlejg_e);
    as_op_a(a, AS_X64(SETB), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setbsetnaesetc_r, NULL);
    as_op_a(a, AS_X64(SETNAE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setbsetnaesetc_r, NULL);
    as_op_a(a, AS_X64(SETC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setbsetnaesetc_r, NULL);
    as_op_a(a, AS_X64(SETNB), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnbsetaesetnc_r, NULL);
    as_op_a(a, AS_X64(SETAE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnbsetaesetnc_r, NULL);
    as_op_a(a, AS_X64(SETNC), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnbsetaesetnc_r, NULL);
    as_op_a(a, AS_X64(SETZ), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setzsete_r, NULL);
    as_op_a(a, AS_X64(SETE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setzsete_r, NULL);
    as_op_a(a, AS_X64(SETNZ), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnzsetne_r, NULL);
    as_op_a(a, AS_X64(SETNE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnzsetne_r, NULL);
    as_op_a(a, AS_X64(SETBE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setbesetna_r, NULL);
    as_op_a(a, AS_X64(SETNA), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setbesetna_r, NULL);
    as_op_a(a, AS_X64(SETNBE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnbeseta_r, NULL);
    as_op_a(a, AS_X64(SETA), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnbeseta_r, NULL);
    as_op_a(a, AS_X64(SETL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setlsetnge_r, NULL);
    as_op_a(a, AS_X64(SETNGE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setlsetnge_r, NULL);
    as_op_a(a, AS_X64(SETNL), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnlsetge_r, NULL);
    as_op_a(a, AS_X64(SETGE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnlsetge_r, NULL);
    as_op_a(a, AS_X64(SETLE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setlesetng_r, NULL);
    as_op_a(a, AS_X64(SETNG), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setlesetng_r, NULL);
    as_op_a(a, AS_X64(SETNLE), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnlesetg_r, NULL);
    as_op_a(a, AS_X64(SETG), ARG_ID(R), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_setnlesetg_r, NULL);
    as_op_a(a, AS_X64(LOOP), ARG_ID(L), ARG_ID(N), ARG_ID(N), ARG_ID(N), as_loop_l, as_loop_e);
    return a;
}
