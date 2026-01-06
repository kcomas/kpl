
#include "./std.h"

static int32_t std_fns_len = 0;

static void *std_fns[NAMESPACE_STD_FN_COUNT] = {};

static int32_t namespace_std_register_fn(void *fn) {
    if (std_fns_len == NAMESPACE_STD_FN_COUNT) {
        printf(COLOR2(BOLD, RED) "Max Std Fn Count, Increase NAMESPACE_STD_FN_COUNT, Exiting\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    std_fns[std_fns_len] = fn;
    return std_fns_len++;
}

static type_base *std = nullptr;

[[gnu::constructor(DEF_CONSTRUCTOR_STD)]] static void namespace_std_constructor(void) {
    std = type_base_init(nullptr);
}

[[gnu::destructor(DEF_DESTRUCTOR_STD)]] static void namespace_std_destructor(void) {
    type_base_free(std);
}

static void _namespace_std_add(type *ty, va_list args) {
    const char *key;
    type_base *base = std;
    while ((key = va_arg(args, const char*))) {
        uint8_t string_buffer[sizeof(string) + TYPE_SYMBOL_C_STR_SIZE] = {};
        string *st = (string*) string_buffer;
        st->len = strlen(key);
        if (st->len > TYPE_SYMBOL_LEN) {
            printf(COLOR2(BOLD, RED) "Namespace std key %s to long, Exiting\n" COLOR(RESET), key);
            va_end(args);
            exit(DEF_EXIT_ERROR);
        }
        memcpy(st->data, key, st->len);
        type_base find_base = { .key = st };
        type find = { .name = TYPE_NAME(BASE), .class_union = TYPE_CLASS_UNION_BASE(&find_base) };
        def_data found;
        if (map_action(&base->type_map, MAP_MODE(FIND), DEF_PTR(&find), &found) != DEF_STATUS(OK)) {
            type *next = type_init(TYPE_NAME(BASE), TYPE_QUALIFIER_FLAG(_),
                TYPE_CLASS_UNION_BASE(type_base_init(string_init_c_str(key))));
            if (map_action(&base->type_map, MAP_MODE(INSERT), DEF_PTR(next), &def_unused) != DEF_STATUS(OK)) {
                printf(COLOR2(BOLD, RED) "Unable To Add Base To Std, Exiting\n" COLOR(RESET));
                va_end(args);
                exit(DEF_EXIT_ERROR);
            }
            base = next->class_union.base;
        } else
            base = ((type*) found.ptr)->class_union.base;
    }
    if (type_base_add(base, ty) != DEF_STATUS(OK)) {
        printf(COLOR2(BOLD, RED) "Unable To Add Type To Std, Exiting\n" COLOR(RESET));
        va_end(args);
        exit(DEF_EXIT_ERROR);
    }
    va_end(args);
}

void namespace_std_add(type *ty, ...) {
    va_list args;
    va_start(args, ty);
    return _namespace_std_add(ty, args);
}

void namespace_std_add_fn(type *ty, void *fn, ...) {
    if (ty->name != TYPE_NAME(FN)) {
        printf(COLOR2(BOLD, RED) "Invalid Function Type, Exiting\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    ty->class_union.table->fn_idx = namespace_std_register_fn(fn);
    ty->qualifier_flags |= TYPE_FLAG(C_CODE);
    va_list args;
    va_start(args, fn);
    return _namespace_std_add(ty, args);
}

void *namespace_std_get_fn(type *ty) {
    if (ty->name != TYPE_NAME(FN)) {
        printf(COLOR2(BOLD, RED) "Invalid Function Type, Exiting\n" COLOR(RESET));
        exit(DEF_EXIT_ERROR);
    }
    if (ty->class_union.table->fn_idx < 0 || ty->class_union.table->fn_idx >= std_fns_len)
        return nullptr;
    return std_fns[ty->class_union.table->fn_idx];
}

void namespace_std_print(FILE *file, int32_t idnt, uint32_t print_opts) {
    type_base_print(std, file, idnt, print_opts);
}
