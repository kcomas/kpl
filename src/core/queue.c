
#include "./queue.h"

MEM_POOL(core_queue_item_pool);

static core_queue_item *core_queue_item_init(core_queue *queue, string *filename) {
    core_queue_item *item = mem_alloc(&core_queue_item_pool, sizeof(core_queue_item));
    item->state = QUEUE_ITEM_STATE(INIT);
    item->dependencies = 0;
    item->parents = nullptr;
    item->queue = queue;
    item->filename = filename;
    item->filedata = nullptr;
    return item;
}

static void core_queue_item_free(void *data) {
    core_queue_item *item = data;
    if (item->parents)
        list_free(item->parents);
    string_free(item->filename);
    if (item->filedata)
        string_free(item->filedata);
    mem_free(&core_queue_item_pool, item);
}

void core_queue_item_add_parent(core_queue_item *restrict dependent, core_queue_item *restrict parent) {
    if (!dependent->parents) {
        dependent->parents = list_init(0, &def_unused_fn_table);
        list_add_back(dependent->parents, DEF_PTR(parent));
        parent->dependencies++;
        return;
    }
    list_item *head = dependent->parents->head;
    while (head) {
        if (head->data.ptr == parent)
            break;
        head = head->next;
    }
    if (head)
        return;
    list_add_back(dependent->parents, DEF_PTR(parent));
    parent->dependencies++;
    return;
}

static size_t core_queue_item_hash(const def_data data) {
    core_queue_item *item = data.ptr;
    return string_hash(item->filename);
}

static bool core_queue_item_eq(const def_data data_a, const def_data data_b) {
    core_queue_item *item_a = data_a.ptr, *item_b = data_b.ptr;
    return string_eq(item_a->filename, item_b->filename);
}

static void core_queue_item_print(const def_data data, FILE *file, int32_t idnt, uint32_t print_opts) {
    core_queue_item *item = data.ptr;
    fprintf(file, "%*s", idnt, "");
    string_print(item->filename, file, idnt, 0);
    fprintf(file, COLOR2(BOLD, MAGENTA) " %d" COLOR(RESET), item->dependencies);
    if (print_opts & QUEUE_ITEM_PRINT(DEPENDENCIES) && item->parents) {
        fprintf(file, COLOR2(BOLD, WHITE) " [" COLOR(RESET));
        for (list_item *head = item->parents->head; head; head = head->next)
            string_print(((core_queue_item*) head->data.ptr)->filename, file, idnt + 1, STRING_PRINT(NL_START));
        fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    }
    if (print_opts & QUEUE_ITEM_PRINT(NL_END))
        fprintf(file, "\n");
}

static def_fn_table core_queue_item_fn_table = {
    .hash_fn = core_queue_item_hash,
    .cmp_fn = nullptr,
    .eq_fn = core_queue_item_eq,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = core_queue_item_print,
    .free_fn = core_queue_item_free
};

static def_fn_table core_queue_item_error_fn_table = {
    .hash_fn = nullptr,
    .cmp_fn = nullptr,
    .eq_fn = nullptr,
    .copy_fn = nullptr,
    .serialize_fn = nullptr,
    .print_fn = core_queue_item_print,
    .free_fn = nullptr
};

error *core_queue_item_error(core_queue_item *item, const char *msg) {
    return ERROR_INIT(item->queue->ma->print_opts ^ QUEUE_ITEM_PRINT(NL_END),
        &core_queue_item_error_fn_table, DEF_PTR(item), msg);
}

void core_queue_init(core_queue *queue, core_queue_item_print_opts print_opts) {
    queue->state_count.init = queue->state_count.dependencies = 0;
    queue->state_count.running = queue->state_count.done = 0;
    queue->ma = map_init(0, print_opts, &core_queue_item_fn_table);
    queue->er = nullptr;
    pthread_mutex_init(&queue->mutex, nullptr);
    sem_init(&queue->sem, 0, 0);
}

void core_queue_free(core_queue *queue) {
    map_free(queue->ma);
    if (queue->er)
        error_free(queue->er);
    pthread_mutex_destroy(&queue->mutex);
    sem_destroy(&queue->sem);
}

core_queue_item *core_queue_add(core_queue *queue, const char *resolvepath, const char *filepath) {
    string *filename = core_util_file_abs_path(resolvepath, filepath);
    if (!filename)
        return nullptr;
    pthread_mutex_lock(&queue->mutex);
    def_data found;
    core_queue_item find = { .filename = filename };
    if (map_action(&queue->ma, MAP_MODE(FIND), DEF_PTR(&find), &found) == DEF_STATUS(OK)) {
        string_free(filename);
        pthread_mutex_unlock(&queue->mutex);
        return found.ptr;
    }
    core_queue_item *insert = core_queue_item_init(queue, filename);
    if (map_action(&queue->ma, MAP_MODE(INSERT), DEF_PTR(insert), &def_unused) != DEF_STATUS(OK)) {
        core_queue_item_free(insert);
        pthread_mutex_unlock(&queue->mutex);
        return nullptr;
    }
    pthread_mutex_unlock(&queue->mutex);
    queue->state_count.init++;
    return insert;
}
