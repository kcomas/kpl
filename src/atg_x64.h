
#pragma once

#include "./lib/atg.h"
#include "opt.h"
#include "./lib/gen_x64.h"

bool atg_x64_enq(const te *an);

atg_stat atg_err(atg_stat stat, te *an, te **e);

atg *atg_b(atg *t);
