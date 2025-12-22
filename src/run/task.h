
#pragma once

#include "./header.h"
#include "./result.h"

#ifndef THREADS
#error "Thread Count Undefined"
#endif

#define TASK_STATUS(NAME) TASK_STATE_##NAME

typedef enum [[gnu::packed]] {
    TASK_STATUS(RUN),
    TASK_STATUS(DONE),
    TASK_STATUS(DETEACHED)
} task_status;

typedef error *task_fn(def_data data);

typedef struct _task {
    RUN_HEADER(_task);
    def_data data;
    task_fn *fn;
    error *er;
    sem_t sem;
    _Atomic task_status status;
} task;

task *task_init(uint32_t print_opts, def_fn_table *fn_table, def_data data, task_fn fn);

void task_free(task *ta);

result task_join(task *ta);
