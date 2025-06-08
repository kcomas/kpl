
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

extern inline mod *mod_i(al *const a, er *const e);

mod_stat mod_er(mod *const m, mod_stat ms) {
    if (ms == MOD_STAT(OK)) return ms;
    er_itm *ei = er_itm_i(m->a, ER(MOD), mod_stat_str(ms));
    ei->path = m->src.path;
    er_a(m->e, ei);
    return ms;
}

mod_stat mod_lfile(mod *const m, const char *const path) {
    m->src.path = ala(m->a, strlen(path) * sizeof(char) + sizeof(char));
    strcpy(m->src.path, path);
    int fd = open(path, O_RDONLY);
    if (fd == -1) return mod_er(m, MOD_STAT(FLF));
    if (fstat(fd, &m->src.sb) == -1) {
        close(fd);
        return mod_er(m, MOD_STAT(FLF));
    }
    m->src.str = ala(m->a, m->src.sb.st_size * sizeof(char) + sizeof(char));
    if (read(fd, m->src.str, m->src.sb.st_size) != m->src.sb.st_size) {
        alf(m->src.str);
        close(fd);
        return mod_er(m, MOD_STAT(FLF));
    }
    close(fd);
    return mod_er(m, MOD_STAT(OK));
}

void mod_ag(mod *const m, uint8_t ng) {
    m->g = ala(m->a, sizeof(var) * ng);
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

void mod_sg_var_sg(mod *const m, uint8_t i, var_sg *const sg) { var_sg_rci(sg); m->g[i].sg = sg; }
var_sg *mod_lg_var_sg(mod *const m, uint8_t i) { var_sg_rci(m->g[i].sg); return m->g[i].sg; }

void mod_sg_jf(mod *const m, uint8_t i, jit_fn *const jf) { m->g[i].jf = jf; }
jit_fn *mod_lg_jf(mod *const m, uint8_t i) { return m->g[i].jf; }

extern inline void mod_psrc(const mod *const m);

extern inline void mod_f(mod *m);
