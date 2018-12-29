#include "printer.h"

static int count_nodes(lock_free_list_t *list)
{
    if (list == NULL)
    {
        return 0;
    }

    node_t *iterator = list->head;
    int result = 0;
    while (iterator != NULL)
    {
        result++;
        iterator = iterator->next;
    }

    return result;
}

void print_addresses(lock_free_list_t *list)
{
    if (list == NULL)
    {
        printf("Empty list!\n");
        return;
    }

    int count = count_nodes(list);
    printf("TOTAL NODES COUNT: %d\n", count);
    node_t *iterator = list->head;
    int i = 0;
    while (iterator != NULL)
    {
        printf("Item[%d] address: %p\n", i, (void *) iterator);
        iterator = iterator->next;
        i++;
    }
}

void print_summary(lock_free_list_t *list)
{
    if (list == NULL)
    {
        printf("Empty list!\n");
        return;
    }

    int count = count_nodes(list);
    printf("TOTAL NODES COUNT: %d\n", count);
}
