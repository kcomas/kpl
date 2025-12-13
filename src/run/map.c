
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

map *map_init(size_t size, def_hash_fn hash_fn, def_eq_fn eq_fn, def_print_fn print_fn, uint32_t print_opts,
    def_free_fn free_fn) {
    if (size < MAP_MIN_SIZE)
        size = MAP_MIN_SIZE;
    map *ma = mem_alloc(&map_pool, sizeof(map) + sizeof(map_bucket) * size);
    ma->size = size;
    ma->used = 0;
    ma->hash_fn = hash_fn;
    ma->eq_fn = eq_fn;
    ma->print_fn = print_fn;
    ma->print_opts = print_opts;
    ma->free_fn = free_fn;
    ma->head = ma->tail = NULL;
    memset(ma->buckets, 0, sizeof(map_bucket*) * size);
    return ma;
}

void map_free(map *ma) {
    for (size_t bucket_idx = 0; bucket_idx < ma->size; bucket_idx++) {
        if (!ma->buckets[bucket_idx])
            continue;
        if (ma->free_fn && ma->buckets[bucket_idx]->data.ptr)
            ma->free_fn(ma->buckets[bucket_idx]->data.ptr);
        map_bucket_free(ma->buckets[bucket_idx]);
    }
    mem_free(&map_pool, ma);
}

static map_bucket **map_search(map *ma, def_data search) {
    size_t hash = ma->hash_fn(search) % ma->size, max_search_count = ma->size * MAP_MAX_SEARCH_PERCENT;
    while (max_search_count) {
        if (!ma->buckets[hash] || ma->eq_fn(search, ma->buckets[hash]->data))
            return &ma->buckets[hash];
        hash = (hash + 1) % ma->size;
        max_search_count--;
    }
    return NULL;
}

static def_status map_resize(map **ma) {
    map *new_ma = map_init((*ma)->size * MAP_RESIZE_MUL, (*ma)->hash_fn, (*ma)->eq_fn, (*ma)->print_fn,
        (*ma)->print_opts, (*ma)->free_fn);
    new_ma->used = (*ma)->used;
    new_ma->head = (*ma)->head;
    new_ma->tail = (*ma)->tail;
    for (map_bucket *head = new_ma->head; head; head = head->next) {
        size_t hash = new_ma->hash_fn(head->data) % new_ma->size,
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
    map_bucket **bucket = NULL;
    if ((bucket = map_search(*ma, search)))
        return bucket;
    if (map_resize(ma) != DEF_STATUS(OK))
        return NULL;
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
                if ((*ma)->free_fn && (*bucket)->data.ptr)
                    (*ma)->free_fn((*bucket)->data.ptr);
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
            *bucket = NULL;
            (*ma)->used--;
            return DEF_STATUS(OK);
        default:
            break;
    }
    return DEF_STATUS(ERROR);
}

void map_print(const map *ma, FILE *file, size_t idnt, map_print_opts opts) {
    if (!ma->print_fn)
        return;
    for (map_bucket *head = ma->head; head; head = head->next)
        if (head == ma->head && (opts & MAP_PRINT(NO_FIRST_IDNT)))
            ma->print_fn(head->data, file, 0, ma->print_opts);
        else
            ma->print_fn(head->data, file, idnt, ma->print_opts);
    if (opts & MAP_PRINT(NL_END))
        fprintf(file, "\n");
}
