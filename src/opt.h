
#pragma once

#include "chk.h"

uint32_t opt_exp_id(te *x);

uint16_t opt_exp_eid(te *x);

uint32_t opt_exp_flgs(te *x);

// export entry te[mc;u5(id)|u4(exportid)|u4(local_arg_flags)>data;type]

fld *opt_b(fld *f);
