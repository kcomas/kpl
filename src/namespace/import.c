
#include "./import.h"

static type_base *import = nullptr;

static pthread_mutex_t import_mutex = PTHREAD_MUTEX_INITIALIZER;

[[gnu::constructor(DEF_CONSTRUCTOR_IMPORT)]] static void namespace_import_constructor(void) {
    import = type_base_init(nullptr);
}

[[gnu::destructor(DEF_DESTRUCTOR_IMPORT)]] static void namespace_import_destructor(void) {
    type_base_free(import);
    pthread_mutex_destroy(&import_mutex);
}

type_base *namespace_import_insert(const string *filename) {
    type_base *base = type_base_init(string_copy(filename));
    type *insert = type_init(TYPE_NAME(BASE), TYPE_QUALIFIER_FLAG(_), type_class_union_base(base));
    pthread_mutex_lock(&import_mutex);
    if (type_base_add(import, insert) != DEF_STATUS(OK)) {
        type_base_free(base);
        base = nullptr;
    }
    pthread_mutex_unlock(&import_mutex);
    return base;
}

type_base *namespace_import_get(const string *filename) {
    type_base find_base = { .key = (string*) filename };
    type find = { .name = TYPE_NAME(BASE), .class_union = type_class_union_base(&find_base) };
    def_data found = def();
    pthread_mutex_lock(&import_mutex);
    if (map_action(&import->type_map, MAP_MODE(FIND), def_ptr(&find), &found) != DEF_STATUS(OK))
        found = def();
    pthread_mutex_unlock(&import_mutex);
    return found.ptr;
}

void namespace_import_print(FILE *file, int32_t idnt, uint32_t print_opts) {
    pthread_mutex_lock(&import_mutex);
    type_base_print(import, file, idnt, print_opts);
    pthread_mutex_unlock(&import_mutex);
}
