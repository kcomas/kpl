
#include "./core.h"

static atomic_size_t running_threads = 0;

static core_queue queue = {};

[[gnu::constructor(DEF_CONSTRUCTOR_CORE)]] static void core_constructor(void) {
    core_queue_init(&queue, CORE_QUEUE_ITEM_PRINT(NL_END) | CORE_QUEUE_ITEM_PRINT(DEPENDENCIES));
}

[[gnu::destructor(DEF_DESTRUCTOR_CORE)]] static void core_destructor(void) {
#ifdef CORE_QUEUE_DEBUG_PRINT
    printf(COLOR2(BOLD, CYAN) "Core Queue\n" COLOR(RESET));
    map_print(queue.ma, stdout, 0, MAP_PRINT(_));
#endif
    core_queue_free(&queue);
}

static error *core_import_thread_fn(def_data data) {
    core_queue_item *queue_item = data.ptr;
    error *er = core_import(&queue, queue_item);
    if (er)
        core_queue_error(&queue, er);
    running_threads--;
    sem_post(&queue.sem);
    return nullptr;
}

static bool core_next_test(core_queue_item *item) {
    return item->state_flags & CORE_QUEUE_ITEM_STATE(INIT) ||
        (item->state_flags & CORE_QUEUE_ITEM_STATE(DEPENDENCIES) && !item->dependencies);
}

error *core_main(const char *filepath) {
    core_queue_item *main = core_queue_add(&queue, nullptr, filepath);
    if (!main)
        return ERROR_INIT(0, &def_unused_fn_table, def(), "File Not Found");
    main->state_flags |= CORE_QUEUE_ITEM_FLAG(MAIN);
    core_queue_item *next = nullptr;
    uint32_t prev_queue_used = 0;
    while (!queue.er && (next || prev_queue_used < queue.ma->used)) {
        next = nullptr;
        pthread_mutex_lock(&queue.mutex);
        for (const map_bucket *tail = queue.ma->tail; tail; tail = tail->prev) {
            core_queue_item *search = tail->data.ptr;
            if (core_next_test(search)) {
                next = search;
                break;
            }
        }
        pthread_mutex_unlock(&queue.mutex);
        prev_queue_used = queue.ma->used;
        if (queue.er || !next) {
            sem_wait(&queue.sem);
        } else if (next->state_flags & CORE_QUEUE_ITEM_STATE(INIT)) {
            core_import_before(next);
            running_threads++;
            task_init_detached(CORE_QUEUE_ITEM_PRINT(_), &def_unused_fn_table,
                    def_ptr(next), core_import_thread_fn);
        } else if (next->state_flags & CORE_QUEUE_ITEM_STATE(DEPENDENCIES)) {
            // TODO
            next->state_flags ^= CORE_QUEUE_ITEM_STATE(DEPENDENCIES);
            next->state_flags |= CORE_QUEUE_ITEM_STATE(RUNNING);
        } else
            core_queue_error(&queue, core_queue_item_error(next, "invalid core queue item state"));
    }
    while (running_threads)
        sem_wait(&queue.sem);
    error *er = queue.er;
    queue.er = nullptr;
    return er;
}
