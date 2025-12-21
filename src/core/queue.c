
#include "./queue.h"

MEM_POOL(queue_item_pool);

static queue_item *queue_item_init(string *filename) {
    queue_item *item = mem_alloc(&queue_item_pool, sizeof(queue_item));
    item->state = QUEUE_ITEM_STATE(INIT);
    item->dependencies = 0;
    item->parents = NULL;
    item->filename = filename;
    item->filedata = NULL;
    return item;
}

static void queue_item_free(void *data) {
    queue_item *item = data;
    if (item->parents)
        list_free(item->parents);
    string_free(item->filename);
    if (item->filedata)
        string_free(item->filedata);
    mem_free(&queue_item_pool, item);
}

void queue_item_add_parent(queue_item *restrict dependent, queue_item *restrict parent) {
    if (!dependent->parents)
        dependent->parents = list_init(0, &def_unused_fn_table);
    list_add_back(dependent->parents, DEF_PTR(parent));
    parent->dependencies++;
}

static size_t queue_item_hash(const def_data data) {
    queue_item *item = data.ptr;
    return string_hash(item->filename);
}

static bool queue_item_eq(const def_data data_a, const def_data data_b) {
    queue_item *item_a = data_a.ptr, *item_b = data_b.ptr;
    return string_eq(item_a->filename, item_b->filename);
}

static void queue_item_print(const def_data data, FILE *file, int32_t idnt, uint32_t opts) {
    queue_item *item = data.ptr;
    fprintf(file, "%*s", idnt, "");
    string_print(item->filename, file, idnt, 0);
    fprintf(file, COLOR2(BOLD, MAGENTA) " %d" COLOR(RESET), item->dependencies);
    if (opts & QUEUE_ITEM_PRINT(DEPENDENCIES) && item->parents) {
        fprintf(file, COLOR2(BOLD, WHITE) " [" COLOR(RESET));
        for (list_item *head = item->parents->head; head; head = head->next)
            string_print(((queue_item*) head->data.ptr)->filename, file, idnt + 1, STRING_PRINT(NL_START));
        fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    }
    if (opts & QUEUE_ITEM_PRINT(NL_END))
        fprintf(file, "\n");
}

def_fn_table queue_item_fn_table = {
    .hash_fn = queue_item_hash,
    .cmp_fn = NULL,
    .eq_fn = queue_item_eq,
    .copy_fn = NULL,
    .serialize_fn = NULL,
    .print_fn = queue_item_print,
    .free_fn = queue_item_free
};

map *queue_init(queue_item_print_opts opts) {
    return map_init(0, opts, &queue_item_fn_table);
}

queue_item *queue_add(map **queue, const char *resolvepath, const char *filepath) {
    string *filename = core_util_file_abs_path(resolvepath, filepath);
    if (!filename)
        return NULL;
    def_data found;
    queue_item find = { .filename = filename };
    if (map_action(queue, MAP_MODE(FIND), DEF_PTR(&find), &found) == DEF_STATUS(OK)) {
        string_free(filename);
        return found.ptr;
    }
    queue_item *insert = queue_item_init(filename);
    if (map_action(queue, MAP_MODE(INSERT), DEF_PTR(insert), &def_unused) != DEF_STATUS(OK)) {
        queue_item_free(insert);
        return NULL;
    }
    return insert;
}
