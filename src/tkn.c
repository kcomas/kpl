
#include "tkn.h"

void tkn_p(tbl *tl, size_t idnt) {
    te *h = tl->i->h;
    while (h) {
        for (size_t i = 0; i < idnt; i++) putchar(' ');
        te *kv = h->d[0].p;
        c4 c = kv->d[0].c;
        printf("%c%c%c%c,", c.a, c.b, c.c, c.d);
        printf("%u", kv->d[1].u4);
        putchar('\n');
        if (kv->d[3].p) tkn_p(kv->d[3].p, idnt + 1);
        h = h->d[2].p;
    }
}

void tkn_m_p(const te *m, const mc *s) {
        printf("tid:%u,lno:%u,cno:%u,start:%u,end:%u,str:", tkn_m_g_i(m), tkn_m_g_l(m), tkn_m_g_c(m), tkn_m_g_s(m), tkn_m_g_e(m));
    for (uint16_t i = tkn_m_g_s(m); i < tkn_m_g_e(m); i++) putchar(s->d[i]);
}

size_t tkn_sh(un v) {
    return v.c.a + v.c.b + v.c.c + v.c.d;
}

tkn_stat tkn_df(tkn *t, te *m, err **e) {
    t->cno = tkn_m_g_c(m);
    t->pos = tkn_m_g_s(m);
    size_t ce = 0;
    un c = c4_g((char*) t->s->d, t->pos, &ce);
    if (!isalnum(c.c.a)) return tkn_err(t, e, "invalid var");
    while (isalnum(c.c.a)) {
        t->cno++;
        t->pos = ce + 1;
        c = c4_g((char*) t->s->d, t->pos, &ce);
    }
    tkn_m_s_i(m, TCUST(VAR));
    tkn_m_s_e(m, t->pos);
    return TKN_STAT(OK);
}

tkn_stat tkn_num(tkn *t, te *m, err **e) {
    (void) e;
    while (isdigit(t->s->d[t->pos])) {
        t->cno++;
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    }
    return TKN_STAT(OK);
}

tkn_stat tkn_nl(tkn *t, te *m, err **e) {
    (void) m;
    (void) e;
    t->lno++;
    t->cno = 1;
    return TKN_STAT(OK);
}

tkn_stat tkn_ft(tkn *t, te *m, err **e) {
    (void) t;
    (void) m;
    (void) e;
    return TKN_STAT(OK);
}

#define TKN_SKIP(N, C) tkn_stat tkn_##N(tkn *t, te *m, err **e) { \
    (void) e; \
    while (t->s->d[t->pos] == C) { \
        t->cno++; \
        t->pos++; \
        tkn_m_s_e(m, tkn_m_g_e(m) + 1); \
    } \
    return TKN_STAT(OK); \
}

TKN_SKIP(ws, ' ');
TKN_SKIP(tab, '\t');

tkn_stat tkn_cmt(tkn *t, te *m, err **e) {
    while (t->s->d[t->pos] != '\n' && t->s->d[t->pos] != '\0') {
        t->cno++;
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    }
    if (t->s->d[t->pos] == '\0') return tkn_err(t, e, "tkn cmt no nl");
    tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    return TKN_STAT(OK);
}

tkn_stat tkn_sym(tkn *t, te *m, err **e) {
    size_t ce = 0;
    un c = c4_g((char*) t->s->d, t->pos, &ce); // `
    c = c4_g((char*) t->s->d, t->pos, &ce);
    if (!isalnum(c.c.a)) return tkn_err(t, e, "invalid sym`");
    while (isalnum(c.c.a)) {
        t->cno++;
        t->pos = ce + 1;
        c = c4_g((char*) t->s->d, t->pos, &ce);
    }
    if (t->pos - tkn_m_g_s(m) < 2) return tkn_err(t, e, "invalid sym` len");
    tkn_m_s_e(m, t->pos);
    return TKN_STAT(OK);
}

tkn_stat tkn_str(tkn *t, te *m, err **e) {
    while (t->s->d[t->pos] != '"' && t->s->d[t->pos] != '\0') {
        t->cno++;
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    }
    if (t->s->d[t->pos] == '\0') return tkn_err(t, e, "tkn sg inv");
    t->cno++;
    t->pos++;
    tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    return TKN_STAT(OK);
}

tkn_stat tkn_regt(tkn *t, te *m, err **e) {
    while (t->s->d[t->pos] != '~' && t->s->d[t->pos] != '\0') {
        t->cno++;
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    }
    if (t->s->d[t->pos] == '\0') return tkn_err(t, e, "tkn regt inv");
    t->pos++;
    t->cno++;
    tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    while (isalpha(t->s->d[t->pos])) {
        t->cno++;
        t->pos++;
        tkn_m_s_e(m, tkn_m_g_e(m) + 1);
    }
    return TKN_STAT(OK);
}

tkn *tkn_b(tkn *t) {
    tkn_a(t, TCUST(NL), "\n", tkn_nl);
    tkn_a(t, TCUST(SEMI), ";", tkn_ft);
    tkn_a(t, TCUST(WS), " ", tkn_ws);
    tkn_a(t, TCUST(TAB), "\t", tkn_tab);
    tkn_a(t, TCUST(NUM), "0", tkn_num);
    tkn_a(t, TCUST(NUM), "1", tkn_num);
    tkn_a(t, TCUST(NUM), "2", tkn_num);
    tkn_a(t, TCUST(NUM), "3", tkn_num);
    tkn_a(t, TCUST(NUM), "4", tkn_num);
    tkn_a(t, TCUST(NUM), "5", tkn_num);
    tkn_a(t, TCUST(NUM), "6", tkn_num);
    tkn_a(t, TCUST(NUM), "7", tkn_num);
    tkn_a(t, TCUST(NUM), "8", tkn_num);
    tkn_a(t, TCUST(NUM), "9", tkn_num);
    tkn_a(t, TCUST(SYM), "`", tkn_sym);
    tkn_a(t, TCUST(STR), "\"", tkn_str);
    tkn_a(t, TCUST(DOT), ".", tkn_ft);
    tkn_a(t, TCUST(REGT), "t~", tkn_regt);
    return t;
}

tkn_stat tkn_g_i6(const te *t, const mc *s, int64_t *i) {
    uint64_t v = 0;
    for(uint32_t p = tkn_m_g_s(t); p < tkn_m_g_e(t); p++) {
        char c = s->d[p];
        if (c < '0' || c > '9') return TKN_STAT(INV);
        v *= 10;
        v += s->d[p] - '0';
        if (v > INT64_MAX) return TKN_STAT(INV);
    }
    *i = v;
    return TKN_STAT(OK);
}

#define MAX_F6 21

tkn_stat tkn_g_f6(const te *restrict tu, const te *restrict tm, const te *restrict tl, const mc *s, double *d) {
    uint32_t pos = 0;
    char buf[MAX_F6];
    const te *uml[] = {tu, tm, tl};
    for (size_t i = 0; i < 3; i++) {
        uint32_t ss = tkn_m_g_s(uml[i]);
        uint32_t ee = tkn_m_g_e(uml[i]);
        if (pos + ee - ss >= MAX_F6) return TKN_STAT(INV);
        memcpy(buf + pos, s->d + ss, ee - ss);
        pos += ee - ss;
    }
    buf[pos] = '\0';
    char *ep;
    *d = strtod(buf, &ep);
    if (*d == 0 && errno == ERANGE) return TKN_STAT(INV);
    return TKN_STAT(OK);
}

#define MC_PAD 10

static tkn_stat _tkn_g_mc(const mc *s, ssize_t start, ssize_t end, const alfr *af, mc **v) {
    if (start < 0 || end < 0 || start > end) return TKN_STAT(INV);
    size_t l = end - start + MC_PAD;
    *v = mc_i(l, af);
    while (start < end) {
        if (s->d[start] == '\\') {
            start++;
            switch (s->d[start]) {
                case 'e':
                    mc_wa(v, '\e');
                    break;
                case 'n':
                    mc_wa(v, '\n');
                    break;
                case '\\':
                    mc_wa(v, '\\');
                    break;
                default:
                    return TKN_STAT(INV);
            }
        } else mc_wa(v, s->d[start]);
        start++;
    }
    mc_wa(v, '\0');
    return TKN_STAT(OK);
}

tkn_stat tkn_g_mc(const te *t, const mc *s, ssize_t off, const alfr *af, mc **v) {
    ssize_t start = tkn_m_g_s(t);
    ssize_t end = tkn_m_g_e(t);
    if (off > 0) start += off;
    else end += off;
    return _tkn_g_mc(s, start, end, af, v);
}

tkn_stat tkn_g_str(const te *t, const mc *s, const alfr *af, mc **v) {
    return _tkn_g_mc(s, tkn_m_g_s(t) + 1, tkn_m_g_e(t) - 1, af, v);
}

const char *reg_mode_str(reg_mode m) {
    const char *modes[] = {
        "N",
        "T"
    };
    if (m < REG_MODE(_)) return modes[m];
    return "REG MODE INV";
}

tkn_stat tkn_g_reg_mode(const te *t, const mc *s, uint16_t *mode) {
    uint16_t id = tkn_m_g_i(t);
    if (id < TCUST(REGT) || id > TCUST(REGS)) return TKN_STAT(INV);
    ssize_t start = tkn_m_g_s(t);
    switch (s->d[start]) {
        case 't':
            *mode = REG_MODE(T);
            break;
        default:
            return TKN_STAT(INV);
    }
    return TKN_STAT(OK);
}

const char *reg_flg_str[] = {
    "I"
};

tkn_stat tkn_g_reg_flg(const te *t, const mc *s, uint16_t *flgs) {
    uint16_t id = tkn_m_g_i(t);
    if (id < TCUST(REGT) || id > TCUST(REGS)) return TKN_STAT(INV);
    uint32_t f = 0;
    ssize_t end = tkn_m_g_e(t) - 1;
    while (s->d[end] != '~') {
        switch (s->d[end--]) {
            case 'i':
                if (f & REG_FLG(I)) return TKN_STAT(INV);
                f |= REG_FLG(I);
                break;
            default:
                return TKN_STAT(INV);
        }
    }
    *flgs = f;
    return TKN_STAT(OK);
}

tkn_stat tkn_g_reg_mtch(const te *t, const mc *s, const alfr *af, mc **m) {
    uint16_t id = tkn_m_g_i(t);
    if (id != TCUST(REGT) && id != TCUST(REGM)) return TKN_STAT(INV);
    ssize_t start = tkn_m_g_s(t) + 2;
    *m = mc_i(tkn_m_g_e(t) - start + 5, af);
    while (s->d[start] != '~') mc_wa(m, s->d[start++]);
    mc_wa(m, '\0');
    return TKN_STAT(OK);
}
