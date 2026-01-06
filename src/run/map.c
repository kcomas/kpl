
#include "./map.h"

MEM_POOL(map_bucket_pool);

static map_bucket *map_bucket_init(def_data data) {
    map_bucket *bucket = mem_alloc(&map_bucket_pool, sizeof(map_bucket));
    bucket->data = data;
    return bucket;
}

static void map_bucket_free(map_bucket *bucket) {
    mem_free(&map_bucket_pool, bucket);
}

MEM_POOL(map_pool);

constexpr uint32_t map_max_size = UINT32_MAX - sizeof(map);

map *map_init(uint32_t size, uint32_t print_opts, def_fn_table *fn_table) {
    if (size < MAP_MIN_SIZE)
        size = MAP_MIN_SIZE;
    if (size * sizeof(map_bucket*) > map_max_size)
        return nullptr;
    map *ma = mem_alloc(&map_pool, sizeof(map) + sizeof(map_bucket*) * size);
    ma->size = size;
    ma->used = 0;
    ma->print_opts = print_opts;
    ma->fn_table = fn_table;
    ma->head = ma->tail = nullptr;
    memset(ma->buckets, 0, sizeof(map_bucket*) * size);
    return ma;
}

void map_free(map *ma) {
    for (uint32_t bucket_idx = 0; bucket_idx < ma->size; bucket_idx++) {
        if (!ma->buckets[bucket_idx])
            continue;
        if (ma->fn_table->free_fn && ma->buckets[bucket_idx]->data.ptr)
            ma->fn_table->free_fn(ma->buckets[bucket_idx]->data.ptr);
        map_bucket_free(ma->buckets[bucket_idx]);
    }
    mem_free(&map_pool, ma);
}

static map_bucket **map_search(map *ma, def_data search) {
    uint32_t hash = ma->fn_table->hash_fn(search) % ma->size, max_search_count = ma->size * MAP_MAX_SEARCH_PERCENT;
    while (max_search_count) {
        if (!ma->buckets[hash] || ma->fn_table->eq_fn(search, ma->buckets[hash]->data))
            return &ma->buckets[hash];
        hash = (hash + 1) % ma->size;
        max_search_count--;
    }
    return nullptr;
}

constexpr const uint32_t map_max_size_check = UINT32_MAX / MAP_RESIZE_MUL;

static def_status map_resize(map **ma) {
    if (map_max_size_check <= (*ma)->size)
        return DEF_STATUS(ERROR);
    map *new_ma = map_init((*ma)->size * MAP_RESIZE_MUL, (*ma)->print_opts, (*ma)->fn_table);
    if (!new_ma)
        return DEF_STATUS(ERROR);
    new_ma->used = (*ma)->used;
    new_ma->head = (*ma)->head;
    new_ma->tail = (*ma)->tail;
    for (map_bucket *head = new_ma->head; head; head = head->next) {
        uint32_t hash = new_ma->fn_table->hash_fn(head->data) % new_ma->size,
            max_search_count = new_ma->size * MAP_MAX_SEARCH_PERCENT;
        while (max_search_count) {
            if (!new_ma->buckets[hash]) {
                new_ma->buckets[hash] = head;
                break;
            }
            hash = (hash + 1) % new_ma->size;
            max_search_count--;
        }
        if (!max_search_count) {
            mem_free(&map_pool, new_ma);
            return DEF_STATUS(ERROR);
        }
    }
    mem_free(&map_pool, *ma);
    *ma = new_ma;
    return DEF_STATUS(OK);
}

static map_bucket **map_search_resize(map **ma, def_data search) {
    map_bucket **bucket = nullptr;
    if ((bucket = map_search(*ma, search)))
        return bucket;
    if (map_resize(ma) != DEF_STATUS(OK))
        return nullptr;
    return map_search(*ma, search);
}

static void map_new_bucket(map **ma, map_bucket **bucket, def_data data) {
    *bucket = map_bucket_init(data);
    if ((*ma)->head) {
        (*bucket)->prev = (*ma)->tail;
        (*ma)->tail = (*ma)->tail->next = *bucket;
    } else
        (*ma)->head = (*ma)->tail = *bucket;
    (*ma)->used++;
}

def_status map_action(map **ma, map_mode mode, def_data search, def_data *found) {
    map_bucket **bucket = map_search_resize(ma, search);
    if (!bucket)
        return DEF_STATUS(ERROR);
    switch (mode) {
        case MAP_MODE(INSERT):
            if (*bucket)
                break;
            map_new_bucket(ma, bucket, search);
            return DEF_STATUS(OK);
        case MAP_MODE(UPSERT):
            if (*bucket) {
                if ((*ma)->fn_table->free_fn && (*bucket)->data.ptr)
                    (*ma)->fn_table->free_fn((*bucket)->data.ptr);
                (*bucket)->data = search;
            } else
                map_new_bucket(ma, bucket, search);
            return DEF_STATUS(OK);
        case MAP_MODE(FIND):
            if (!*bucket)
                break;
            *found = (*bucket)->data;
            return DEF_STATUS(OK);
        case MAP_MODE(REMOVE):
            if (!*bucket)
                break;
            *found = (*bucket)->data;
            mem_list_remove(*bucket);
            map_bucket_free(*bucket);
            *bucket = nullptr;
            (*ma)->used--;
            return DEF_STATUS(OK);
        default:
            break;
    }
    return DEF_STATUS(ERROR);
}

size_t map_hash(const map *ma) {
    size_t hash = ma->used;
    if (!ma)
        return 0;
    for (const map_bucket *bucket = ma->head; bucket; bucket = bucket->next)
        hash += ma->fn_table->hash_fn(bucket->data);
    return hash;
}

bool map_eq(const map *ma_a, const map *ma_b) {
    if (ma_a == ma_b)
        return true;
    if (!ma_a || !ma_b || ma_a->used != ma_b->used)
        return false;
    const map_bucket *bucket_a = ma_a->head, *bucket_b = ma_b->head;
    while (bucket_a && bucket_b) {
        if (!ma_a->fn_table->eq_fn(bucket_a->data, bucket_b->data))
            return false;
        bucket_a = bucket_a->next;
        bucket_b = bucket_b->next;
    }
    return !bucket_a && !bucket_b;
}

void map_print(const map *ma, FILE *file, int32_t idnt, map_print_opts print_opts) {
    for (map_bucket *head = ma->head; head; head = head->next) {
        if (head == ma->head && (print_opts & MAP_PRINT(NO_FIRST_IDNT)))
            ma->fn_table->print_fn(head->data, file, 0, ma->print_opts);
        else
            ma->fn_table->print_fn(head->data, file, idnt, ma->print_opts);
        if ((print_opts & MAP_PRINT(SEMI_SPACER)) && head->next)
            fprintf(file, COLOR(DARK_GREY) ";" COLOR(RESET));
    }
    if (print_opts & MAP_PRINT(NL_END))
        fprintf(file, "\n");
}
