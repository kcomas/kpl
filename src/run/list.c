
#include "./list.h"

MEM_POOL(list_item_pool);

static list_item *list_item_init(def_data data) {
    list_item *item = mem_alloc(&list_item_pool, sizeof(list_item));
    item->data = data;
    return item;
}

static void list_item_free(list_item *item) {
    mem_free(&list_item_pool, item);
}

MEM_POOL(list_pool);

list *list_init(uint32_t print_opts, def_fn_table *fn_table) {
    list *li = mem_alloc(&list_pool, sizeof(list));
    li->print_opts = print_opts;
    li->fn_table = fn_table;
    li->head = li->tail = nullptr;
    return li;
}

void list_free(list *li) {
    list_item *head = li->head;
    while (head) {
        list_item *tmp = head;
        head = head->next;
        if (li->fn_table->free_fn && tmp->data.ptr)
            li->fn_table->free_fn(tmp->data.ptr);
        list_item_free(tmp);
    }
    mem_free(&list_pool, li);
}

void list_add_back(list *li, def_data data) {
    list_item *item = list_item_init(data);
    if (li->tail) {
        item->prev = li->tail;
        li->tail = li->tail->next = item;
    } else
        li->head = li->tail = item;
}

size_t list_hash(const list *li) {
    size_t hash = 0;
    for (list_item *head = li->head; head; head = head->next)
        hash += li->fn_table->hash_fn(head->data);
    return hash;
}

bool list_eq(const list *li_a, const list *li_b) {
    if (li_a == li_b)
        return true;
    if (!li_a || !li_b)
        return false;
    list_item *head_a = li_a->head, *head_b = li_b->head;
    while (head_a && head_b) {
        if (!li_a->fn_table->eq_fn(head_a->data, head_b->data))
            return false;
        head_a = head_a->next;
        head_b = head_b->next;
    }
    return !head_a && !head_b;
}

void list_print(const list *li, FILE *file, int32_t idnt, list_print_opts print_opts) {
    for (list_item *head = li->head; head; head = head->next) {
        int32_t data_idnt = idnt;
        if (head == li->head && (print_opts & LIST_PRINT(NO_FIRST_IDNT)))
            data_idnt = 0;
        li->fn_table->print_fn(head->data, file, data_idnt, li->print_opts);
        if ((print_opts & LIST_PRINT(SEMI_SPACER)) && head->next)
            fprintf(file, COLOR(DARK_GREY) ";" COLOR(RESET));
    }
    if (print_opts & LIST_PRINT(NL_END))
        fprintf(file, "\n");
}
