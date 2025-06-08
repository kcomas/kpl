
#include "al.h"

extern inline al *al_i(void *nal);

extern inline alc *alc_i(al *const a, size_t size);

void alc_f(alc *ac, void *fn) {
    (void) fn;
#ifdef KPL_ALD
    if (ac->aus > 0) printf("==Lost: %lub==\n", ac->aus);
#endif
    munmap(ac, ac->size);
}

extern inline void al_f(al *a);

void *ala(al *const a, size_t size) {
    a->u++;
    size += sizeof(alci);
    alc *ac = a->h;
    while (ac) {
        if (ac->size - ac->len >= size) break;
        ac = ac->next;
    }
    if (!ac) ac = alc_i(a, size);
    void *ptr = ac->h + ac->len;
    posix_memalign(&ptr, sizeof(alci), ac->size);
    ac->aus += size;
    ac->len += size;
    alci *ai = (alci*) ptr;
    ai->size = size;
    ai->ac = ac;
    ptr += sizeof(alci);
    return ptr;
}

void alf(void *ptr) {
    alci *ai = ptr - sizeof(alci);
    alc *ac = ai->ac;
    ac->a->f++;
    ac->aus -= ai->size;
#ifdef KPL_ALD
    if (ai->size == 0) {
        printf("Double Free\n");
        exit(1);
    }
    ai->size = 0;
#endif
    if (ac->aus == 0 && ((double) ac->len / (double) ac->size >= ALC_USED_FREE_PCT)) {
        al *a = ac->a;
        a->size -= ac->size;
        LST_R(a, alc, ac, alc_f, NULL);
    }
    // TODO move small pieces
}
