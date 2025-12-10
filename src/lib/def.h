
#pragma once

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define DEF_STATUS(N) DEF_STATUS_##N

typedef enum [[gnu::packed]] {
    DEF_STATUS(OK),
    DEF_STATUS(ERROR)
} def_status;
