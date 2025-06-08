
#include "al.h"

extern inline al *al_i(void);

extern inline alc *alc_i(al *const a, int size);

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
