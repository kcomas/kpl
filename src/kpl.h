
#pragma once

#include "tbl.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

typedef struct _fn_node fn_node;

// free not null
#define FNN(V) if (V != NULL) free(V)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))
