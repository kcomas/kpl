
#pragma once

#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>
#include "lst.h"

typedef struct _alc alc;

typedef struct {
    size_t len;
    alc *h, *t;
} al;

inline al *al_i(void) {
    return calloc(1, sizeof(al));
}

#ifndef ALC_USED_FREE_PCT
    #define ALC_USED_FREE_PCT 0.95
#endif

typedef struct _alc {
    al *a;
    alc *prev, *next;
    size_t aus, len, size; // active used size, total used, size of chunk
    uint8_t *h; // memmap
} alc; // allocator chunk no frees filled then freed

#ifndef AL_PS_MUL
    #define AL_PS_MUL 2
#endif

inline alc *alc_i(al *const a, size_t size) {
    alc *ac = calloc(1, sizeof(alc));
    ac->a = a;
    LST_A(a, ac);
    size_t ps = (size_t) getpagesize() * AL_PS_MUL;
    ac->size = size <= ps ? ps : (size / ps + 1) * ps;
    ac->h = mmap(NULL, ac->size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    return ac;
}

void alc_f(alc *ac, void *fn);

inline void al_f(al *a) {
    LST_F(a, alc, alc_f, NULL);
    free(a);
}

typedef struct {
    size_t size;
    alc *ac;
} alci;

void *ala(al *const a, size_t size);

void alf(void *ptr);
