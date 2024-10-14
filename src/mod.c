
#include "mod.h"

static const char *const mds[] = {
    "OK",
    "FLF"
};

const char *mod_stat_str(mod_stat ms) {
    const char *s = "INVALID_MOD_STAT";
    if (ms >= MOD_STAT(OK) && ms <= MOD_STAT(FLF)) s = mds[ms];
    return s;
}

extern inline mod *mod_i(tds *const s, tdr *const r);

void mod_done(mod *const m) {
    m->done = true;
}

extern inline mod_stat mod_er(mod *const m, const char *const fnn, mod_stat ms);

#define MOD_ER(M, MS) mod_er(m, __func__, MOD_STAT(MS))

static mod_stat _mod_lfile(mod *const m) {
    int fd = open(m->src.path, O_RDONLY);
    if (fd == -1) return MOD_ER(m, FLF);
    if (fstat(fd, &m->src.sb) == -1) {
        close(fd);
        return MOD_ER(m, FLF);
    }
    m->src.str = ala(m->r->a, m->src.sb.st_size * sizeof(char) + sizeof(char));
    if (read(fd, m->src.str, m->src.sb.st_size) != m->src.sb.st_size) {
        alf(m->src.str);
        close(fd);
        return MOD_ER(m, FLF);
    }
    close(fd);
    return MOD_ER(m, OK);
}

mod_stat mod_lfile(mod *const m, const char *const path) {
    m->src.path = ala(m->r->a, strlen(path) * sizeof(char) + sizeof(char));
    strcpy(m->src.path, path);
    return _mod_lfile(m);
}

mod_stat mod_lfile_tkn(mod *const m, const char *const dirs, size_t dirl, const tkn *const t) {
    m->src.path = ala(m->r->a, dirl + (t->len - 1) * sizeof(char)); // null byte
    memcpy(m->src.path, dirs, dirl);
    memcpy(m->src.path + dirl, t->str + t->pos + 1, t->len - 2);
    return _mod_lfile(m);
}

void mod_ag(mod *const m, uint8_t ng) {
    m->g = ala(m->r->a, sizeof(var) * ng);
    m->ng = ng;
}

#define MOD_SG_FN(CT, T) MOD_SG(CT, T) { m->g[i].T = T; }
#define MOD_LG_FN(CT, T) MOD_LG(CT, T) { return m->g[i].T; }

MOD_SG_FN(int64_t, i6)
MOD_LG_FN(int64_t, i6)

MOD_SG_FN(uint64_t, u6)
MOD_LG_FN(uint64_t, u6)

MOD_SG_FN(int, fd);
MOD_LG_FN(int, fd);

void mod_sg_var_sg(mod *const m, uint8_t i, var_sg *const sg) { m->g[i].sg = sg; }
var_sg *mod_lg_var_sg(mod *const m, uint8_t i) { return m->g[i].sg; }

void mod_sg_var_tsv(mod *const m, uint8_t i, var_tsv *const tsv) { m->g[i].tsv = tsv; }
var_tsv *mod_lg_var_tsv(mod *const m, uint8_t i) { return m->g[i].tsv; }

void mod_sg_jf(mod *const m, uint8_t i, jit_fn *const jf) { m->g[i].jf = jf; }
jit_fn *mod_lg_jf(mod *const m, uint8_t i) { return m->g[i].jf; }

void mod_sg_er(mod *const m, uint8_t i, er_itm *const ei) { m->g[i].ei = ei; }
er_itm *mod_lg_er(mod *const m, uint8_t i) { return m->g[i].ei; }

void mod_sg_td(mod *const m, uint8_t i, var_td *const td) { m->g[i].td = td; }
var_td *mod_lg_td(mod *const m, uint8_t i) { return m->g[i].td; }

extern inline void mod_psrc(const mod *const m);

extern inline void mod_f(mod *m);
