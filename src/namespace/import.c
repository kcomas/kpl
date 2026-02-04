
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
