#include "benchmark.h"
#include "printer.h"

#include <assert.h>

// global variable
lock_free_list_t *list = NULL;
lock_free_list_t *other = NULL;

/*
* Sample garbage collector
*/
static void cleaner(void *elem)
{
    lock_free_list_t *list = (lock_free_list_t *) elem;
    if (list == NULL)
    {
        return;
    }

    node_t *iterator = list->head;
    if (iterator == NULL)
    {
        free(list);
        return;
    }

    if (iterator->next == NULL)
    {
        free(iterator);
        free(list);
        return;
    }

    node_t *tmp = NULL;
    while ((tmp = iterator->next) != NULL)
    {
        free(iterator->data);
        free(iterator);
        iterator = tmp;
    }

    free(list);
}

/*
* Count time feature
*/
static unsigned long long tick(void)
{
    unsigned long low, high;
    __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high));

    return RETURN_TIME;
}

/*
* Run the thread with passed thread_variable to it
*/
static void *run(void *thread_variable)
{
    // detached implementation is faster
    pthread_detach(pthread_self());
    int my_inserts = 0;
    while (my_inserts != INSERTS)
    {
        node_t *element = calloc(1, sizeof(node_t));
        if (element != NULL)
        {
            //cleanup push are important if we need save data to nodes
            // pthread_cleanup_push(cleaner, element);
            insert(&(list->head), element, list->head);

            my_inserts++;
            // pthread_cleanup_pop(1);
        }
    }
    pthread_exit(NULL);
    return NULL;
}

void execute_with_threads()
{
    // Initialize list
    printf("Executing with threads...\n");
    int rc = init_list(&list);
    assert(rc == SUCCESS);

    if (list != NULL)
    {
        pthread_cleanup_push(cleaner, list);
        node_t *head = calloc(1, sizeof(node_t));
        assert(head != NULL);
        // list->head = head
        while (compare_and_swap(&(list->head), NULL, head) == FALSE);

        // Create and run threads
        pthread_t tid[THREAD_COUNT];
        pthread_attr_t pthread_attr = { 0 };
        pthread_attr_init(&pthread_attr);
        // pthread_attr_setdetachstate(&pthread_attr, PTHREAD_CREATE_DETACHED);
        pthread_attr_setstacksize(&pthread_attr, THREAD_STACK_SIZE);

        for (int i = 0; i < THREAD_COUNT; i++)
        {
            rc = pthread_create(&tid[i], NULL, run, (void *) &tid);
            assert(rc == SUCCESS);
            // pthread_detach(tid);
        }

        // With joins it stucks forever
        // for (int j = 0; j < THREAD_COUNT; j++)
        // {
        //     rc = pthread_join(tid[j], NULL);
        //     assert(rc == SUCCESS);
        // }

        pthread_attr_destroy(&pthread_attr);
        printf("THREADS INSERTIONS COUNT: %d\n", THREAD_COUNT * INSERTS);
        print_summary(list);
        pthread_cleanup_pop(1);
    }
}

void execute_regular()
{
    // Initialize list
    printf("Executing without threads...\n");
    int rc = init_list(&other);
    assert(rc == SUCCESS);

    node_t *head = calloc(1, sizeof(node_t));
    assert(head != NULL);
    other->head = head;

    for (int i = 0; i < THREAD_COUNT * INSERTS; i++)
    {
        node_t *element = calloc(1, sizeof(node_t));
        if (element != NULL)
        {
            insert(&(other->head), element, other->head);
        }
    }
    printf("REGULAR INSERTIONS COUNT: %d\n", THREAD_COUNT * INSERTS);
    print_summary(other);
    free_list(other);
}

// If threads are detached, actions passed, but with memory leaks
int main(void)
{
    unsigned long long tb, te, delta_threads, delta_regular;
    tb = tick();
    execute_regular();
    te = tick();
    delta_regular = te - tb;

    tb = tick();
    execute_with_threads();
    te = tick();
    delta_threads = te - tb;

    printf("SUMMARY TICKS:\n");
    printf("WITH THREADS:\t\t%llu ticks\n", delta_threads);
    printf("WITHOUT THREADS:\t\t%llu ticks\n", delta_regular);

    return SUCCESS;
}
