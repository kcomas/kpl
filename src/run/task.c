
#include "./task.h"

MEM_POOL(task_pool);

static atomic_bool running = true;

static pthread_t threads[THREADS] = {};

static task *task_queue_head = nullptr, *task_queue_tail = nullptr;

static void task_queue_add(task *ta) {
    if (task_queue_tail) {
        ta->prev = task_queue_tail;
        task_queue_tail = task_queue_tail->next = ta;
    } else
        task_queue_head = task_queue_tail = ta;
}

static task *task_queue_next(void) {
    if (!task_queue_head)
        return nullptr;
    task *ta = task_queue_head;
    if (task_queue_head != task_queue_tail) {
        task_queue_head = task_queue_head->next;
        task_queue_head->prev = nullptr;
    } else
        task_queue_head = task_queue_tail = nullptr;
    ta->prev = ta->next = nullptr;
    return ta;
}

static void task_delete(task *ta) {
    if (ta->fn_table->free_fn && ta->data.ptr)
        ta->fn_table->free_fn(ta->data.ptr);
    if (ta->er)
        error_free(ta->er);
    sem_destroy(&ta->sem);
    mem_free(&task_pool, ta);
}

static pthread_mutex_t task_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t task_queue_cond = PTHREAD_COND_INITIALIZER;

static void *thread_runner(void *arg) {
    size_t thread_idx = (size_t) arg;
    (void) thread_idx;
    while (running) {
        pthread_mutex_lock(&task_queue_mutex);
        task *ta = task_queue_next();
        if (!ta) {
            pthread_cond_wait(&task_queue_cond, &task_queue_mutex);
            pthread_mutex_unlock(&task_queue_mutex);
            continue;
        } else
            pthread_mutex_unlock(&task_queue_mutex);
        ta->er = ta->fn(ta->data);
        sem_post(&ta->sem);
        int sem_value;
        sem_getvalue(&ta->sem, &sem_value);
        if (sem_value > 1)
            task_delete(ta);
    }
    return nullptr;
}

[[gnu::constructor(DEF_CONSTRUCTOR_TASK)]] static void task_constructor(void) {
    for (size_t thread_idx = 0; thread_idx < THREADS; thread_idx++)
        if (pthread_create(&threads[thread_idx], nullptr, thread_runner, (void*) thread_idx) != 0) {
            printf(COLOR2(BOLD, RED) "Failed To Create Thread Exiting" COLOR(RESET));
            exit(DEF_EXIT_ERROR);
        }
}

[[gnu::destructor(DEF_DESTRUCTOR_TASK)]] static void task_destructor(void) {
    running = false;
    for (size_t thread_idx = 0; thread_idx < THREADS; thread_idx++)
        pthread_cond_signal(&task_queue_cond);
    for (size_t thread_idx = 0; thread_idx < THREADS; thread_idx++)
        pthread_join(threads[thread_idx], nullptr);
    while (task_queue_head) {
        task *tmp = task_queue_head;
        task_queue_head = task_queue_head->next;
        task_delete(tmp);
    }
    pthread_cond_destroy(&task_queue_cond);
    pthread_mutex_destroy(&task_queue_mutex);
}

static task *_task_init(uint32_t print_opts, def_fn_table *fn_table, def_data data, task_fn fn, int sem_value) {
    task *ta = mem_alloc(&task_pool, sizeof(task));
    ta->print_opts = print_opts;
    ta->fn_table = fn_table;
    ta->data = data;
    ta->fn = fn;
    ta->er = nullptr;
    sem_init(&ta->sem, 0, sem_value);
    pthread_mutex_lock(&task_queue_mutex);
    task_queue_add(ta);
    pthread_mutex_unlock(&task_queue_mutex);
    pthread_cond_signal(&task_queue_cond);
    return ta;
}

task *task_init(uint32_t print_opts, def_fn_table *fn_table, def_data data, task_fn fn) {
    return _task_init(print_opts, fn_table, data, fn, 0);
}

task *task_init_detached(uint32_t print_opts, def_fn_table *fn_table, def_data data, task_fn fn) {
    return _task_init(print_opts, fn_table, data, fn, 1);
}

void task_free(task *ta) {
    if (!ta)
        return;
    sem_wait(&ta->sem);
    task_delete(ta);
}

result task_join(task *ta) {
    if (!ta)
        return result_error(ERROR_INIT(0, &def_unused_fn_table, def(), "THREAD ALREADY JOINED"));
    sem_wait(&ta->sem);
    result re;
    if (ta->er) {
        re = result_error(ta->er);
        ta->er = nullptr;
    } else {
        re = result_value(ta->data);
        ta->data = def();
    }
    task_delete(ta);
    return re;
}
