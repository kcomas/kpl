
#pragma once

#include "kpl.h"
#include "var.h"
#include "er.h"

#define MOD_STAT(N) MOD_STAT_##N

typedef enum {
    MOD_STAT(OK),
    MOD_STAT(FLF) // file load fail
} mod_stat;

const char *mod_stat_str(mod_stat ms);

typedef struct {
    uint8_t ng; // number of globals
    struct {
        struct stat sb;
        char *path, *str;
    } src;
    al *a; // allocator
    er *e; // error
    fn_node *fns; // ast root
    code *c;
    jit *j;
    var *g; // globals
} mod;

inline mod *mod_i(al *const a, er *const e) {
    mod *m = ala(a, sizeof(mod));
    m->a = a;
    m->e = e;
    return m;
}

inline mod_stat mod_er(mod *const m, const char *const fnn, mod_stat ms) {
    if (ms == MOD_STAT(OK)) return ms;
    er_itm *ei = er_itm_i(m->a, ER(MOD), fnn, mod_stat_str(ms));
    ei->path = m->src.path;
    er_a(m->e, ei);
    return ms;
}

// load file
mod_stat mod_lfile(mod *const m, const char *const path);

// allocate globals
void mod_ag(mod *const m, uint8_t ng);

#define MOD_SG(CT, T) void mod_sg_##T(mod *const m, uint8_t i, CT T)
#define MOD_LG(CT, T) CT mod_lg_##T(mod *const m, uint8_t i)

MOD_SG(int64_t, i6);
MOD_LG(int64_t, i6);

MOD_SG(uint64_t, u6);
MOD_LG(uint64_t, u6);

MOD_SG(int, fd);
MOD_LG(int, fd);

void mod_sg_var_sg(mod *const m, uint8_t i, var_sg *const sg);
var_sg *mod_lg_var_sg(mod *const m, uint8_t i);

void mod_sg_jf(mod *const m, uint8_t i, jit_fn *const jf);
jit_fn *mod_lg_jf(mod *const m, uint8_t i);

// print src
inline void mod_psrc(const mod *const m) {
    if (m->src.path != NULL) printf("%s\n", m->src.path);
    printf("%s", m->src.str);
}

inline void mod_f(mod *m) {
    FNNF(m->src.path, alf);
    FNNF(m->src.str, alf);
    FNNF(m->g, alf);
    alf(m);
}
