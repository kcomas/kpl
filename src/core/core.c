
#include "./core.h"

static atomic_size_t running_threads = 0;

static core_queue queue = {};

[[gnu::constructor(DEF_CONSTRUCTOR_CORE)]] static void core_constructor(void) {
    core_queue_init(&queue, CORE_QUEUE_ITEM_PRINT(NL_END) | CORE_QUEUE_ITEM_PRINT(DEPENDENCIES));
}

[[gnu::destructor(DEF_DESTRUCTOR_CORE)]] static void core_destructor(void) {
    map_print(queue.ma, stdout, 0, MAP_PRINT(_));
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

error *core_main(const char *filepath) {
    core_queue_item *main = core_queue_add(&queue, nullptr, filepath);
    if (!main)
        return ERROR_INIT(0, &def_unused_fn_table, def(), "File Not Found");
    main->state_flags |= CORE_QUEUE_ITEM_FLAG(MAIN);
    uint32_t queue_size = 0;
    while (running_threads || queue_size < queue.ma->used) {
        core_queue_item *next = nullptr;
        pthread_mutex_lock(&queue.mutex);
        for (map_bucket *head = queue.ma->head; head; head = head->next) {
            core_queue_item *search = head->data.ptr;
            if (search->state_flags & CORE_QUEUE_ITEM_STATE(INIT)) {
                next = search;
                break;
            }
            if (search->state_flags & CORE_QUEUE_ITEM_STATE(DEPENDENCIES) && !search->dependencies) {
                next = search;
                break;
            }
        }
        pthread_mutex_unlock(&queue.mutex);
        if (!next || queue.er) {
            queue_size = queue.ma->used;
            sem_wait(&queue.sem);
            continue;
        }
        running_threads++;
        if (next->state_flags & CORE_QUEUE_ITEM_STATE(INIT)) {
            core_import_before(next);
            task_init_detached(CORE_QUEUE_ITEM_PRINT(_), &def_unused_fn_table,
                    def_ptr(next), core_import_thread_fn);
        } else if (next->state_flags & CORE_QUEUE_ITEM_STATE(DEPENDENCIES)) {
            // TODO
            next->state_flags ^= CORE_QUEUE_ITEM_STATE(DEPENDENCIES);
            next->state_flags |= CORE_QUEUE_ITEM_STATE(RUNNING);
            running_threads--;
        }
        else
            core_queue_error(&queue, core_queue_item_error(next, "invalid core queue item state"));
    }
    error *er = queue.er;
    queue.er = nullptr;
    return er;
}
