
#include "./type.h"

MEM_POOL(type_pool);

type *type_init(type_name name, type_qualifier_flags qualifier_flags, type_class_union class_union) {
    type *ty = mem_alloc(&type_pool, sizeof(type));
    ty->ref_count = 1;
    ty->name = name;
    ty->qualifier_flags = qualifier_flags;
    ty->class_union = class_union;
    return ty;
}

void type_free(type *ty) {
    // TODO
    mem_free(&type_pool, ty);
}
