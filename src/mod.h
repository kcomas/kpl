
#pragma once

#include "kpl.h"
#include "var.h"
#include "er.h"
#include "tkn.h"
#include "td.h"

#define MOD_STAT(N) MOD_STAT_##N

typedef enum {
    MOD_STAT(OK),
    MOD_STAT(FLF) // file load fail
} mod_stat;

const char *mod_stat_str(mod_stat ms);

inline mod *mod_i(tds *const s, tdr *const r) {
    mod *m = ala(r->a, sizeof(mod));
    m->s = s;
    m->r = r;
    m->donep = &m->done;
    sem_init(m->donep, -1, 0);
    return m;
}

inline mod_stat mod_er(mod *const m, const char *const fnn, mod_stat ms) {
    if (ms == MOD_STAT(OK)) return ms;
    er_itm *ei = er_itm_i(m->r->a, ER(MOD), fnn, mod_stat_str(ms));
    ei->path = m->src.path;
    er_a(m->r->e, ei);
    return ms;
}

// load file
mod_stat mod_lfile(mod *const m, const char *const path);

// load file from tkn
mod_stat mod_lfile_tkn(mod *const m, const char *const dirs, size_t dirl, const tkn *const t);

// allocate globals
void mod_ag(mod *const m, uint8_t ng);

#define MOD_SG(CT, T) void mod_sg_##T(mod *const m, uint8_t i, CT T)
#define MOD_LG(CT, T) CT mod_lg_##T(mod *const m, uint8_t i)

MOD_SG(int64_t, i6);
MOD_LG(int64_t, i6);

MOD_SG(uint64_t, u6);
MOD_LG(uint64_t, u6);

MOD_SG(double, f6);
MOD_LG(double, f6);

MOD_SG(int, fd);
MOD_LG(int, fd);

void mod_sg_var_sg(mod *const m, uint8_t i, var_sg *const sg);
var_sg *mod_lg_var_sg(mod *const m, uint8_t i);

void mod_sg_var_tsv(mod *const m, uint8_t i, var_tsv *const tsv);
var_tsv *mod_lg_var_tsv(mod *const m, uint8_t i);

void mod_sg_jf(mod *const m, uint8_t i, jit_fn *const jf);
jit_fn *mod_lg_jf(mod *const m, uint8_t i);

void mod_sg_er(mod *const m, uint8_t i, er_itm *const ei);
er_itm *mod_lg_er(mod *const m, uint8_t i);

void mod_sg_var_hh(mod *const m, uint8_t i, var_hh *const hh);
var_hh *mod_lg_var_hh(mod *const m, uint8_t i);

void mod_sg_td(mod *const m, uint8_t i, var_td *const td);
var_td *mod_lg_td(mod *const m, uint8_t i);

// print src
inline void mod_psrc(const mod *const m) {
    if (m->src.path != NULL) printf("%s\n", m->src.path);
    printf("%s", m->src.str);
}
