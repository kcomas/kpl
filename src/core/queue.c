
#include "./queue.h"

MEM_POOL(core_queue_item_pool);

static core_queue_item *core_queue_item_init(string *filename) {
    core_queue_item *item = mem_alloc(&core_queue_item_pool, sizeof(core_queue_item));
    item->state = QUEUE_ITEM_STATE(INIT);
    item->dependencies = 0;
    item->parents = NULL;
    item->filename = filename;
    item->filedata = NULL;
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
    if (!dependent->parents)
        dependent->parents = list_init(0, &def_unused_fn_table);
    list_add_back(dependent->parents, DEF_PTR(parent));
    parent->dependencies++;
}

static size_t core_queue_item_hash(const def_data data) {
    core_queue_item *item = data.ptr;
    return string_hash(item->filename);
}

static bool core_queue_item_eq(const def_data data_a, const def_data data_b) {
    core_queue_item *item_a = data_a.ptr, *item_b = data_b.ptr;
    return string_eq(item_a->filename, item_b->filename);
}

static void core_queue_item_print(const def_data data, FILE *file, int32_t idnt, uint32_t opts) {
    core_queue_item *item = data.ptr;
    fprintf(file, "%*s", idnt, "");
    string_print(item->filename, file, idnt, 0);
    fprintf(file, COLOR2(BOLD, MAGENTA) " %d" COLOR(RESET), item->dependencies);
    if (opts & QUEUE_ITEM_PRINT(DEPENDENCIES) && item->parents) {
        fprintf(file, COLOR2(BOLD, WHITE) " [" COLOR(RESET));
        for (list_item *head = item->parents->head; head; head = head->next)
            string_print(((core_queue_item*) head->data.ptr)->filename, file, idnt + 1, STRING_PRINT(NL_START));
        fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    }
    if (opts & QUEUE_ITEM_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table core_queue_item_fn_table = {
    .hash_fn = core_queue_item_hash,
    .cmp_fn = NULL,
    .eq_fn = core_queue_item_eq,
    .copy_fn = NULL,
    .serialize_fn = NULL,
    .print_fn = core_queue_item_print,
    .free_fn = core_queue_item_free
};

void core_queue_init(core_queue *queue, core_queue_item_print_opts opts) {
    mtx_init(&queue->mutex, mtx_plain);
    queue->ma = map_init(0, opts, &core_queue_item_fn_table);
}

void core_queue_free(core_queue *queue) {
    mtx_destroy(&queue->mutex);
    map_free(queue->ma);
}

core_queue_item *core_queue_add(core_queue *queue, const char *resolvepath, const char *filepath) {
    mtx_lock(&queue->mutex);
    string *filename = core_util_file_abs_path(resolvepath, filepath);
    if (!filename)
        return NULL;
    def_data found;
    core_queue_item find = { .filename = filename };
    if (map_action(&queue->ma, MAP_MODE(FIND), DEF_PTR(&find), &found) == DEF_STATUS(OK)) {
        string_free(filename);
        mtx_unlock(&queue->mutex);
        return found.ptr;
    }
    core_queue_item *insert = core_queue_item_init(filename);
    if (map_action(&queue->ma, MAP_MODE(INSERT), DEF_PTR(insert), &def_unused) != DEF_STATUS(OK)) {
        core_queue_item_free(insert);
        mtx_unlock(&queue->mutex);
        return NULL;
    }
    mtx_unlock(&queue->mutex);
    return insert;
}
