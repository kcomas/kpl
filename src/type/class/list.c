
#include "./list.h"

MEM_POOL(list_item_pool);

static type_list_item *type_list_item_init(type *inner_type) {
    type_list_item *list_item = mem_alloc(&list_item_pool, sizeof(type_list_item));
    list_item->inner_type = inner_type;
    return list_item;
}

static void type_list_item_free(type_list_item *list_item) {
    if (list_item->inner_type)
        type_free(list_item->inner_type);
    mem_free(&list_item_pool, list_item);
}

MEM_POOL(list_pool);

type_list *type_list_init(void) {
    type_list *list = mem_alloc(&list_pool, sizeof(type_list));
    list->len = 0;
    list->head = list->tail = NULL;
    return list;
}

void type_list_free(type_list *list) {
    type_list_item *head = list->head;
    while (head) {
        type_list_item *tmp = head;
        head = head->next;
        type_list_item_free(tmp);
    }
    mem_free(&list_pool, list);
}

void type_list_add(type_list *list, type *inner_type) {
    type_list_item *list_item = type_list_item_init(inner_type);
    if (list->tail) {
        list_item->prev = list->tail;
        list->tail = list->tail->next = list_item;
    } else
        list->head = list->tail = list_item;
    list->len++;
}

void type_list_print(const type_list *list, FILE *file, uint32_t idnt, type_print_opts opts) {
    fprintf(file, "%*s", idnt, "");
    fprintf(file, COLOR2(BOLD, WHITE) "[" COLOR(RESET));
    for (type_list_item *head = list->head; head; head = head->next) {
        type_print(head->inner_type, file, 0, opts);
        if (head->next)
            fprintf(file, COLOR(DARK_GREY) "; " COLOR(RESET));
    }
    fprintf(file, COLOR2(BOLD, WHITE) "]" COLOR(RESET));
    if (opts & TYPE_PRINT(CLASS_NL_END))
        fprintf(file, "\n");
}
