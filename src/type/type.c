
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
    if (!ty || --ty->ref_count > 0)
        return;
    switch (type_name_get_class(ty->name)) {
        case TYPE_CLASS(SCALAR):
        case TYPE_CLASS(_):
            break;
        case TYPE_CLASS(FIXED):
            type_fixed_free(ty->class_union.fixed);
            break;
        case TYPE_CLASS(VECTOR):
            type_free(ty->class_union.inner_type);
            break;
        case TYPE_CLASS(LIST):
            type_list_free(ty->class_union.list);
            break;
        case TYPE_CLASS(TABLE):
            type_table_free(ty->class_union.table);
            break;
        case TYPE_CLASS(TAG):
            type_tag_free(ty->class_union.tag);
            break;
    }
    mem_free(&type_pool, ty);
}

size_t type_hash(const type *ty) {
    size_t hash = 0;
    if (!ty)
        return hash;
    hash += ty->name << type_name_get_class(ty->name);
    switch (type_name_get_class(ty->name)) {
        case TYPE_CLASS(SCALAR):
        case TYPE_CLASS(_):
            break;
        case TYPE_CLASS(VECTOR):
            hash += type_hash(ty->class_union.inner_type);
            break;
        case TYPE_CLASS(FIXED):
            hash += type_fixed_hash(ty->class_union.fixed);
            break;
        case TYPE_CLASS(LIST):
            hash += type_list_hash(ty->class_union.list);
            break;
        case TYPE_CLASS(TABLE):
            hash += type_table_hash(ty->class_union.table);
            break;
        case TYPE_CLASS(TAG):
            hash += type_tag_hash(ty->class_union.tag);
            break;
    }
    return hash;
}

bool type_eq(const type *ty_a, const type *ty_b) {
    if (ty_a == ty_b)
        return true;
    if (!ty_a || !ty_b)
        return false;
    if (ty_a->name != ty_b->name)
        return false;
    switch (type_name_get_class(ty_a->name)) {
        case TYPE_CLASS(SCALAR):
            return true;
        case TYPE_CLASS(VECTOR):
            return type_eq(ty_a->class_union.inner_type, ty_b->class_union.inner_type);
        case TYPE_CLASS(FIXED):
            return type_fixed_eq(ty_a->class_union.fixed, ty_b->class_union.fixed);
        case TYPE_CLASS(LIST):
            return type_list_eq(ty_a->class_union.list, ty_b->class_union.list);
        case TYPE_CLASS(TABLE):
            return type_table_eq(ty_a->class_union.table, ty_b->class_union.table);
        case TYPE_CLASS(TAG):
            return type_tag_eq(ty_a->class_union.tag, ty_b->class_union.tag);
        case TYPE_CLASS(_):
            break;
    }
    return false;
}

type *type_copy(type *ty) {
    ty->ref_count++;
    return ty;
}

static void print_type_name(FILE *file, type_name name) {
    fprintf(file, COLOR2(BOLD, LIGHT_YELLOW) "%s" COLOR(RESET), type_name_str(name));
}

void type_print(const type *ty, FILE *file, int32_t idnt, type_print_opts opts) {
    if (!ty)
        return;
    fprintf(file, "%*s", idnt, "");
    print_type_name(file, ty->name);
    switch (type_name_get_class(ty->name)) {
        case TYPE_CLASS(SCALAR):
        case TYPE_CLASS(_):
            break;
        case TYPE_CLASS(FIXED):
            type_fixed_print(ty->class_union.fixed, file, 0, opts);
            break;
        case TYPE_CLASS(VECTOR):
            fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
            type_print(ty->class_union.inner_type, file, 0, opts);
            fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
            break;
        case TYPE_CLASS(LIST):
            type_list_print(ty->class_union.list, file, 0, opts);
            break;
        case TYPE_CLASS(TABLE):
            type_table_print(ty->class_union.table, file, 0, opts);
            break;
        case TYPE_CLASS(TAG):
            type_tag_print(ty->class_union.tag, file, 0, opts);
            break;
    }
    if (opts & TYPE_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table type_fn_table = {
    .hash_fn = (void*) type_hash,
    .cmp_fn = NULL,
    .eq_fn = (void*) type_eq,
    .copy_fn = (void*) type_copy,
    .serialize_fn = NULL,
    .print_fn = (void*) type_print,
    .free_fn = (void*) type_free
};
