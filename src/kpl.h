
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

typedef struct _fn_node fn_node;

// free not null w fn
#define FNNF(V, F) if (V) F(V)

// free not null
#define FNN(V) FNNF(V, free)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))
