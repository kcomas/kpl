
#include "al.h"

extern inline al *al_i(void);

extern inline alc *alc_i(al *const a, size_t size);

void alc_f(alc *ac, void *fn) {
    (void) fn;
    munmap(ac->h, ac->size);
    free(ac);
}

void *ala(al *const a, size_t size) {
    size += sizeof(alci);
    size_t mo = size % sizeof(alci);
    if (mo) size = size - mo + sizeof(alci);
    alc *ac;
    if (!a->h) ac = alc_i(a, size);
    else {
        // TODO find chunk with enough space
    }
    void *p = &ac->h[ac->len];
    ac->aus += size;
    ac->len += size;
    alci *ai = (alci*) p;
    ai->size = size;
    ai->ac = ac;
    return p + sizeof(alci);
}

void alf(void *ptr) {
    alci *ai = (alci*) ptr - sizeof(alci);
    alc *ac = ai->ac;
    ac->aus -= ai->size;
    if (ac->aus <= 0 && ((float) ac->len / (float) ac->size > ALC_USED_FREE_PCT)) {
        LST_R(ac->a, alc, ac, alc_f, NULL);
    }
}
