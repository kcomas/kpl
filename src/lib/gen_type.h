
#pragma once

#include "type.h"
#include "gen_x64.h"

// if valid check if fn is not null, then check gen
gen_stat gen_type_des(gen *bg, te *t, gen **g, void **fn, err **e);
