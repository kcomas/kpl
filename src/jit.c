
#include "jit.h"

static const char *const jss[] = {
    "OK",
    "SG_T_INV",
    "LG_T_INV",
    "LM_INV",
    "PV_T_INV",
    "ZOO_T_INV",
    "ADD_T_INV",
    "SUB_T_INV",
    "MUL_T_INV",
    "DIV_T_INV",
    "EQ_T_INV",
    "GT_T_INV",
    "LT_T_INV",
    "OR_T_INV",
    "CSTSG_T_INV",
    "CNCTSG_T_INV",
    "WFD_T_INV",
    "RCI_T_INV",
    "RCD_T_INV",
    "RCF_T_INV",
    "GC_T_INV",
    "GCTSV_T_INV",
    "GCVR_T_INV",
    "DEL_T_INV",
    "IF_ELSE_INV",
    "INV_CODE"
};

const char *jit_stat_str(jit_stat jstat) {
    const char *s = "INVALID_JIT_STAT";
    if (jstat >= JIT_STAT(OK) && jstat <= JIT_STAT(INV_CODE)) s = jss[jstat];
    return s;
}

extern inline fn_stk *fn_stk_i(al *const a, size_t size);

extern inline void fn_stk_a(al *const a, fn_stk **stk, code *const c);

void fn_stk_b(al *const a, fn_stk **stk, code *const c) {
    for (size_t i = 0; i < c->len; i++) {
        if (c->ops[i].oc == OP_C(PV) && c->ops[i].ot == TYPE(FN)) {
            fn_stk_b(a, stk, c->ops[i].od.c);
            fn_stk_a(a, stk, c->ops[i].od.c);
        }
        if (c->ops[i].oc == OP_C(CTSV) || c->ops[i].oc == OP_C(LM)) {
            fn_stk_b(a, stk, c->ops[i].od.tsvm->gc);
            fn_stk_a(a, stk, c->ops[i].od.tsvm->gc);
        }
        if (c->ops[i].oc == OP_C(IF) && c->ops[i].ot == TYPE(IF)) fn_stk_b(a, stk, c->ops[i].od.c);
        if ((c->ops[i].oc == OP_C(LOP) && c->ops[i].ot == TYPE(LOP)) || (c->ops[i].oc == OP_C(COND) && c->ops[i].ot == TYPE(COND))) fn_stk_b(a, stk, c->ops[i].od.of->body);
    }
    (*stk)->nops += c->len;
}

extern inline void fn_stk_f(fn_stk *f);

void jit_i(al *const a, size_t nops, jit **j, uint8_t flgs) {
    size_t size = nops * BYTES_PER_OP;
    if ((*j)->size > 0) {
        if ((*j)->size <= size) {
            (*j)->len = 0;
            (*j)->flgs = flgs;
            return;
        } else if ((*j)->size < size) {
            jit_f(*j);
            *j = ala(a, sizeof(jit));
            (*j)->flgs = flgs;
        }
    }
    size_t ps = (size_t) getpagesize() * 5;
    (*j)->size = (size / ps + 1) * ps;
    (*j)->h = mmap(NULL, (*j)->size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANONYMOUS | MAP_SHARED, -1, 0);
}

extern inline jit_stat jit_er(mod *const m, const char *const fnn, jit_stat jstat, const op *const o);

#define JIT_ER(M, JSTAT, O) jit_er(M, __func__, JIT_STAT(JSTAT), O);

extern inline void jit_a(jit *j, uint8_t b);

extern inline void jit_b(jit *j, size_t len, ...);

extern inline void jit_c(jit *j, size_t len, uint8_t b[]);

static void op_set_jidx(const jit *const j, op *const o) {
    if (!(j->flgs & JIT_FLG(TD))) o->jidx = j->len;
}

static void op_set_jlen(const jit *const j, op *const o) {
    if (!(j->flgs & JIT_FLG(TD))) o->jlen = j->len - o->jidx;
}

static void mov_reg(jit *j, bool rexwr, uint8_t reg, uint8_t *buf) {
    jit_a(j, rexwr ? 0x49 : 0x48);
    jit_a(j, 0xB8 + reg);
    jit_c(j, sizeof(void*), buf);
}

#define SET_BUF(BUF, DATA, SIZE) memset(BUF, 0, sizeof(void*)); \
    memcpy(BUF, &DATA, SIZE)

#define SET_REG(TGT, SIZEOF, REXRW, REGN) SET_BUF(buf, TGT, sizeof(SIZEOF)); \
    mov_reg(j, REXRW, 0x0##REGN, buf)

#define SET_FP(FN) fp = (void*) &FN

#define CT_SET_FN(T, FN) case TYPE(T): \
    SET_FP(FN); \
    break

#define SET_REG_CALL(REXRW, REGN) SET_REG(fp, void*, REXRW, REGN); \
    jit_b(j, 2, 0xFF, 0xD##REGN);

#define OPPVT(T, D, CT) case TYPE(T): \
    SET_BUF(buf, o->od.D, sizeof(CT)); \
    mov_reg(j, true, 0x01, buf); \
    jit_b(j, 2, 0x49, 0x51); \
    op_set_jlen(j, o); \
    break

// ids start at zero
static void j_pop(jit *j, type t, uint8_t rid, uint8_t xid) {
    switch (t) {
        case TYPE(F5):
        case TYPE(F6):
            jit_b(j, 5, 0xF2, 0x0F, 0x10, 0x04 + (xid * 8), 0x24); // movsd x + xd qword ptr rsp
            jit_b(j, 4, 0x48, 0x83, 0xC4, 0x08); // add rsp 8
            break;
        default:
            jit_a(j, 0x58 + rid); // pop r + id
            break;
    }
}

// ids start at zero
static void j_push(jit *j, type t, uint8_t rid, uint8_t xid) {
    switch (t) {
        case TYPE(F5):
        case TYPE(F6):
            jit_b(j, 4, 0x48, 0x83, 0xEC, 0x08); // sub rsp 8
            jit_b(j, 5, 0xF2, 0x0F, 0x11, 0x04 + (xid * 8), 0x24); // mov qword ptr rsp x + xid
            break;
        default:
            jit_a(j, 0x50 + rid); // push r + id
            break;
    }
}

#define C_OP_C_BOP(N, FN) case OP_C(N): \
    op_set_jidx(j, o); \
    j_pop(j, o->od.t, 6, 1); \
    j_pop(j, o->od.t, 7, 0); \
    switch (o->od.t) { \
        CT_SET_FN(BL, var_##FN##_bl); \
        CT_SET_FN(I6, var_##FN##_i6); \
        CT_SET_FN(U6, var_##FN##_u6); \
        CT_SET_FN(F6, var_##FN##_f6); \
        default: \
            return JIT_ER(m, N##_T_INV, o); \
    } \
    SET_REG_CALL(false, 0); \
    j_push(j, o->od.t, 0, 0); \
    op_set_jlen(j, o); \
    break

#ifndef IF_STK_LEN
    #define IF_STK_LEN 10
#endif

static jit_stat jit_if(mod *const m, code *const c, jit_fn *const jf, jit *j) {
    jit_stat jstat;
    op *o;
    int32_t jmpp, jmpl;
    int32_t stki = 0;
    int32_t stk[IF_STK_LEN];
    memset(stk, 0, sizeof(int32_t) * IF_STK_LEN);
    for (size_t i = 0; i < c->len; i++) {
        o = &c->ops[i];
        op_set_jidx(j, o);
        if (o->od.of->cond->len == 0) {
            if (i != c->len - 1) return JIT_ER(m, IF_ELSE_INV, o);
            if ((jstat = jit_code(m, o->od.of->body, jf, j, false)) != JIT_STAT(OK)) return jstat;
            break;
        }
        if ((jstat = jit_code(m, o->od.of->cond, jf, j, false)) != JIT_STAT(OK)) return jstat; // 0 or 1 on stack
        jit_b(j, 2, 0x41, 0x5B); // pop r11
        jit_b(j, 3, 0x4D, 0x85, 0xDB); // test r11 r11
        jit_b(j, 2, 0x0F, 0x84); // je 0
        jmpp = j->len;
        jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after body jmp to next cond
        if ((jstat = jit_code(m, o->od.of->body, jf, j, false)) != JIT_STAT(OK)) return jstat;
        jit_a(j, 0xE9); // jmp
        stk[stki++] = j->len;
        jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after if jmp to end of if
        jmpl = j->len - jmpp - sizeof(int32_t);
        memcpy(j->h + jmpp, &jmpl, sizeof(int32_t)); // fill cond skip
        op_set_jlen(j, o);
    }
    while (stki > 0) {
        jmpl = j->len - stk[stki - 1] - sizeof(int32_t);
        memcpy(j->h + stk[stki - 1], &jmpl, sizeof(int32_t)); // fill body skip
        stki--;
    }
    return JIT_ER(m, OK, NULL);
}

static jit_stat jit_cond(mod *const m, op_if *const of, jit_fn *const jf, jit *j) {
    jit_stat jstat;
    int32_t jmpp, jmpl;
    if ((jstat = jit_code(m, of->cond, jf, j, false)) != JIT_STAT(OK)) return jstat; // 0 or 1 on stack
    jit_b(j, 2, 0x41, 0x5B); // pop r11
    jit_b(j, 3, 0x4D, 0x85, 0xDB); // test r11 r11
    jit_b(j, 2, 0x0F, 0x84); // je 0
    jmpp = j->len;
    jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after body jmp to end
    if ((jstat = jit_code(m, of->body, jf, j, false)) != JIT_STAT(OK)) return jstat;
    jmpl = j->len - jmpp - sizeof(int32_t);
    memcpy(j->h + jmpp, &jmpl, sizeof(int32_t)); // fill cond skip
    return JIT_ER(m, OK, NULL);
}

static jit_stat jit_lop(mod *const m, op_if *const of, jit_fn *const jf, jit *j) {
    jit_stat jstat;
    int32_t lops = j->len, lope, bs, jmpl;
    if ((jstat = jit_code(m, of->cond, jf, j, false)) != JIT_STAT(OK)) return jstat; // 0 or 1 on stack
    jit_b(j, 2, 0x41, 0x5B); // pop r11
    jit_b(j, 3, 0x4D, 0x85, 0xDB); // test r11 r11
    jit_b(j, 2, 0x0F, 0x84); // je 0
    bs = j->len;
    jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after body jmp to end of lop
    if ((jstat = jit_code(m, of->body, jf, j, false)) != JIT_STAT(OK)) return jstat;
    jit_a(j, 0xE9); // jmp
    lope = j->len;
    jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after lop jmp to start of if
    jmpl = lops - j->len;
    memcpy(j->h + lope, &jmpl, sizeof(int32_t));
    jmpl = j->len - bs - sizeof(int32_t);
    memcpy(j->h + bs, &jmpl, sizeof(int32_t));
    return JIT_ER(m, OK, NULL);
}

static void jit_var_tsv_gidx(jit *j, bool push) {
    void *fp;
    uint8_t buf[sizeof(void*)];
    jit_b(j, 4, 0x48, 0x8B, 0x7D, 2 * sizeof(void*)); // mov rdi rbp+16
    jit_b(j, 4, 0x48, 0x8B, 0x34, 0x24); // mov rsi qword ptr rsp
    SET_FP(var_tsv_gidx);
    SET_REG_CALL(false, 0);
    if (push) jit_a(j, 0x50); // push rax
    else jit_b(j, 3, 0x48, 0x89, 0xC7); // mov rdi rax
}

#define JIT_TD_GC() jit_b(j, 4, 0X48, 0X8B, 0X3C, 0X24); /* mov rdi qword ptr [rsp] */ \
    SET_FP(var_td_te); \
    SET_REG_CALL(false, 0); \
    jit_a(j, 0x50); /* push rax */ \
    jit_b(j, 4, 0X48, 0X8B, 0X3C, 0X24); /* mov rdi qword ptr [rsp] */ \
    SET_FP(var_tsv_gc); \
    SET_REG_CALL(false, 0); \
    jit_b(j, 2, 0XFF, 0XD0); /* call rax with gc fn */ \
    jit_a(j, 0X5F); /* pop rdi */ \
    jit_a(j, 0X5F); /* pop rdi */ \
    SET_FP(var_td_f); \
    SET_REG_CALL(false, 0)

static jit_stat jit_gc_vr(mod *const m, const op *const o, jit *j) {
    void *fp;
    uint8_t buf[sizeof(void*)];
    int32_t lops, lope, bs, jmpl;
    jit_b(j, 3, 0x48, 0x31, 0xF6); // xor rsi rsi, rsi=0
    jit_a(j, 0x56); // push rsi
    lops = j->len; // loop start
    jit_b(j, 4, 0x48, 0x8B, 0x7D, 2 * sizeof(void*)); // mov rdi rbp+16
    SET_FP(var_tsv_len);
    SET_REG_CALL(false, 0); // len in rax
    jit_b(j, 7, 0x48, 0x8B, 0x34, 0x24, 0x48, 0x39, 0xC6); // mov rsi qword ptr [rsp], cmp rsi rax
    jit_b(j, 2, 0x0F, 0x84); // je
    bs = j->len;
    jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after body jmp to end of lop
    jit_var_tsv_gidx(j, false);
    switch (o->od.t) {
        case TYPE(STR):
        case TYPE(SG):
            SET_FP(var_sg_rcd);
            SET_REG_CALL(false, 0);
            jit_var_tsv_gidx(j, false);
            SET_FP(var_sg_f);
            SET_REG_CALL(false, 0)
            break;
        case TYPE(VR):
        case TYPE(TE):
        case TYPE(ST):
            SET_FP(var_tsv_rcd);
            SET_REG_CALL(false, 0);
            jit_var_tsv_gidx(j, false);
            SET_FP(var_tsv_gc);
            SET_REG_CALL(false, 0);
            jit_b(j, 2, 0xFF, 0xD0); // call rax with gc fn
            break;
        case TYPE(ER):
            SET_FP(er_itm_rcd);
            SET_REG_CALL(false, 0);
            jit_var_tsv_gidx(j, false);
            SET_FP(er_itm_gc);
            SET_REG_CALL(false, 0);
            break;
        case TYPE(TD):
            SET_FP(var_td_rcd);
            SET_REG_CALL(false, 0);
            jit_var_tsv_gidx(j, true);
            JIT_TD_GC();
            break;
        default:
            return JIT_ER(m, GCVR_T_INV, o);
    }
    jit_b(j, 5, 0x5E, 0x48, 0xFF, 0xC6, 0x56); // pop rsi, inc rsi, push rsi
    jit_a(j, 0xE9); // jmp
    lope = j->len;
    jit_b(j, 4, 0x00, 0x00, 0x00, 0x00); // filled after lop jmp to start of if
    jmpl = lops - j->len;
    memcpy(j->h + lope, &jmpl, sizeof(int32_t));
    jmpl = j->len - bs - sizeof(int32_t);
    memcpy(j->h + bs, &jmpl, sizeof(int32_t));
    jit_a(j, 0x5E); // pop rsi
    return JIT_ER(m, OK, NULL);
}

int thread_fn(void *volatile arg) {
    var_td *volatile td = (var_td*) arg;
    void *fp;
    uint8_t buf[sizeof(void*)];
    size_t i = 0;
    fn_stk *stk = fn_stk_i(td->m->r->a, FN_STK_SIZE);
    fn_stk_b(td->m->r->a, &stk, td->m->c);
    fn_stk_a(td->m->r->a, &stk, td->m->c);
    jit_i(td->m->r->a, stk->nops + (td->te->len * 10), &td->m->r->j, JIT_FLG(TD));
    jit_stat jstat;
    if ((jstat = jit_stk(td->m, stk, td->m->r->j, false)) != JIT_STAT(OK)) {
        code_p(td->m->c, 0);
        er_p(td->m->r->e);
        printf("THREAD EXIT: %d\n", jstat);
        exit(jstat);
    }
    fn_stk_f(stk);
    jit *j = td->m->r->j;
    jit_fn *jf = (jit_fn*) &j->h[j->len];
    jit_b(j, 4, 0x55, 0x48, 0x89, 0xE5); // push rbp, mov rbp rsp
    for (i = 0; i < td->te->len - 1; i++) {
        SET_REG(td->te, var_tsv*, false, 7);
        SET_REG(i, size_t, false, 6);
        SET_FP(var_tsv_gidx);
        SET_REG_CALL(false, 0);
        jit_a(j, 0x50); // push rax
    }
    SET_REG(td->m->c->jf, jit_fn*, false, 0);
    jit_b(j, 2, 0xFF, 0xD0); // call rax
    jit_b(j, 3, 0x48, 0x89, 0xC2); // mov rdx rax
    SET_REG(td->te, var_tsv*, false, 7);
    i = td->te->len - 1;
    SET_REG(i, size_t, false, 6);
    SET_FP(var_tsv_sidx);
    SET_REG_CALL(false, 0);
    SET_REG(td->m, mod*, false, 7);
    SET_FP(mod_done);
    SET_REG_CALL(false, 0);
    jit_b(j, 2, 0x5D, 0xC3); // pop rbp, ret
    jf(NULL);
    return 0;
}

static var_td *jit_thrd(mod *const m, var_tsv *const te, code *const c) {
    var_td *volatile td = var_td_i(mod_i(m->s, tds_g(m->s, true)), te, c);
    td->m->id = clone(&thread_fn, td->m->r->stkp, CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_IO | CLONE_SIGHAND | SIGCHLD, td);
    nanosleep((const struct timespec[]){{0, 1e6L}}, NULL); // TODO find better way to start threads
    return td;
}

static var join_thrd(var_td *volatile td) {
    //waitpid(td->m->id, NULL, WEXITED);
    sem_wait(&td->m->done);
    return td->te->v[td->te->len - 1];
}

static void deb(mod *const m) {
    printf("?? DEB m: %p ??\n", m);
}

jit_stat jit_code(mod *const m, code *const c, jit_fn *const jf, jit *j, bool done) {
    jit_stat jstat;
    op *o;
    void *fp;
    uint32_t vsp; // var stack pos
    size_t tsvs; // size of tsv
    uint8_t buf[sizeof(void*)];
    fn_stk *stk;
    var vn = (var) { .vd = NULL };
    for (size_t i = 0;  i < c->len; i++) {
        o = &c->ops[i];
        switch (o->oc) {
            case OP_C(EFN):
                op_set_jidx(j, o);
                jit_b(j, 4, 0x55, 0x48, 0x89, 0xE5); // push rbp, mov rbp rsp
                op_set_jlen(j, o);
                break;
            case OP_C(PUSH):
                op_set_jidx(j, o);
                j_push(j, o->od.v.t, o->od.v.id, o->od.v.id);
                op_set_jlen(j, o);
                break;
            case OP_C(POP):
                op_set_jidx(j, o);
                j_pop(j, o->od.v.t, o->od.v.id, o->od.v.id);
                op_set_jlen(j, o);
                break;
            case OP_C(SWAP):
                op_set_jidx(j, o);
                jit_b(j, 2, 0x41, 0x5A); // pop r10
                jit_b(j, 2, 0x41, 0x59); // pop r9
                jit_b(j, 2, 0x41, 0x52); // push r10
                jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(j, o);
                break;
            case OP_C(DONE):
                op_set_jidx(j, o);
                if (done) {
                    SET_REG(m, mod*, false, 7);
                    SET_FP(mod_done);
                    SET_REG_CALL(false, 0);
                }
                op_set_jlen(j, o);
                break;
            case OP_C(RFN):
                op_set_jidx(j, o);
                jit_b(j, 2, 0x5D, 0xC3); // pop rbp, ret
                op_set_jlen(j, o);
                break;
            case OP_C(CFN):
                op_set_jidx(j, o);
                jit_b(j, 3, 0x58, 0xFF, 0xD0); // pop rax, call rax
                op_set_jlen(j, o);
                break;
            case OP_C(CS):
                op_set_jidx(j, o);
                SET_REG(jf, jit_fn*, false, 0);
                jit_b(j, 2, 0xFF, 0xD0); // call rax
                op_set_jlen(j, o);
                break;
            case OP_C(DEB):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_FP(deb);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(AG):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.u3, uint8_t, false, 6);
                SET_FP(mod_ag);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(SG):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.v.id, uint8_t, false, 6);
                j_pop(j, o->od.v.t, 2, 0);
                switch (o->od.v.t) {
                    CT_SET_FN(STR, mod_sg_var_sg);
                    CT_SET_FN(I6, mod_sg_i6);
                    CT_SET_FN(U6, mod_sg_u6);
                    CT_SET_FN(F6, mod_sg_f6);
                    CT_SET_FN(SG, mod_sg_var_sg);
                    CT_SET_FN(VR, mod_sg_var_tsv);
                    CT_SET_FN(TE, mod_sg_var_tsv);
                    CT_SET_FN(ST, mod_sg_var_tsv);
                    CT_SET_FN(FN, mod_sg_jf);
                    CT_SET_FN(FD, mod_sg_fd);
                    CT_SET_FN(ER, mod_sg_er);
                    CT_SET_FN(TD, mod_sg_td);
                    default:
                        return JIT_ER(m, SG_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(LG):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->od.v.id, uint8_t, false, 6);
                switch (o->od.v.t) {
                    CT_SET_FN(STR, mod_lg_var_sg);
                    CT_SET_FN(I6, mod_lg_i6);
                    CT_SET_FN(U6, mod_lg_u6);
                    CT_SET_FN(F6, mod_lg_f6);
                    CT_SET_FN(SG, mod_lg_var_sg);
                    CT_SET_FN(VR, mod_lg_var_tsv);
                    CT_SET_FN(TE, mod_lg_var_tsv);
                    CT_SET_FN(ST, mod_lg_var_tsv);
                    CT_SET_FN(FN, mod_lg_jf);
                    CT_SET_FN(FD, mod_lg_fd);
                    CT_SET_FN(ER, mod_lg_er);
                    CT_SET_FN(TD, mod_lg_td);
                    default:
                        return JIT_ER(m, LG_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                j_push(j, o->od.v.t, 0, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(LM):
                op_set_jidx(j, o);
                stk = fn_stk_i(m->r->a, FN_STK_SIZE);
                fn_stk_b(m->r->a, &stk, o->od.tsvm->m->c);
                fn_stk_a(m->r->a, &stk, o->od.tsvm->m->c);
                jit_i(m->r->a, stk->nops, &o->od.tsvm->m->r->j, 0);
                if (jit_stk(o->od.tsvm->m, stk, o->od.tsvm->m->r->j, true) != JIT_STAT(OK)) return JIT_ER(m, LM_INV, o);
                fn_stk_f(stk);
                SET_REG(o->od.tsvm->m->c->jf, jit_fn*, false, 0);
                jit_b(j, 2, 0xFF, 0xD0); // call rax
                SET_REG(m->r->a, al*, false, 7);
                SET_REG(o->od.tsvm->m, mod*, false, 6);
                SET_REG(o->od.tsvm->gc->jf, jit_fn*, false, 2);
                SET_FP(var_ts_fm);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(AL):
                op_set_jidx(j, o);
                SET_REG(vn, var, true, 1);
                for (uint8_t i = 0; i < o->od.u3; i++) jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(j, o);
                break;
            case OP_C(SL):
                op_set_jidx(j, o);
                jit_b(j, 2, 0x41, 0x59); // pop r9
                vsp = -(o->od.v.id + 1) * sizeof(void*);
                jit_b(j, 4, 0x4C, 0x89, 0x4D, vsp); // mov qword ptr rbp-vsp r9
                op_set_jlen(j, o);
                break;
            case OP_C(LL):
                op_set_jidx(j, o);
                vsp = -(o->od.v.id + 1) * sizeof(void*);
                jit_b(j, 4, 0x4C, 0x8B, 0x4D, vsp); // mov r9 qword ptr rbp-vsp
                jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(j, o);
                break;
            case OP_C(SA):
                op_set_jidx(j, o);
                jit_b(j, 2, 0x41, 0x59); // pop r9
                vsp = (o->od.v.id + 2) * sizeof(void*);
                jit_b(j, 4, 0x4C, 0x89, 0x4D, vsp); // mov rbp+vsp r9
                op_set_jlen(j, o);
                break;
            case OP_C(LA):
                op_set_jidx(j, o);
                vsp = (o->od.v.id + 2) * sizeof(void*);
                jit_b(j, 4, 0x4C, 0x8B, 0x4D, vsp); // mov r9 rbp+vsp
                jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(j, o);
                break;
            case OP_C(PV):
                op_set_jidx(j, o);
                switch (o->ot) {
                    case TYPE(STR):
                        SET_REG(m->r->a, al*, false, 7);
                        SET_REG(o->od.sg, char*, false, 6);
                        SET_FP(var_sg_i_str);
                        SET_REG_CALL(false, 0);
                        jit_a(j, 0x50); // push rax
                        op_set_jlen(j, o);
                        break;
                    // TODO
                    OPPVT(U6, u6, uint64_t);
                    // TODO
                    OPPVT(I6, i6, int64_t);
                    // TODO
                    OPPVT(F6, f6, double);
                    OPPVT(SG, sg, var_sg*);
                    OPPVT(FN, c->jf, jit_fn*);
                    OPPVT(FD, fd, int);
                    OPPVT(TD, c, code*);
                    default:
                        return JIT_ER(m, PV_T_INV, o);
                }
                break;
            case OP_C(CTSV):
                op_set_jidx(j, o);
                SET_REG(m->r->a, al*, false, 7);
                tsvs = o->od.tsvm->len;
                if (o->ot == TYPE(VR)) tsvs *= TSVML;
                SET_REG(tsvs, size_t, false, 6);
                SET_REG(o->od.tsvm->len, size_t, false, 2);
                SET_REG(o->od.tsvm->gc->jf, jit_fn*, false, 1);
                SET_FP(var_tsv_i);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                for (ssize_t i = (ssize_t) o->od.tsvm->len - 1; i >= 0; i--) { // TODO better algo for underflow
                    jit_a(j, 0x5F); // pop rdi
                    SET_REG(i, ssize_t, false, 6);
                    jit_a(j, 0x5A); // pop rdx
                    jit_a(j, 0x57); // push rdi
                    SET_FP(var_tsv_sidx);
                    SET_REG_CALL(false, 0);
                }
                op_set_jlen(j, o);
                break;
            case OP_C(GIDX):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_REG(o->od.u6, size_t, false, 6);
                SET_FP(var_tsv_gidx);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(SIDX):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_REG(o->od.u6, size_t, false, 6);
                jit_a(j, 0x5A); // pop rdx
                SET_FP(var_tsv_sidx);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(VRA):
                op_set_jidx(j, o);
                SET_REG(m->r->a, al*, false, 7);
                jit_a(j, 0x5A); // pop rdx value
                jit_b(j, 4, 0x48, 0x8B, 0x34, 0x24); // mov rsi qword ptr [rsp]
                SET_FP(var_tsv_add);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(VRS):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->a, ast*, false, 6);
                jit_b(j, 4, 0x48, 0x8B, 0x14, 0x24); // mov rdx qword ptr [rsp]
                SET_FP(var_tsv_sub);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(IF):
                op_set_jidx(j, o);
                if ((jstat = jit_if(m, o->od.c, jf, j)) != JIT_STAT(OK)) return jstat;
                op_set_jlen(j, o);
                break;
            case OP_C(LOP):
                op_set_jidx(j, o);
                if ((jstat = jit_lop(m, o->od.of, jf, j)) != JIT_STAT(OK)) return jstat;
                op_set_jlen(j, o);
                break;
            case OP_C(COND):
                op_set_jidx(j, o);
                if ((jstat = jit_cond(m, o->od.of, jf, j)) != JIT_STAT(OK)) return jstat;
                op_set_jlen(j, o);
                break;
            case OP_C(ZOO):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                switch (o->od.t) {
                    CT_SET_FN(U6, var_zoo_u6);
                    CT_SET_FN(I6, var_zoo_i6);
                    CT_SET_FN(ER, var_zoo_er);
                    default:
                        return JIT_ER(m, ZOO_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(TE):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->a, ast*, false, 6);
                jit_a(j, 0x5A); // pop rdx
                SET_FP(var_sg_er);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(RTE):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(o->a, ast*, false, 6);
                jit_a(j, 0x5A); // pop rdx
                SET_FP(er_rt);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(CE):
                op_set_jidx(j, o);
                SET_REG(m->r->a, al*, false, 7);
                SET_REG(m->r->e, er*, false, 6);
                SET_FP(er_g);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(PE):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                SET_REG(m->r->e, er*, false, 6);
                SET_REG(o->a->t.lno, size_t, false, 2);
                SET_REG(o->a->t.cno, size_t, false, 1);
                SET_FP(er_e);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            // TODO
            case OP_C(CSTSG):
                op_set_jidx(j, o);
                SET_REG(m->r->a, al*, false, 7);
                j_pop(j, o->od.t, 6, 0);
                switch (o->od.t) {
                    CT_SET_FN(U6, var_u6_sg);
                    CT_SET_FN(I6, var_i6_sg);
                    CT_SET_FN(F6, var_f6_sg);
                    default:
                        return JIT_ER(m, CSTSG_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            C_OP_C_BOP(ADD, add);
            C_OP_C_BOP(SUB, sub);
            C_OP_C_BOP(MUL, mul);
            C_OP_C_BOP(DIV, div);
            C_OP_C_BOP(EQ, eq);
            case OP_C(NOT):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_FP(var_not);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            // TODO
            C_OP_C_BOP(GT, gt);
            C_OP_C_BOP(LT, lt);
            C_OP_C_BOP(OR, or);
            case OP_C(CNCTSG):
                op_set_jidx(j, o);
                SET_REG(m->r->a, al*, false, 7);
                jit_b(j, 5, 0x48, 0x8B, 0x74, 0x24, 0x08); // mov rsi qword ptr [rsp+8] sg
                jit_b(j, 4, 0x48, 0x8B, 0x14, 0x24); // mov rdx qword ptr [rsp] sg/te
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_cnct_sg_sg);
                        break;
                    case TYPE(TE):
                    case TYPE(VR):
                        SET_FP(var_sg_cnct_sg_tsv);
                        break;
                    default:
                        return JIT_ER(m, CNCTSG_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                jit_b(j, 2, 0x41, 0x5A); // pop r10 tgt sg/te
                jit_b(j, 2, 0x41, 0x59); // pop r9 cnct sg
                jit_a(j, 0x50); // push rax
                jit_b(j, 2, 0x41, 0x52); // push r10
                jit_b(j, 2, 0x41, 0x51); // push r9
                op_set_jlen(j, o);
                break;
            case OP_C(WFD):
                op_set_jidx(j, o);
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                        SET_FP(var_sg_str);
                        SET_REG_CALL(false, 0);
                        jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                        jit_a(j, 0x50); // push rax
                        SET_FP(var_sg_len);
                        SET_REG_CALL(false, 0);
                        jit_b(j, 3, 0x48, 0x89, 0xC2); // mov rdx, rax
                        jit_a(j, 0x5E); // str in rsi
                        break;
                    default:
                        return JIT_ER(m, WFD_T_INV, o);
                }
                jit_b(j, 2, 0x41, 0x5A); // pop r10
                jit_a(j, 0x5F); // fd in rdi
                jit_b(j, 2, 0x41, 0x52); // push r10
                SET_FP(write);
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(TDI):
                op_set_jidx(j, o);
                SET_REG(m, mod*, false, 7);
                jit_a(j, 0x5A); // pop rdx fn
                jit_a(j, 0x5E); // pop rsi te
                SET_FP(jit_thrd);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(TDJ):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_FP(join_thrd);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(NK):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                SET_FP(var_nvd);
                SET_REG_CALL(false, 0);
                jit_a(j, 0x50); // push rax
                op_set_jlen(j, o);
                break;
            case OP_C(RCI):
                op_set_jidx(j, o);
                jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_rci);
                        break;
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                        SET_FP(var_tsv_rci);
                        break;
                    case TYPE(ER):
                        SET_FP(er_itm_rci);
                        break;
                    case TYPE(TD):
                        SET_FP(var_td_rci);
                        break;
                    default:
                        return JIT_ER(m, RCI_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(RCD):
                op_set_jidx(j, o);
                jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_rcd);
                        break;
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                        SET_FP(var_tsv_rcd);
                        break;
                    case TYPE(ER):
                        SET_FP(er_itm_rcd);
                        break;
                     case TYPE(TD):
                        SET_FP(var_td_rcd);
                        break;
                    default:
                        return JIT_ER(m, RCD_T_INV, o);
                }
                SET_REG_CALL(false, 0);
                op_set_jlen(j, o);
                break;
            case OP_C(RCF):
                op_set_jidx(j, o);
                jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                    case TYPE(TD):
                        break;
                    default:
                        return JIT_ER(m, RCF_T_INV, o);
                }
                SET_REG(o->od.t, type, false, 6);
                SET_FP(var_rcf);
                SET_REG_CALL(false, 0);
                jit_b(j, 3, 0x4D, 0x31, 0xE4); // xor r12 12
                jit_b(j, 3, 0x4C, 0x39, 0xE0); // cmp rax r12
                jit_b(j, 4, 0x7E, 0x02, 0xC9, 0xC3); // jng 2, leave, ret
                op_set_jlen(j, o);
                break;
            case OP_C(GC):
                op_set_jidx(j, o);
                switch (o->od.t) {
                    case TYPE(U3):
                    case TYPE(U4):
                    case TYPE(U5):
                    case TYPE(U6):
                    case TYPE(I3):
                    case TYPE(I4):
                    case TYPE(I5):
                    case TYPE(I6):
                    case TYPE(F5):
                    case TYPE(F6):
                    case TYPE(FN):
                        jit_a(j, 0x5F); // pop rdi
                        break;
                    case TYPE(STR):
                    case TYPE(SG):
                        jit_a(j, 0x5F); // pop rdi
                        SET_FP(var_sg_f);
                        SET_REG_CALL(false, 0);
                        break;
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                        jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                        SET_FP(var_tsv_gc);
                        SET_REG_CALL(false, 0);
                        jit_b(j, 2, 0xFF, 0xD0); // call rax with gc fn
                        jit_a(j, 0x5F); // pop rdi
                        break;
                    case TYPE(ER):
                        jit_a(j, 0x5F); // pop rdi
                        SET_FP(er_itm_gc);
                        SET_REG_CALL(false, 0);
                        break;
                    case TYPE(TD):
                        JIT_TD_GC();
                        break;
                    default:
                        return JIT_ER(m, GC_T_INV, o);
                }
                op_set_jlen(j, o);
                break;
            case OP_C(GCTSVI):
                op_set_jidx(j, o);
                switch (o->od.v.t) {
                    case TYPE(U3):
                    case TYPE(U4):
                    case TYPE(U5):
                    case TYPE(U6):
                    case TYPE(I3):
                    case TYPE(I4):
                    case TYPE(I5):
                    case TYPE(I6):
                    case TYPE(F5):
                    case TYPE(F6):
                    case TYPE(FN):
                        break;
                    case TYPE(STR):
                    case TYPE(SG):
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                    case TYPE(ER):
                    case TYPE(TD):
                        jit_b(j, 4, 0x48, 0x8B, 0x3C, 0x24); // mov rdi qword ptr [rsp]
                        SET_REG(o->od.v.id, uint8_t, false, 6);
                        SET_FP(var_tsv_gidx);
                        SET_REG_CALL(false, 0);
                        break;
                    default:
                        return JIT_ER(m, GCTSV_T_INV, o);
                }
                switch (o->od.v.t) {
                    case TYPE(U3):
                    case TYPE(U4):
                    case TYPE(U5):
                    case TYPE(U6):
                    case TYPE(I3):
                    case TYPE(I4):
                    case TYPE(I5):
                    case TYPE(I6):
                    case TYPE(F5):
                    case TYPE(F6):
                    case TYPE(FN):
                        break;
                    case TYPE(TD):
                        jit_a(j, 0x50); // push rax
                        break;
                    default:
                        jit_b(j, 3, 0x48, 0x89, 0xC7); // mov rdi rax
                        break;
                }
                switch (o->od.v.t) {
                    case TYPE(U3):
                    case TYPE(U4):
                    case TYPE(U5):
                    case TYPE(U6):
                    case TYPE(I3):
                    case TYPE(I4):
                    case TYPE(I5):
                    case TYPE(I6):
                    case TYPE(F5):
                    case TYPE(F6):
                    case TYPE(FN):
                        break;
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_f);
                        SET_REG_CALL(false, 0)
                        break;
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                        SET_FP(var_tsv_gc);
                        SET_REG_CALL(false, 0);
                        jit_b(j, 2, 0xFF, 0xD0); // call rax with gc fn
                        break;
                    case TYPE(ER):
                        SET_FP(er_itm_gc);
                        SET_REG_CALL(false, 0);
                        break;
                    case TYPE(TD):
                        JIT_TD_GC();
                        break;
                    default:
                        return JIT_ER(m, GCTSV_T_INV, o);
                }
                op_set_jlen(j, o);
                break;
            case OP_C(GCVR):
                op_set_jidx(j, o);
                if ((jstat = jit_gc_vr(m, o, j)) != JIT_STAT(OK)) return jstat;
                op_set_jlen(j, o);
                break;
            case OP_C(DEL):
                op_set_jidx(j, o);
                jit_a(j, 0x5F); // pop rdi
                switch (o->od.t) {
                    case TYPE(STR):
                    case TYPE(SG):
                        SET_FP(var_sg_d);
                        SET_REG_CALL(false, 0)
                        break;
                    case TYPE(VR):
                    case TYPE(TE):
                    case TYPE(ST):
                        SET_FP(var_tsv_d);
                        SET_REG_CALL(false, 0);
                        break;
                    case TYPE(ER):
                        SET_FP(er_itm_d);
                        SET_REG_CALL(false, 0);
                        break;
                    case TYPE(TD):
                        SET_FP(var_td_d);
                        SET_REG_CALL(false, 0);
                        break;
                    default:
                        return JIT_ER(m, DEL_T_INV, o);
                }
                op_set_jlen(j, o);
                break;
            default:
                return JIT_ER(m, INV_CODE, o);
        }
    }
    return JIT_ER(m, OK, NULL);
}

jit_stat jit_stk(mod *const m, fn_stk *const stk, jit *j, bool done) {
    jit_stat jstat;
    for (size_t i = 0; i < stk->len; i++) {
        stk->fn[i]->jf = (jit_fn*) &j->h[j->len];
        if ((jstat = jit_code(m, stk->fn[i], stk->fn[i]->jf, j, done && i == stk->len - 1)) != JIT_STAT(OK)) return jstat;
    }
    return JIT_ER(m, OK, NULL);
}
