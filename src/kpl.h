
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <ctype.h>
#include "lst.h"

typedef struct _ast ast;

typedef struct _fn_node fn_node;

typedef struct _code code;

typedef void jit_fn(void);

typedef struct _jit jit;

// free not null w fn
#define FNNF(V, F) if (V) F(V)

// free not null
#define FNN(V) FNNF(V, free)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))

// print char x times
#define PCX(C, X) for (size_t i = 0; i < X; i++) putchar(C)
