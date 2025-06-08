
#include "al.h"

extern inline size_t algn(size_t size, size_t an);

extern inline al *al_i(void *nal);

extern inline alc *alc_i(size_t size);

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
    size = algn(size + sizeof(alci), DEFALGN);
    alc *ac = a->h;
    while (ac) {
        if (ac->size - ac->len >= size) break;
        ac = ac->next;
    }
    if (!ac) {
        ac = alc_i(size);
        LST_A(a, ac);
        ac->a = a;
        a->size += ac->size;
    }
    alci *ai = (void*) ac + ac->len;
    ac->aus += size;
    ac->len += size;
    ai->size = size;
    ai->ac = ac;
    return (void*) ai + sizeof(alci);
}

void alf(void *ptr) {
    alci *ai = (void*) ptr - sizeof(alci);
    alc *ac = ai->ac;
    al *a = ac->a;
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
        a->size -= ac->size;
        LST_R(a, alc, ac, alc_f, NULL);
    }
    // TODO move small pieces
}
