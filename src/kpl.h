
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

typedef struct _ast ast;

// free not null
#define FNN(V) if (V != NULL) free(V)

// static array len
#define AL(V) (sizeof(V) / sizeof(V[0]))
