
#pragma once

#include "kpl.h"
#include "var.h"

#define MOD_STAT(N) MOD_STAT_##N

typedef enum {
    MOD_STAT(OK),
    MOD_STAT(FLF) // file load fail
} mod_stat;

typedef struct {
    uint8_t ng; // number of globals
    struct {
        struct stat sb;
        char *path, *str;
    } src;
    fn_node *fns; // ast root
    code *c;
    jit *j;
    var *g; // globals
} mod;

inline mod *mod_i(void) {
    return calloc(1, sizeof(mod));
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
    FNN(m->src.path);
    FNN(m->src.str);
    // TODO use fn scope symtbl to free globals
    free(m->g);
    free(m);
}
