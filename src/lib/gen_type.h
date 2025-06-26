
#pragma once

#include "type.h"
#include "gen_x64.h"

void* type_ref_g_des(type t);

// p->af->f(p)
gen_stat gen_type_aff(gen *g, te *t, err **e, const char *pf);

// if valid check if fn is not null, then check gen
gen_stat gen_type_des(gen *bg, te *t, uint32_t *lc, gen **g, void **fn, err **e);
