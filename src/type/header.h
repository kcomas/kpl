
#pragma once

#include "../run/run.h"
#include "./qualifier_flags.h"
#include "./class_name.h"

typedef struct _type type;

void type_free(type *ty);
